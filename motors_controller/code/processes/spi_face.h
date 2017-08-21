#ifndef SPI_FACE_H
#define SPI_FACE_H

#include <stdint.h>

#include "utils/atomic.h"
#include "utils/pid.h"
#include "utils/flag.h"

#include "processes/spi_slave_interface.h"
#include "processes/spi_face_defs.h"

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
        left_set_speed_  = 0;
        right_set_speed_ = 0;

		// pid configs (to client)
        PidConfig* left_pid_cfg_ptr;
        PidConfig* right_pid_cfg_ptr;

        MotorsController::get_pid_configs(
                &left_pid_cfg_ptr, &right_pid_cfg_ptr);

        left_pid_config_  = *left_pid_cfg_ptr;
        right_pid_config_ = *right_pid_cfg_ptr;

        // write_values();
        left_real_speed_	= 150;
        right_real_speed_	= 151;

        left_counter_  = 85;
        right_counter_ = 86;


		Interface::init(get_status());
		Interface::set_register(REG_LEFT_SPEED, &left_real_speed_,
                &left_set_speed_, 2, &flag_new_set_speed_);
		Interface::set_register(REG_LEFT_COUNTER, &left_counter_,
                0, 4, 0);
		Interface::set_register(REG_RIGHT_SPEED, &right_real_speed_,
                &right_set_speed_, 2, &flag_new_set_speed_);
		Interface::set_register(REG_RIGHT_COUNTER, &right_counter_,
                0, 4, 0);
		Interface::set_register(REG_LEFT_P, &(left_pid_config_.p_gain_x100),
                &(left_pid_config_.p_gain_x100), 2, &flag_new_pid_config_);
		Interface::set_register(REG_LEFT_I,&(left_pid_config_.i_gain_x100),
                &(left_pid_config_.i_gain_x100), 2, &flag_new_pid_config_);
		Interface::set_register(REG_LEFT_I_MAX, &(left_pid_config_.i_max),
                &(left_pid_config_.i_max), 2, &flag_new_pid_config_);
		Interface::set_register(REG_LEFT_I_MIN, &(left_pid_config_.i_min),
                &(left_pid_config_.i_min), 2, &flag_new_pid_config_);
		Interface::set_register(REG_RIGHT_P, &(right_pid_config_.p_gain_x100),
                &(right_pid_config_.p_gain_x100), 2, &flag_new_pid_config_);
		Interface::set_register(REG_RIGHT_I, &(right_pid_config_.i_gain_x100),
                &(right_pid_config_.i_gain_x100), 2, &flag_new_pid_config_);
		Interface::set_register(REG_RIGHT_I_MAX, &(right_pid_config_.i_max),
                &(right_pid_config_.i_max), 2, &flag_new_pid_config_);
		Interface::set_register(REG_RIGHT_I_MIN, &(right_pid_config_.i_min),
                &(right_pid_config_.i_min), 2, &flag_new_pid_config_);
        }

    static inline void update()
    {
        Atomic atomic;

		// to divice
        // set speed
        if(flag_new_set_speed_.check()){
            MotorsController::set_input_rps(left_set_speed_, right_set_speed_);
        }

        // set pid config
        if(flag_new_pid_config_.is_set()){
            PidConfig* left_pid_cfg_ptr;
            PidConfig* right_pid_cfg_ptr;

            MotorsController::get_pid_configs(
                    &left_pid_cfg_ptr, &right_pid_cfg_ptr);

            *left_pid_cfg_ptr   = left_pid_config_;
            *right_pid_cfg_ptr  = right_pid_config_;
        }

		// to client
        // write_values();

		Interface::set_status(get_status());

        atomic.release();

         // save pid configs
        if(flag_new_pid_config_.check()){
            MotorsController::save_configs();
        }
    }

private:
	typedef SpiSlaveInterface<SpiSlave, NUM_REGS> Interface;

	// to client
    static void write_values()
    {
        // real speed
		if(	Interface::is_available(REG_LEFT_SPEED) &&
			Interface::is_available(REG_RIGHT_SPEED))
		{
			int16_t left, right;
			MotorsController::real_rps(&left, &right);
			left_real_speed_	= left;
			right_real_speed_	= right;
		}

        // counter
		if(Interface::is_available(REG_LEFT_COUNTER)){
			left_counter_  = Odometers::left_counter();
		}
		if(Interface::is_available(REG_RIGHT_COUNTER)){
			right_counter_ = Odometers::right_counter();
		}
    }

    static uint8_t get_status()
    {
        return 42;
    }

private:
	static Flag	 flag_new_set_speed_;
	static Flag	 flag_new_pid_config_;

    static int16_t  left_real_speed_;
    static int16_t  left_set_speed_;
    static uint32_t left_counter_;
    static int16_t  right_real_speed_;
    static int16_t  right_set_speed_;
    static uint32_t right_counter_;

    static PidConfig left_pid_config_;
    static PidConfig right_pid_config_;
};

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
Flag SpiFace<SpiSlave, Odometers, MotorsController>::flag_new_set_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
Flag SpiFace<SpiSlave, Odometers, MotorsController>::flag_new_pid_config_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
int16_t SpiFace<SpiSlave, Odometers, MotorsController>::left_real_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
int16_t SpiFace<SpiSlave, Odometers, MotorsController>::left_set_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
uint32_t SpiFace<SpiSlave, Odometers, MotorsController>::left_counter_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
int16_t SpiFace<SpiSlave, Odometers, MotorsController>::right_real_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
int16_t SpiFace<SpiSlave, Odometers, MotorsController>::right_set_speed_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
uint32_t SpiFace<SpiSlave, Odometers, MotorsController>::right_counter_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
PidConfig SpiFace<SpiSlave, Odometers, MotorsController>::left_pid_config_;

template<
    typename SpiSlave,
    typename Odometers,
    typename MotorsController
    >
PidConfig SpiFace<SpiSlave, Odometers, MotorsController>::right_pid_config_;

#endif
