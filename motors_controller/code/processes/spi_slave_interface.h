#ifndef SPI_SLAVE_INTERFACE_H
#define SPI_SLAVE_INTERFACE_H

#include <stdint.h>

#include "utils/flag.h"

#include "spi_face_defs.h"

struct Register
{
	uint8_t* 	tx_buffer;
	uint8_t* 	rx_buffer;
	uint8_t 	len;
	bool 		is_available;
	Flag*		is_new_value_ptr;
};

template<
	typename SpiSlave,
	uint8_t	 NUM_BUFFERS
	>
class SpiSlaveInterface
{
public:
	static inline void init(uint8_t status)
	{
		status_ = status;

		SpiSlave::attach_handler(&request_handler);
        SpiSlave::set_data(status_);
        SpiSlave::enable();

		state_ = ST_WAIT_4_CMD;
		tx_id_ = 0;
		index_ = 0;
	}

	static void request_handler()
	{
		uint8_t data = SpiSlave::get_data();

		if(state_ == ST_WAIT_4_CMD){
			if(((data & CMD_MASK) != CMD_NOP) && ((data & REG_MASK) < NUM_REGS)){ // NO NOP
				tx_id_ = data & REG_MASK;

				if(registers_[tx_id_].tx_buffer || registers_[tx_id_].rx_buffer){
					index_ = 0;
					registers_[tx_id_].is_available = false;
                    state_ = (data & CMD_MASK) == CMD_WR_REG ? ST_WR : ST_R;

					if(registers_[tx_id_].tx_buffer){
						SpiSlave::set_data(registers_[tx_id_].tx_buffer[index_]);// first tx byte
					}

				}else{ // just command
					SpiSlave::set_data(status_);
					if(registers_[tx_id_].is_new_value_ptr)
						registers_[tx_id_].is_new_value_ptr->set();
				}
			}else{ //CMD_NOP
				SpiSlave::set_data(status_);
			}
		}else{ // transfering
			if(registers_[tx_id_].rx_buffer && state_ == ST_WR){
				registers_[tx_id_].rx_buffer[index_] = SpiSlave::get_data();
			}

			index_++;

			if(index_ < registers_[tx_id_].len){
				if(registers_[tx_id_].tx_buffer){
					SpiSlave::set_data(registers_[tx_id_].tx_buffer[index_]);
				}else{
                    SpiSlave::set_data(0xff);
                }
			}else if(index_ ==  registers_[tx_id_].len){

            }else{
				registers_[tx_id_].is_available = true;

				if(registers_[tx_id_].is_new_value_ptr && state_ == ST_WR)
					registers_[tx_id_].is_new_value_ptr->set();

				state_ = ST_WAIT_4_CMD;
				SpiSlave::set_data(status_);
			}
		}
	}

	static inline void set_status(uint8_t status)
	{
		status_ = status;
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
	static uint8_t	status_;
	static Register 	registers_[NUM_BUFFERS];

	static uint8_t 	state_;
	enum{
		ST_WAIT_4_CMD,
		ST_WR,
		ST_R
	};
	static uint8_t tx_id_;
	static uint8_t index_;
};

template<
	typename SpiSlave,
	uint8_t	 NUM_BUFFERS
	>
uint8_t SpiSlaveInterface<SpiSlave, NUM_BUFFERS>::status_;

template<
	typename SpiSlave,
	uint8_t	 NUM_BUFFERS
	>
Register SpiSlaveInterface<SpiSlave, NUM_BUFFERS>::registers_[NUM_BUFFERS];

template<
	typename SpiSlave,
	uint8_t	 NUM_BUFFERS
	>
uint8_t SpiSlaveInterface<SpiSlave, NUM_BUFFERS>::state_;

template<
	typename SpiSlave,
	uint8_t	 NUM_BUFFERS
	>
uint8_t SpiSlaveInterface<SpiSlave, NUM_BUFFERS>::tx_id_;

template<
	typename SpiSlave,
	uint8_t	 NUM_BUFFERS
	>
uint8_t SpiSlaveInterface<SpiSlave, NUM_BUFFERS>::index_;


#endif
