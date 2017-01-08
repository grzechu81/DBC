#include "encoder.h"
#include "gpio.h"

extern void gpio_intr_handler(int * dummy);

const uint8 ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

void encoder_init()
{
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
  PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);

  PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO4_U);
  PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO5_U);

  GPIO_DIS_OUTPUT(GPIO_ID_PIN(4));
  GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));

  // Disable interrupts by GPIO
  ETS_GPIO_INTR_DISABLE();
  ETS_GPIO_INTR_ATTACH(gpio_intr_handler, 0);

  // Clear gpio status. 
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(4));
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(5));

  // Enable interrupt for his GPIO
  gpio_pin_intr_state_set(GPIO_ID_PIN(4), GPIO_PIN_INTR_ANYEDGE);
  gpio_pin_intr_state_set(GPIO_ID_PIN(5), GPIO_PIN_INTR_ANYEDGE);

  ETS_GPIO_INTR_ENABLE();

	state = R_START;
}

uint8 encoder_process(uint8 p1, uint8 p2)
{
 // Grab state of input pins.
  uint8 pinstate = (p2 << 1) | p1;
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return state & 0x30;
}