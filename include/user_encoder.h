#ifndef __USER_ENCODER_H
#define __USER_ENCODER_H

#include "os_type.h"

#define DIR_CW 0x10
#define DIR_CCW 0x20
#define DIR_UNDEFINED 0x00

//States
#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

uint8 state;

void user_encoder_init();
uint8 user_encoder_process(uint8 p1, uint8 p2);



#endif