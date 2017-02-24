#ifndef __USER_DISPLAY_4BIT_H__
#define __USER_DISPLAY_4BIT_H__ 

#include "os_type.h"
#include "ets_sys.h"
#include "osapi.h"

#define PIN_D7 PERIPHS_IO_MUX_MTDO_U //FUNC_GPIO15, D8
#define PIN_D7_N 15
#define PIN_D7_MASK 0x8000

#define PIN_D6 PERIPHS_IO_MUX_MTCK_U //FUNC_GPIO13, D7
#define PIN_D6_N 13
#define PIN_D6_MASK 0x2000

#define PIN_D5 PERIPHS_IO_MUX_MTDI_U //FUNC_GPIO12, D6
#define PIN_D5_N 12
#define PIN_D5_MASK 0x1000

#define PIN_D4 PERIPHS_IO_MUX_MTMS_U //FUNC_GPIO14, D5
#define PIN_D4_N 14
#define PIN_D4_MASK 0x4000

// #define PIN_E  PERIPHS_IO_MUX_GPIO0_U //FUNC_GPIO0, D3
// #define PIN_E_N 0
// #define PIN_E_MASK 0x1

#define PIN_E  PERIPHS_IO_MUX_GPIO4_U //FUNC_GPIO4, D2
#define PIN_E_N 4
#define PIN_E_MASK 0x10

#define PIN_RS PERIPHS_IO_MUX_GPIO2_U //FUNC_GPIO2, D4
#define PIN_RS_N 2
#define PIN_RS_MASK 0x4

// #define PIN_RW PERIPHS_IO_MUX_GPIO4_U //FUNC_GPIO4, D2
// #define PIN_RW_N 4
// #define PIN_RW_MASK 0x10

#define DATA_PINS_MASK    0xf000
#define CONTROL_PINS_MASK PIN_E_MASK | PIN_RS_MASK

#define GPIO_SET_AS_INPUT(mask) GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, mask)
#define GPIO_SET_AS_OUTPUT(mask) GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, mask)
#define GPIO_SET_HIGH(mask) GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, mask)
#define GPIO_SET_LOW(mask) GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, mask)

#define CMD_CLEAR        0b00000001
#define CMD_HOME         0b00000010
#define CMD_ENTRY_MODE   0b00000110
#define CMD_DISPLAY_ON   0b00001100
#define CMD_DISPLAY_OFF  0b00001000
#define CMD_FUNCTION_SET 0b00111011

void oled_init();
void oled_sync();
void oled_cmd(uint8_t cmd);
void oled_data(uint8_t data);
void oled_move_xy(uint8_t row, uint8_t col);
void oled_str(char* string);
void oled_put_buffer(unsigned char *buffer, uint8_t length);

#endif