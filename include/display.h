// #ifndef __USER_DISPLAY_H
// #define __USER_DISPLAY_H

// #include "os_type.h"
// #include "driver/spi_register.h"
// #include "ets_sys.h"
// #include "osapi.h"

// #define CMD_CLEAR        0b00000001
// #define CMD_HOME         0b00000010
// #define CMD_ENTRY_MODE   0b00000110
// #define CMD_DISPLAY_ON   0b00001111
// #define CMD_DISPLAY_OFF  0b00001000
// #define CMD_FUNCTION_SET 0b00111011

// #define SPI_CLK_PREDIV 8 // 80 /(7+1) / (9+1) = 1Mhz
// #define SPI_CLK_CNTDIV 10

// //Define all SPI Pins
// #define SPI_OUT     PERIPHS_IO_MUX_MTMS_U     	/* Define SDA signal to be pin 14 */
// #define SPI_OUT_N   14						 	/* Define SDA signal to be pin 14 */
// #define SPI_CS      PERIPHS_IO_MUX_MTCK_U     	/* Define SPI CS signal to be pin 13 */
// #define SPI_CS_N    13		     				/* Define SPI CS signal to be pin 13 */
// #define SPI_IN      PERIPHS_IO_MUX_GPIO4_U     	/* Define SDO signal to be pin 5 (ESP-12 swaps pins) */
// #define SPI_IN_N    4		     				/* Define SDO signal to be pin 5 (ESP-12 swaps pins) */
// #define SPI_CLK     PERIPHS_IO_MUX_MTDI_U     	/* Define SCL signal to be pin 12 */
// #define SPI_CLK_N   12					     	/* Define SCL signal to be pin 12 */

// void user_display_init();
// void user_display_on();
// void user_display_off();
// void user_display_clear();
// void user_display_test1();
// void user_display_test2();
// uint8 read_busy_flag();


// #endif