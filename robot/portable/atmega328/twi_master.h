#ifndef TWI_MASTER_H
#define TWI_MASTER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "pio.h"

#define TWI_FREQ (100000)

class TwiMaster
{
public:
    typedef PioC5 SclPin;
    typedef PioC4 SdaPin;

    enum{
        // not ready
        STATUS_REQUESTING       = 0x00,
        STATUS_RECEIVING        = 0x01,
        STATUS_TRANSFERRING     = 0x02,

        // ready
        STATUS_READY            = 0x10,
        STATUS_TX_ADDR_NACK_ERR = 0x11,
        STATUS_TX_DATA_NACK_ERR = 0x12,
        STATUS_OTHER_MASTER_ERR = 0x13,
        STATUS_OTHER_ERR        = 0x14
    };

    static inline void init()
    {
        //bit rate
        TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;   // bit rate
        TWSR &= ~((1<<TWPS1) | (1<<TWPS0));     //prescaler - 1

        SclPin::setHigh();
        SdaPin::setHigh();

        status_ = STATUS_READY;

        transfer_buffer_        = 0;
        transfer_buffer_size_   = 0;
        transfer_buffer_index_  = 0;

        slave_address_          = 0;

        receive_buffer_         = 0;
        receive_buffer_size_    = 0;
        receive_buffer_index_   = 0;

        // work_log_index_ = 0;
    }

    static inline void disable()
    {
        TWCR &= ~((1<<TWEN) | (1<<TWIE) | (1<<TWEA));

        SclPin::setLow();
        SdaPin::setLow();
    }

    static void write_to(uint8_t address, const void* buffer, uint16_t size)
    {
        if(!buffer)
            return;

        slave_address_          = address;
        transfer_buffer_        = static_cast<const uint8_t*>(buffer);
        transfer_buffer_size_   = size;
        transfer_buffer_index_  = 0;

        status_ = STATUS_TRANSFERRING;

        send_start();
    }

    static void request_from(uint8_t address,
            const void* request_buffer, uint16_t request_size,
            void* respond_buffer, uint16_t respond_buffer_size)
    {
        if(!respond_buffer)
            return;

        slave_address_ = address;

        transfer_buffer_        = static_cast<const uint8_t*>(request_buffer);
        transfer_buffer_size_   = request_size;
        transfer_buffer_index_  = 0;

        receive_buffer_         = static_cast<uint8_t*>(respond_buffer);
        receive_buffer_size_    = respond_buffer_size;
        receive_buffer_index_   = 0;

        status_ = STATUS_REQUESTING;

        send_start();
    }

    static bool is_ready()
    {
        return status_ & 0xf0;
    }

    static uint16_t available()
    {
        uint16_t tmp = 0;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            tmp = transfer_buffer_index_ + 1;
        }

        return tmp;
    }

    static uint8_t status()
    {
        uint8_t tmp = STATUS_READY;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            tmp = status_;
        }

        return tmp;
    }

private:
    static inline void send_start()
    {
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWEA) | (1<<TWIE);
    }

    static inline void send_stop()
    {
        TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWSTO);
    }

    static inline void reply(bool ack)
    {
        if(ack){
            TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT) | (1<<TWEA);
        }else{
            TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT);
        }
    }

public:
    static inline void interrupt_handler()
    {
        // if(TWSR != 0){
        //     work_log_[work_log_index_] = TWSR & 0xf8;
        // }else{
        //     work_log_[work_log_index_] = 0xff;
        // }
        // work_log_index_++;

        switch(TWSR & 0xf8)
        {
        case 0x08: //start
        case 0x10: //restart
            if(status_ == STATUS_RECEIVING){
                TWDR = (slave_address_ << 1) | (1<<TWD0);
            }else{
                TWDR = (slave_address_ << 1);
            }
            reply(true);
            break;

        // Transfering
        case 0x18: //SLA+W ACK
        case 0x28: //байт был послан и получен ACK
            if(transfer_buffer_index_ < transfer_buffer_size_){
                TWDR = transfer_buffer_[transfer_buffer_index_];
                transfer_buffer_index_++;
                reply(true);
            }else if(status_ == STATUS_TRANSFERRING){
                status_ = STATUS_READY;
                send_stop();
            }else if(status_ == STATUS_REQUESTING){
                status_ = STATUS_RECEIVING;
                send_start();
            }
            break;

        case 0x20: //SLA+W NACK
            status_ = STATUS_TX_ADDR_NACK_ERR;
            send_stop();
            break;

        case 0x30: //байт был послан но получен NACK
            status_ = STATUS_TX_DATA_NACK_ERR;
            send_stop();
            break;

        case 0x38: //арбитраж был проигран
            status_ = STATUS_OTHER_MASTER_ERR;
            break;

        // Receiving
        case 0x50: //data byte has ben received ACK
            receive_buffer_[receive_buffer_index_] = TWDR;
            receive_buffer_index_++;
        case 0x40: //SLA+R ACK
            if(receive_buffer_index_ + 1 < receive_buffer_size_){
                reply(true);
            }else{
                reply(false);
            }
            break;

        case 0x58: //data byte has been receiver NACK
            receive_buffer_[receive_buffer_index_] = TWDR;
            receive_buffer_index_++;
            status_ = STATUS_READY;
            send_stop();
            break;

        case 0x48: //SLA+R NACK
            status_ = STATUS_TX_ADDR_NACK_ERR;
            send_stop();
            break;

        default:
            status_ = STATUS_OTHER_ERR;;
        }
    }

// volatile static uint8_t work_log_[20];
// volatile static uint8_t work_log_index_;

private:
    static const uint8_t*   transfer_buffer_;
    static uint16_t         transfer_buffer_size_;
    static uint16_t         transfer_buffer_index_;

    static uint8_t slave_address_;

    static uint8_t*     receive_buffer_;
    static uint16_t     receive_buffer_size_;
    static uint16_t     receive_buffer_index_;

    volatile static uint8_t status_;

};

#endif
