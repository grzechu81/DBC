#include "oled_4bit.h"
#include "gpio.h"

void send_4bits(uint8_t data);

void oled_init()
{
	uint8_t i;

    ETS_GPIO_INTR_DISABLE();

    //Set pin functions
    PIN_FUNC_SELECT(PIN_D7, FUNC_GPIO15);
    PIN_FUNC_SELECT(PIN_D6, FUNC_GPIO13);
    PIN_FUNC_SELECT(PIN_D5, FUNC_GPIO12);
    PIN_FUNC_SELECT(PIN_D4, FUNC_GPIO14);
    PIN_FUNC_SELECT(PIN_E,  FUNC_GPIO0);
    PIN_FUNC_SELECT(PIN_RS, FUNC_GPIO2);

    //Turn interrupt back on
    ETS_GPIO_INTR_ENABLE();

    GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_RS_N), 0);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_E_N), 0);

    //Syncronization
    for(i=0; i<5; ++i)
    {
    	send_4bits(0x0);
    }

    //Enter 4Bit mode
    send_4bits(0x2); 
    send_4bits(0x2);
    send_4bits(0xb); 

    oled_cmd(0x2b); //function set
    oled_cmd(0xc); //display on
    oled_cmd(0x6); //entry mode
    oled_cmd(0x2); //return home
    oled_cmd(0x1); //clear display

    os_delay_us(30000);

}

void oled_cmd(uint8_t cmd)
{
	GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_RS_N), 0);
	send_4bits(cmd >> 4);
	send_4bits(cmd);

	os_delay_us(100);
}

void oled_data(uint8_t data)
{
	GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_RS_N), 1);
	send_4bits(data >> 4);
	send_4bits(data);

	os_delay_us(100);
}

void oled_move_xy(uint8_t row, uint8_t col)
{
	switch(row)
	{
		case 0:
			oled_cmd(0x80+col);
			break;
		case 1:
			oled_cmd(0xc0+col);
			break;
		case 2:
			oled_cmd(0x94+col);
			break;
		case 3:
			oled_cmd(0xd4+col);
			break;
	}
}

void oled_str(char* string)
{
	while(*string != 0x0)
	{
		oled_data(*string);
		string++;
	}
}

void oled_put_buffer(char buffer[4][20])
{
	uint8_t line, row;
	for(line = 0; line< 4; ++line)
	{
		for(row = 0; row < 20; ++row)
		{
			oled_data(buffer[line][row]);
		}
	}
}

void send_4bits(uint8_t data)
{
	GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_E_N), 1);

	if(data & 0x01)
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D4_N), 1);
	else
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D4_N), 0);

	if(data & 0x02)
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D5_N), 1);
	else
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D5_N), 0);

	if(data & 0x04)
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D6_N), 1);
	else
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D6_N), 0);

	if(data & 0x08)
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D7_N), 1);
	else
		GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_D7_N), 0);

	GPIO_OUTPUT_SET(GPIO_ID_PIN(PIN_E_N), 0);

	return;
}