#ifndef SPI_FACE_H
#define SPI_FACE_H

#include <stdint.h>

#include "utils/atomic.h"
#include "utils/pid.h"

#include "processes/spi_slave_transfer.h"

#define CMD_NOP         (0x00)
#define CMD_SPEED       (0x10)
#define CMD_COUNTER     (0x20)
#define CMD_COUNTER_C   (0x30)
#define CMD_W_REG       (0x40)
#define CMD_R_REG       (0x50)

#define REG_LEFT_P      (0x0)
#define REG_LEFT_I      (0x1)
#define REG_LEFT_I_MAX  (0x2)
#define REG_LEFT_I_MIN  (0x3)
#define REG_LEFT_D      (0x4)
#define REG_RIGHT_P     (0x5)
#define REG_RIGHT_I     (0x6)
#define REG_RIGHT_I_MAX (0x7)
#define REG_RIGHT_I_MIN (0x8)
#define REG_RIGHT_D     (0x9)

#define REG_MASK        (0x0f)
#define CMD_MASK        (0xf0)

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
class SpiFace
{
public:
    static inline void init()
    {
        state_ = ST_WAITING_4_CMD;

        transfer_ = SpiSlaveTransfer<SpiSlave>(&state_);

        is_new_speed_           = false;
        is_new_pid_config_      = false;
        is_reset_counter_       = false;
        is_interface_ready_     = true;

        left_set_speed_  = 0;
        right_set_speed_ = 0;

        write_values();

        SpiSlave::attach_handler(&request_handler);
        SpiSlave::set_data(get_status());
        SpiSlave::enable();
    }

    static inline void update()
    {
        Atomic atomic;

        if(!is_interface_ready_)
            return;

        // set speed
        if(is_new_speed_){
            is_new_speed_ = false;
            MotorsController::set_input_rps(left_set_speed_, right_set_speed_);
        }

        // set pid config
        if(is_new_pid_config_){

            PidConfig* left_pid_cfg_ptr;
            PidConfig* right_pid_cfg_ptr;

            MotorsController::get_pid_configs(
                    &left_pid_cfg_ptr, &right_pid_cfg_ptr);

            *left_pid_cfg_ptr   = left_pid_config_;
            *right_pid_cfg_ptr  = right_pid_config_;
        }

        // reset odometer
        if(is_reset_counter_){
            is_reset_counter_ = false;
            Odometers::reset_left_counter();
            Odometers::reset_right_counter();
        }

        write_values();
        SpiSlave::set_data(get_status());

        atomic.release();

        // save pid configs
        if(is_new_pid_config_){
            is_new_pid_config_ = false;
            MotorsController::save_configs();
        }
    }

private:
    static void request_handler()
    {
        is_interface_ready_ = false;
        uint8_t data = SpiSlave::get_data();

        if(state_ == ST_WAITING_4_CMD){
            switch(data & CMD_MASK)
            {
            case CMD_W_REG:
                break;

            case CMD_R_REG:
                break;

            default:
                break;
            }
        }else if(state_ == ST_TX){
            transfer_.update();
        }

        if(state_ == ST_END){
            state_ = ST_WAITING_4_CMD;
            SpiSlave::set_data(get_status());
            is_interface_ready_ = true;
        }
    }

    static void write_values()
    {
        // real speed
        MotorsController::real_rps(
                &left_real_speed_, &right_real_speed_);

        // counter
        left_counter_ = Odometers::left_counter();
        right_counter_ = Odometers::right_counter();

        // pid configs
        PidConfig* left_pid_cfg_ptr;
        PidConfig* right_pid_cfg_ptr;

        MotorsController::get_pid_configs(
                &left_pid_cfg_ptr, &right_pid_cfg_ptr);

        left_pid_config_  = *left_pid_cfg_ptr;
        right_pid_config_ = *right_pid_cfg_ptr;
    }

    static uint8_t get_status()
    {
        return 0;
    }

private:
    static uint8_t state_;
    enum{
        ST_WAITING_4_CMD,
        ST_TX,
        ST_END
    };

    static SpiSlaveTransfer<SpiSlave> transfer_;

    volatile static int16_t  left_real_speed_;
    volatile static int16_t  left_set_speed_;
    volatile static uint32_t left_counter_;
    volatile static int16_t  right_real_speed_;
    volatile static int16_t  right_set_speed_;
    volatile static uint32_t right_counter_;

    volatile static PidConfig left_pid_config_;
    volatile static PidConfig right_pid_config_;

    volatile static bool is_new_speed_;
    volatile static bool is_new_pid_config_;
    volatile static bool is_reset_counter_;
    volatile static bool is_interface_ready_;
};

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
uint8_t SpiFace<SpiSlave, Odometers, MotorsController>::state_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile SpiSlaveTransfer<SpiSlave>
SpiFace<SpiSlave, Odometers, MotorsController>::transfer_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile int16_t SpiFace<SpiSlave, Odometers, MotorsController>::left_real_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile int16_t SpiFace<SpiSlave, Odometers, MotorsController>::left_set_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile uint32_t SpiFace<SpiSlave, Odometers, MotorsController>::left_counter_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile int16_t SpiFace<SpiSlave, Odometers, MotorsController>::right_real_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile int16_t SpiFace<SpiSlave, Odometers, MotorsController>::right_set_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile uint32_t SpiFace<SpiSlave, Odometers, MotorsController>::right_counter_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile PidConfig SpiFace<SpiSlave, Odometers, MotorsController>::left_pid_config_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
volatile PidConfig SpiFace<SpiSlave, Odometers, MotorsController>::right_pid_config_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
bool SpiFace<SpiSlave, Odometers, MotorsController>::is_new_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
bool SpiFace<SpiSlave, Odometers, MotorsController>::is_new_pid_config_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
bool SpiFace<SpiSlave, Odometers, MotorsController>::is_reset_counter_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
bool SpiFace<SpiSlave, Odometers, MotorsController>::is_interface_ready_;





#endif
