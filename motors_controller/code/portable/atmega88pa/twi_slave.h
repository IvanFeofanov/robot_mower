#ifndef TWI_SLAVE_H
#define TWI_SLAVE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

typedef void (*TwiCallbackPtr)(uint8_t);

template<uint8_t RECEIVE_BUFFER_SIZE>
class TwiSlave
{
public:
    enum{
        ST_WRITE,
        ST_DATA_RECEIVED,
        ST_READ,
        ST_DATA_TRANSMITTED,
        ST_ERR
    };

public:
    static inline void init(uint8_t address, bool is_broadcast = false, uint8_t address_mask = 0)
    {
        TWAR    = (address << 1) | (is_broadcast ? 1 : 0);
        TWCR    = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        TWAMR   = address_mask;

        receive_buffer_ = 0;
        transfer_buffer_ = 0;
        handler_ptr_ = 0;

        //DEBUG ONLY
        // log_index = 0;
    }

    static inline void attach_handler(TwiCallbackPtr function)
    {
        handler_ptr_ = function;
    }

    static void attach_tx_buffer(const void* buffer, uint8_t size)
    {
        transfer_buffer_       = static_cast<const uint8_t*>(buffer);
        transfer_buffer_size_  = size;
        transfer_buffer_index_ = 0;
    }

    static void attach_rx_buffer(void* buffer, uint8_t size)
    {
        receive_buffer_         = static_cast<uint8_t*>(buffer);
        receive_buffer_size_    = size;
        receive_buffer_index_   = 0;
    }

private:
    static inline void reply(bool ack)
    {
        if(ack){
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
        }else{
            TWCR = (1<<TWINT) | (0<<TWEA) | (1<<TWEN) | (1<<TWIE);
        }
    }

    static inline void handler(uint8_t state)
    {
        if(handler_ptr_){
            handler_ptr_(state);
        }
    }

public:
    // DEBUG ONLY
    // static volatile uint8_t log[20];
    // static volatile uint16_t log_index;

    static inline void interrupt_handler()
    {
        // DEBUG ONLY
        // log[log_index] = TWSR & 0xf8;
        // log_index++;

        switch(TWSR & 0xf8)
        {
        case 0x70: //Receive SLA+W broadcast
        case 0x60: //Received SLA+W
            receive_buffer_       = 0;
            receive_buffer_index_ = 0;

            handler(ST_WRITE);

            if(!receive_buffer_){
                reply(false);
            }else{
                reply(true);
            }
            break;

        case 0x90: //Receive byte broadcast
        case 0x80: //Receive byte
            if(receive_buffer_){
                receive_buffer_[receive_buffer_index_] = TWDR;
            }
            receive_buffer_index_++;

            //data received
            if(receive_buffer_index_ >= receive_buffer_size_){
                receive_buffer_ = 0;
                handler(ST_DATA_RECEIVED);
                if(!receive_buffer_){
                    reply(false);
                }else{
                    reply(true);
                }
            }else{
                reply(true);
            }
            break;

        case 0x98: //Receive last byte broadcast
        case 0x88: //Receive last byte
            //ignore received byte
            reply(true); //if false then go to master mode
            break;

        case 0xA0: //Receive reStart
            reply(true);
            break;

        case 0xA8: //Receive SLA+R
            transfer_buffer_       = 0;
            transfer_buffer_index_ = 0;
            handler(ST_READ);

            if(transfer_buffer_){
                TWDR = transfer_buffer_[transfer_buffer_index_];
                transfer_buffer_index_++;
                reply(true);
            }else{
                TWDR = 0xff;
                reply(false);
            }
            break;

        case 0xB8: //Send byte receive ACK
            if(transfer_buffer_index_ >= transfer_buffer_size_){
                transfer_buffer_       = 0;
                transfer_buffer_index_ = 0;
                handler(ST_DATA_TRANSMITTED);
                if(transfer_buffer_){
                    TWDR = transfer_buffer_[transfer_buffer_index_];
                    transfer_buffer_index_++;
                    reply(true);
                }else{
                    TWDR = 0xff;
                    reply(false);
                }
            }else{
                TWDR = transfer_buffer_[transfer_buffer_index_];
                transfer_buffer_index_++;
                reply(true);
            }
            break;

        case 0xC0: //Send last byte receive NACK
        case 0xC8: //Send last byte receive ACK
            reply(true);
            break;

        case 0xF8: //No relevant state inforamation, TWINT=0
            break;

        case 0x00: //Bus error
            handler(ST_ERR);
            break;

        default:
            break;
        }
    }

private:
    static const uint8_t* transfer_buffer_;
    static uint8_t transfer_buffer_size_;
    static uint8_t transfer_buffer_index_;

    static uint8_t* receive_buffer_;
    static uint8_t  receive_buffer_size_;
    static uint8_t  receive_buffer_index_;

    static TwiCallbackPtr handler_ptr_;
};

template<uint8_t RECEIVE_BUFFER_SIZE>
const uint8_t* TwiSlave<RECEIVE_BUFFER_SIZE>::transfer_buffer_;

template<uint8_t RECEIVE_BUFFER_SIZE>
uint8_t TwiSlave<RECEIVE_BUFFER_SIZE>::transfer_buffer_size_;

template<uint8_t RECEIVE_BUFFER_SIZE>
uint8_t TwiSlave<RECEIVE_BUFFER_SIZE>::transfer_buffer_index_;

template<uint8_t RECEIVE_BUFFER_SIZE>
uint8_t* TwiSlave<RECEIVE_BUFFER_SIZE>::receive_buffer_;

template<uint8_t RECEIVE_BUFFER_SIZE>
uint8_t TwiSlave<RECEIVE_BUFFER_SIZE>::receive_buffer_size_;

template<uint8_t RECEIVE_BUFFER_SIZE>
uint8_t TwiSlave<RECEIVE_BUFFER_SIZE>::receive_buffer_index_;

template<uint8_t RECEIVE_BUFFER_SIZE>
TwiCallbackPtr TwiSlave<RECEIVE_BUFFER_SIZE>::handler_ptr_;

// DEBUG ONLY
// template<uint8_t RECEIVE_BUFFER_SIZE>
// volatile uint8_t TwiSlave<RECEIVE_BUFFER_SIZE>::log[20];
//
// template<uint8_t RECEIVE_BUFFER_SIZE>
// volatile uint16_t TwiSlave<RECEIVE_BUFFER_SIZE>::log_index;

typedef TwiSlave<0> Twi;

#endif
