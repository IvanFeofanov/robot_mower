#ifndef TWI_SLAVE_INTERFACE_H
#define TWI_SLAVE_INTERFACE_H

#include <stdint.h>

#include "utils/flag.h"

struct Register
{
	uint8_t* 	tx_buffer;
	uint8_t* 	rx_buffer;
	uint8_t 	len;
	bool 		is_available;
	Flag*		is_new_value_ptr;
};

template<
	typename Twi,
	uint8_t NUM_BUFFERS
	>
class TwiSlaveInterface
{
public:
	static inline void init(uint8_t address)
	{
		Twi::init(address);
		Twi::attach_handler(&handler);

        state_ = ST_RX_ADDRESS;
	}

    static void handler(uint8_t twi_state)
    {
        switch(twi_state)
        {
        case Twi::ST_WRITE:
            Twi::attach_rx_buffer(&register_address_, 1);
            state_ = ST_RX_ADDRESS;
            break;

        case Twi::ST_DATA_RECEIVED:
            if(state_ == ST_RX_ADDRESS){
                Twi::attach_rx_buffer(  registers_[register_address_].rx_buffer,
                                        registers_[register_address_].len);
                registers_[register_address_].is_available = false;
                state_ = ST_RX_DATA;
            }else if(state_ == ST_RX_DATA){
                registers_[register_address_].is_available = true;
            }
            break;

        case Twi::ST_READ:
            Twi::attach_tx_buffer(  registers_[register_address_].tx_buffer,
                                    registers_[register_address_].len);

            state_ = ST_TX_DATA;
            break;

        case Twi::ST_DATA_TRANSMITTED:
            if(state_ == ST_TX_DATA){
                registers_[register_address_].is_available = true;
                if(registers_[register_address_].is_new_value_ptr){
                    registers_[register_address_].is_new_value_ptr->set();
                }
            }
            state_ = ST_RX_ADDRESS;
            break;

        default:
            break;
        }
    }

	static void set_register(uint8_t id, void* tx_buffer, void* rx_buffer, uint8_t len, Flag* is_new_value)
	{
		registers_[id].tx_buffer	= static_cast<uint8_t*>(tx_buffer);
		registers_[id].rx_buffer	= static_cast<uint8_t*>(rx_buffer);
		registers_[id].len			= len;
		registers_[id].is_available	= true;
		registers_[id].is_new_value_ptr = is_new_value;
        registers_[id].is_new_value_ptr->check();
	}

	static inline bool is_available(uint8_t id)
	{
		return registers_[id].is_available;
	}

private:
    enum{
        ST_RX_ADDRESS,
        ST_RX_DATA,
        ST_TX_DATA
    };
	static Register registers_[NUM_BUFFERS];
    static uint8_t register_address_;
    static uint8_t state_;
};

template<
	typename TwiSlave,
	uint8_t	 NUM_BUFFERS
	>
Register TwiSlaveInterface<TwiSlave, NUM_BUFFERS>::registers_[NUM_BUFFERS];

template<
	typename TwiSlave,
	uint8_t	 NUM_BUFFERS
	>
uint8_t TwiSlaveInterface<TwiSlave, NUM_BUFFERS>::register_address_;

template<
	typename TwiSlave,
	uint8_t	 NUM_BUFFERS
	>
uint8_t TwiSlaveInterface<TwiSlave, NUM_BUFFERS>::state_;

#endif
