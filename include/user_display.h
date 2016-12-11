#ifndef __USER_DISPLAY_H
#define __USER_DISPLAY_H

#include "os_type.h"
#include "driver/spi_register.h"
#include "ets_sys.h"
#include "osapi.h"

#define CMD_CLEAR        0b00000001
#define CMD_HOME         0b00000010
#define CMD_ENTRY_MODE   0b00000110
#define CMD_DISPLAY_ON   0b00001111
#define CMD_DISPLAY_OFF  0b00001000
#define CMD_FUNCTION_SET 0b00111011

#define SPI_CLK_PREDIV 8 // 80 /(7+1) / (9+1) = 1Mhz
#define SPI_CLK_CNTDIV 10

void user_display_init();
void user_display_on();
void user_display_off();
void user_display_clear();
void user_display_test();


#endif