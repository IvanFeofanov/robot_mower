#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
#include <avr/interrupt.h>

template<
    uint8_t RX_BUFFER_SIZE
    >
class Twi
{
public:

    enum{
        STATUS_TRANSFERRING,
        STATUS_COMPLATE
    };

    //master
    static void init()
    {
        //bit rate
        TWBR = 72;
        TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); //prescaler - 1

        DDRC &= ~(1<<5|1<<4);
        PORTC |= (1<<5) | (1<<4);

        status_ = STATUS_COMPLATE;

        work_log_index_ = 0;
    }

    //slave
    static void init(uint8_t address)
    {
        TWAR = address;     //slave address
        TWCR |= (1<<TWEA)   //TWE enable acknowledge
                |(1<<TWEN)  //TWE enable
                |(1<<TWIE); //TWE enable interrupt

    }

    static void write(uint8_t address, uint8_t* buffer, uint8_t size)
    {
        transfer_buffer_ = buffer;
        transfer_buffer_size_ = size;
        transfer_buffer_current_index_ = 0;
        slave_address_ = address;

        //send start
        TWCR |= (1<<TWINT) //
                |(1<<TWSTA)
                |(1<<TWEN)
                |(1<<TWIE);

        status_ = STATUS_TRANSFERRING;
    }

    static uint8_t getWritingStatus()
    {
        return status_;
    }

public:
    static void interruptHandler()
    {
        if(TWSR != 0){
            work_log_[work_log_index_] = TWSR;
        }else{
            work_log_[work_log_index_] = 0xff;
        }
        work_log_index_++;

        switch(TWSR & 0xf8)
        {
        case 0x08: //послан старт
            TWDR = slave_address_ << 1; //запись адреса и бит записи
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            break;

        case 0x10: //отправлен повторный старт
            // PORTB |= (1<<5);
            break;

        case 0x18: //адрес послан и получен ACK
            TWDR = transfer_buffer_[transfer_buffer_current_index_];
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
            transfer_buffer_current_index_++;
            break;

        case 0x20: //адрес послан но получен NACK
            // PORTB |= (1<<5);
            break;

        case 0x28: //байт был послан и получен ACK
            if(transfer_buffer_current_index_ < transfer_buffer_size_){
                TWDR = transfer_buffer_[transfer_buffer_current_index_];
                TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
                transfer_buffer_current_index_++;

            }else{
                TWCR = (1<<TWINT) | (1<<TWEN) | (0<<TWIE) | (TWSTO);
                status_ = STATUS_COMPLATE;
            }
            break;

        case 0x30: //байт был послан но получен NACK
            break;

        case 0x40:
            // PORTB |= (1<<5);
            break;
        case 0x48:
            // PORTB |= (1<<4);
            break;
        case 0x50:
            break;
        case 0x58:
            break;

        default:
            // PORTB |= (1<<5);
            break;
        }
    }

static uint8_t work_log_[20];
static uint8_t work_log_index_;

private:
    static uint8_t* transfer_buffer_;
    static uint8_t transfer_buffer_size_;
    static uint8_t transfer_buffer_current_index_;
    static uint8_t slave_address_;
    static uint8_t status_;

    static uint8_t receive_buffer_[RX_BUFFER_SIZE];
    static uint8_t receive_buffer_current_index_;

};

template<uint8_t RX_BUFFER_SIZE>
uint8_t* Twi<RX_BUFFER_SIZE>::transfer_buffer_;

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::transfer_buffer_size_;

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::transfer_buffer_current_index_;

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::slave_address_;

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::receive_buffer_[RX_BUFFER_SIZE];

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::receive_buffer_current_index_;

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::status_;

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::work_log_[20];

template<uint8_t RX_BUFFER_SIZE>
uint8_t Twi<RX_BUFFER_SIZE>::work_log_index_;

typedef Twi<2> I2c;

#endif
