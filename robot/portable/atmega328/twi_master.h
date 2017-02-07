#ifndef TWI_MASTER_H
#define TWI_MASTER_H

#include <avr/io.h>
#include <avr/interrupt.h>

class TwiMaster
{
public:
    enum{
        STATUS_REQUESTING,
        STATUS_RECEIVING,
        STATUS_TRANSFERRING,
        STATUS_READY
    };

    //master
    static inline void init()
    {
        //bit rate
        TWBR = 72;
        TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); //prescaler - 1

        DDRC &= ~(1<<5|1<<4);
        PORTC |= (1<<5) | (1<<4);

        status_ = STATUS_READY;

        // work_log_index_ = 0;
    }

    static void write(uint8_t address, uint8_t* buffer, uint8_t size)
    {
        transfer_buffer_ = buffer;
        transfer_buffer_size_ = size;
        transfer_buffer_current_index_ = 0;
        slave_address_ = address;

        status_ = STATUS_TRANSFERRING;

        //send start
        TWCR = (1<<TWINT) //
                |(1<<TWSTA)
                |(1<<TWEN)
                |(1<<TWIE);
    }

    static void requestFrom(uint8_t address,
            uint8_t* request_buffer, uint8_t request_size,
            uint8_t* get_buffer, uint8_t get_buffer_size)
    {
        slave_address_ = address;

        transfer_buffer_ = request_buffer;
        transfer_buffer_size_ = request_size;
        transfer_buffer_current_index_ = 0;

        receive_buffer_ = get_buffer;
        receive_buffer_size_ = get_buffer_size;
        receive_buffer_current_index_ = 0;

        status_ = STATUS_REQUESTING;

        //send start
        TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWIE);

    }


    static uint8_t getStatus()
    {
        return status_;
    }

public:
    static void interruptHandler()
    {
        // if(TWSR != 0){
        //     work_log_[work_log_index_] = TWSR;
        // }else{
        //     work_log_[work_log_index_] = 0xff;
        // }
        // work_log_index_++;

        switch(TWSR & 0xf8)
        {
        case 0x08: //start
            TWDR = (slave_address_ << 1);

            if(transfer_buffer_size_ == 0)
                TWDR |= (1<<TWD0); //read

            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            break;

        case 0x10: //restart
            TWDR = (slave_address_ << 1) | (1<<TWD0);

            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            break;

        case 0x18: //SLA+W ACK
            TWDR = transfer_buffer_[transfer_buffer_current_index_];
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            transfer_buffer_current_index_++;
            break;

        case 0x20: //SLA+W NACK
            // PORTB |= (1<<5);
            break;

        case 0x28: //байт был послан и получен ACK
            if(transfer_buffer_current_index_ < transfer_buffer_size_){
                TWDR = transfer_buffer_[transfer_buffer_current_index_];
                TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
                transfer_buffer_current_index_++;
            }else{
                switch(status_)
                {
                case STATUS_TRANSFERRING:
                    //stop
                    TWCR = (1<<TWINT) | (1<<TWEN) | (0<<TWIE) | (TWSTO);
                    status_ = STATUS_READY;
                    break;

                case STATUS_REQUESTING:
                    //restart
                    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWIE);
                    break;

                }
            }
            break;

        case 0x30: //байт был послан но получен NACK
            break;

        case 0x38: //арбитраж был проигран
            // PORTB |= (1<<5);
            break;

        case 0x40: //SLA+R ACK
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
            break;

        case 0x48: //SLA+R NACK
            // PORTB |= (1<<4);
            break;

        case 0x50: //data byte has ben received ACK
            receive_buffer_[receive_buffer_current_index_] = TWDR;
            if(receive_buffer_current_index_ + 1 >= receive_buffer_size_){
                TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            }else{
                receive_buffer_current_index_++;
                TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
            }
            break;

        case 0x58: //data byte has been receiver NACK
            status_ = STATUS_READY;
            TWCR = (1<<TWINT) | (1<<TWEN) | (0<<TWIE) | (1<<TWSTO);
            break;
        }
    }

volatile static uint8_t work_log_[20];
volatile static uint8_t work_log_index_;

private:
    static uint8_t* transfer_buffer_;
    static uint8_t transfer_buffer_size_;
    static uint8_t transfer_buffer_current_index_;
    static uint8_t slave_address_;

    static uint8_t* receive_buffer_;
    static uint8_t receive_buffer_size_;
    static uint8_t receive_buffer_current_index_;

    volatile static uint8_t status_;

};

#endif
