#ifndef MOTORS_CONTROLLER_TWI_FACE_H
#define MOTORS_CONTROLLER_TWI_FACE_H

#define MCTF_DEVICE_ADDRESS (2)

#define MCTF_REG_LEFT_P      	(0x0)
#define MCTF_REG_LEFT_I      	(0x1)
#define MCTF_REG_LEFT_I_MAX  	(0x2)
#define MCTF_REG_LEFT_I_MIN  	(0x3)
#define MCTF_REG_LEFT_D      	(0x4)
#define MCTF_REG_RIGHT_P     	(0x5)
#define MCTF_REG_RIGHT_I     	(0x6)
#define MCTF_REG_RIGHT_I_MAX 	(0x7)
#define MCTF_REG_RIGHT_I_MIN 	(0x8)
#define MCTF_REG_RIGHT_D     	(0x9)
#define MCTF_REG_LEFT_SPEED		(0xA)
#define MCTF_REG_LEFT_COUNTER	(0xB)
#define MCTF_REG_RIGHT_SPEED 	(0xC)
#define MCTF_REG_RIGHT_COUNTER	(0xD)
#define MCTF_REG_UNDEFINED		(0xE)

#define MCTF_NUM_REGS 			(MCTF_REG_UNDEFINED)


#endif
