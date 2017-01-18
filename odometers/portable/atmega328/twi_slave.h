#ifndef TWI_SLAVE_H
#define TWI_SLAVE_H

#include <avr/io.h>
#include <avr/interrupt.h>

typedef void (*CallbackPtr)(uint8_t*, uint8_t);

template<uint8_t BUFFER_SIZE>
class TwiSlave
{
public:
    static inline void init(uint8_t address)
    {
        TWAR = address << 1;
        TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
    }

    static void onReceive(CallbackPtr function)
    {
        receive_event_ptr_ = function;
    }

    static void onRequest(CallbackPtr function)
    {
        request_event_ptr_ = function;
    }

    static uint8_t available()
    {
        return receive_buffer_write_index_ - receive_buffer_read_index_;
    }

    static uint8_t read()
    {
        uint8_t tmp = receive_buffer_[receive_buffer_read_index_];
        receive_buffer_read_index_ ++;

        return tmp;
    }

    static void write(uint8_t* buffer, uint8_t size)
    {
        transfer_buffer_ = buffer;
        transfer_buffer_size_ = size;
        transfer_buffer_index_ = 0;
    }

public:
    static inline void interruptHandler()
    {
        // if(TWSR != 0){
        //     work_log_[work_log_index_] = TWSR;
        // }else{
        //     work_log_[work_log_index_] = 0xff;
        // }
        // work_log_index_++;

        switch(TWSR & 0xf8)
        {
        case 0x78: //Receive SLA+W LP broadcast
        case 0x68: //Receive SLA+W LP
        case 0x70: //Receive SLA+W broadcast
        case 0x60: //Received SLA+W
            receive_buffer_write_index_ = 0;
            receive_buffer_read_index_ = 0;

            if(BUFFER_SIZE == 1){
                TWCR = (1<<TWINT) | (0<<TWEA) | (1<<TWEN) | (1<<TWIE);
            }else{
                TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            }
            break;

        case 0x90: //Receive byte broadcast
        case 0x80: //Receive byte
            receive_buffer_[receive_buffer_write_index_] = TWDR;

            if(receive_buffer_write_index_+2 >= BUFFER_SIZE){
                TWCR = (1<<TWINT) | (0<<TWEA) | (1<<TWEN) | (1<<TWIE);
            }else{
                TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
                receive_buffer_write_index_++;
            }
            break;

        case 0x98: //Receive last byte broadcast
        case 0x88: //Receive last byte
            receive_buffer_[receive_buffer_write_index_] = TWDR;
            if(receive_event_ptr_ != 0){
                receive_event_ptr_((uint8_t*)receive_buffer_, receive_buffer_write_index_);
            }

            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;

        case 0xA0: //Receive reStart
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;

        case 0xB0: //Receive SLA+R LP
            break;

        case 0xA8: //Receive SLA+R
            transfer_buffer_index_ = 0;
            if(request_event_ptr_ != 0){
                request_event_ptr_((uint8_t*)receive_buffer_, receive_buffer_write_index_);
            }
        case 0xB8: //Send byte receive ACK
            TWDR = transfer_buffer_[transfer_buffer_index_];

            if(transfer_buffer_index_ + 1 >= transfer_buffer_size_){
                TWCR = (1<<TWINT) | (0<<TWEA) | (1<<TWEN) | (1<<TWIE);
            }else{
                transfer_buffer_index_++;
                TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            }
            break;

        case 0xC0: //Send last byte receive NACK
        case 0xC8: //Send last byte receive ACK
            TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
            break;

        case 0xF8: //No relevant state inforamation, TWINT=0
            break;

        case 0x00: //Bus error
            break;

        default:
            break;
        }
    }

private:
    static uint8_t* transfer_buffer_;
    static uint8_t transfer_buffer_size_;
    static uint8_t transfer_buffer_index_;

    volatile static uint8_t receive_buffer_[BUFFER_SIZE];
    static uint8_t receive_buffer_write_index_;
    static uint8_t receive_buffer_read_index_;

    static CallbackPtr receive_event_ptr_;
    static CallbackPtr request_event_ptr_;


// public:
//     volatile static uint8_t work_log_[20];
//     volatile static uint8_t work_log_index_;

};

template<uint8_t BUFFER_SIZE>
uint8_t* TwiSlave<BUFFER_SIZE>::transfer_buffer_;

template<uint8_t BUFFER_SIZE>
uint8_t TwiSlave<BUFFER_SIZE>::transfer_buffer_size_;

template<uint8_t BUFFER_SIZE>
uint8_t TwiSlave<BUFFER_SIZE>::transfer_buffer_index_;

template<uint8_t BUFFER_SIZE>
volatile uint8_t TwiSlave<BUFFER_SIZE>::receive_buffer_[BUFFER_SIZE];

template<uint8_t BUFFER_SIZE>
uint8_t TwiSlave<BUFFER_SIZE>::receive_buffer_write_index_;

template<uint8_t BUFFER_SIZE>
uint8_t TwiSlave<BUFFER_SIZE>::receive_buffer_read_index_;

template<uint8_t BUFFER_SIZE>
CallbackPtr TwiSlave<BUFFER_SIZE>::receive_event_ptr_;

template<uint8_t BUFFER_SIZE>
CallbackPtr TwiSlave<BUFFER_SIZE>::request_event_ptr_;

// template<uint8_t BUFFER_SIZE>
// volatile uint8_t TwiSlave<BUFFER_SIZE>::work_log_[20];
//
// template<uint8_t BUFFER_SIZE>
// volatile uint8_t TwiSlave<BUFFER_SIZE>::work_log_index_;

typedef TwiSlave<6> Twi;

#endif
