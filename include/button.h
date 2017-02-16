#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "os_type.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

#define BUTTON_PROBING_TIMOUT 20
#define BUTTON_LONG_PRESS_THRESHOLD 30
#define BUTTON_PIN PERIPHS_IO_MUX_GPIO5_U
#define BUTTON_PIN_N 5

#define SIG_BUTTON_SHORT_PRESS 0
#define SIG_BUTTON_LONG_PRESS  1

void button_init();
uint8_t button_is_processed();
void button_pushed();
void button_released();

#endif