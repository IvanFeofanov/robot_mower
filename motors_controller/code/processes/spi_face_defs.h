#ifndef SPI_FACE_DEFS_H
#define SPI_FACE_DEFS_H

#define CMD_NOP         (0x00)
#define CMD_WR_REG      (0b01000000)
#define CMD_R_REG       (0b10000000)

#define REG_MASK        (0b00111111)
#define CMD_MASK        (0b11000000)

#define REG_LEFT_P      	(0x0)
#define REG_LEFT_I      	(0x1)
#define REG_LEFT_I_MAX  	(0x2)
#define REG_LEFT_I_MIN  	(0x3)
#define REG_LEFT_D      	(0x4)
#define REG_RIGHT_P     	(0x5)
#define REG_RIGHT_I     	(0x6)
#define REG_RIGHT_I_MAX 	(0x7)
#define REG_RIGHT_I_MIN 	(0x8)
#define REG_RIGHT_D     	(0x9)
#define REG_LEFT_SPEED		(0xA)
#define REG_LEFT_COUNTER	(0xB)
#define REG_RIGHT_SPEED 	(0xC)
#define REG_RIGHT_COUNTER	(0xD)
#define REG_UNDEFINED		(0xE)

#define NUM_REGS 			(REG_UNDEFINED)

#endif
