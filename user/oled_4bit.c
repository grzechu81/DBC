#include "oled_4bit.h"
#include "gpio.h"

void send_4bits(uint8_t data);
void init_custom_chars();
void wait();

uint32_t regComp1, regComp2;
uint8_t customCharacters[8][8] = {
    {0x04, 0x04, 0x04, 0x00, 0x0A, 0x1F, 0x1F, 0x1F}, //Bat 1
    {0x0A, 0x0A, 0x0A, 0x00, 0x0A, 0x1F, 0x1F, 0x1F}, //Bat 2
    {0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00}, //Heart
    {0x00, 0x0E, 0x15, 0x17, 0x11, 0x0E, 0x00, 0x00}  //Clock
};

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
    PIN_FUNC_SELECT(PIN_RW, FUNC_GPIO4);

    //Turn interrupt back on
    ETS_GPIO_INTR_ENABLE();

    //Set pins as outputs
    GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, CONTROL_PINS_MASK);
    GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, DATA_PINS_MASK);

    //Put all control lines low
    GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, CONTROL_PINS_MASK);

    os_delay_us(5000);

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
    oled_cmd(0x8); //display off
    oled_cmd(0x6); //entry mode
    oled_cmd(0x17); //character mode
    oled_cmd(0x1); //clear display
    oled_cmd(0xc); //display on

    os_delay_us(30000);

    //init_custom_chars();
}

void oled_cmd(uint8_t cmd)
{
    regComp1 = GPIO_REG_READ(GPIO_ENABLE_ADDRESS);
    wait();
    regComp2 = GPIO_REG_READ(GPIO_ENABLE_ADDRESS);

    if(regComp1 != regComp2)
    {
        os_printf("oled_cmd GPIO_ENABLE_ADDRESS differs\n");
    }

    RW_LOW;
    RS_LOW;
	
    send_4bits(cmd >> 4);
	send_4bits(cmd);
}

void oled_data(uint8_t data)
{
    regComp1 = GPIO_REG_READ(GPIO_ENABLE_ADDRESS);
    wait();
    regComp2 = GPIO_REG_READ(GPIO_ENABLE_ADDRESS);

    if(regComp1 != regComp2)
    {
        os_printf("oled_data GPIO_ENABLE_ADDRESS differs\n");
    }

    RW_LOW;
    RS_HIGH;

	send_4bits(data >> 4);
	send_4bits(data);
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

void oled_put_buffer(unsigned char *buffer, uint8_t length)
{
	uint8_t i;

	for(i = 0; i< length; ++i)
	{
		oled_data(buffer[i]);
	}
}

void send_4bits(uint8_t data)
{
    uint32_t mask = 0;

    if(data & 0x01)
        mask |= PIN_D4_MASK;

    if(data & 0x02)
        mask |= PIN_D5_MASK;

    if(data & 0x04)
        mask |= PIN_D6_MASK;

    if(data & 0x08)
        mask |= PIN_D7_MASK;

    GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, DATA_PINS_MASK);
    GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, mask);

    E_HIGH;
    os_delay_us(1);
    E_LOW;
    return;
}

void wait()
{
    uint8_t busyFlag = 0;

    RW_HIGH;
    RS_LOW;

    //Set data pins as inputs
    GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, DATA_PINS_MASK);

    do
    {
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, PIN_E_MASK);
        os_delay_us(1);
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, PIN_E_MASK);

        //EXPERIMENTAL: busyFlag = (GPIO_REG_READ(GPIO_IN_ADDRESS) >> PIN_D7_N) & 0x1;
        busyFlag = (gpio_input_get() >> PIN_D7_N) & 0x1;

        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, PIN_E_MASK);
        os_delay_us(1);
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, PIN_E_MASK);

        if(busyFlag)
        {
            os_printf('!');
        }
            
    }
    while(busyFlag);

    //Set data pins as outputs
    GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, DATA_PINS_MASK);
}

void init_custom_chars()
{
    uint8_t i,j;
    oled_cmd(0x40);
    for(i=0; i<4;++i)
    {
        for(j=0;j<8;++j)
        {
            oled_data(customCharacters[i][j]);    
        }
    }
}

