#include "twi_master.h"

extern uint8_t* TwiMaster::transfer_buffer_;
extern uint8_t TwiMaster::transfer_buffer_size_;
extern uint8_t TwiMaster::transfer_buffer_current_index_;

extern uint8_t TwiMaster::slave_address_;

extern uint8_t* TwiMaster::receive_buffer_;
extern uint8_t TwiMaster::receive_buffer_size_;
extern uint8_t TwiMaster::receive_buffer_current_index_;

extern volatile uint8_t TwiMaster::status_;

extern volatile uint8_t TwiMaster::work_log_[20];
extern volatile uint8_t TwiMaster::work_log_index_;


ISR(TWI_vect)
{
    TwiMaster::interruptHandler();
}
