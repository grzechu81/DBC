#include "button.h"

static volatile os_timer_t buttonTimer;
uint8_t timerStarted;
uint32_t buttonCounter;

void _button_timer_elapsed(void *arg);
void _btn_int_handler(int* arg);

void button_init()
{
    timerStarted = 0;
    buttonCounter = 0;

    PIN_FUNC_SELECT(BUTTON_PIN, FUNC_GPIO5);

    //PIN_PULLUP_EN(BUTTON_PIN);
    GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));

    ETS_GPIO_INTR_DISABLE();

    ETS_GPIO_INTR_ATTACH(_btn_int_handler, 0);
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(5));
    gpio_pin_intr_state_set(GPIO_ID_PIN(5), GPIO_PIN_INTR_POSEDGE);

    os_timer_disarm(&buttonTimer);
    os_timer_setfn(&buttonTimer, (os_timer_func_t *)_button_timer_elapsed, NULL);

    ETS_GPIO_INTR_ENABLE();
}

uint8_t button_is_processed()
{
    return timerStarted;
}

void button_pressed()
{
    if(timerStarted == 0)
    {
        os_timer_arm(&buttonTimer, BUTTON_PROBING_TIMOUT, 1);
        timerStarted = 1;
    }

    os_printf("p");  

    buttonCounter++;
}

void button_released()
{
    os_timer_disarm(&buttonTimer);

    if(buttonCounter > BUTTON_LONG_PRESS_THRESHOLD)
    {
        system_os_post(0, SIG_BUTTON_LONG_PRESS, 0 );
    }
    else
    {
        system_os_post(0, SIG_BUTTON_SHORT_PRESS, 0 );
    }

     os_printf("r");

    buttonCounter = 0;
    timerStarted = 0;
}

void _button_timer_elapsed(void *arg)
{
    uint32 inputs = gpio_input_get();

    uint8_t buttonState = (inputs >> BUTTON_PIN_N) & 0x1;

    if(buttonState == 1)
    {
        if(buttonCounter > BUTTON_LONG_PRESS_THRESHOLD)
        {
            button_released();
            os_timer_disarm(&buttonTimer);
        }
        else
        {
            button_pressed();       
        }
        
    }
    else
    {
        button_released();
        os_timer_disarm(&buttonTimer);
    }
}

void _btn_int_handler(int* arg)
{   
    ETS_GPIO_INTR_DISABLE();

    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    os_printf("i");    

    if(!button_is_processed())
    {
        button_pressed();
    }

    //clear interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
    ETS_GPIO_INTR_ENABLE();      
}