#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "user_encoder.h"
#include "user_display.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static volatile os_timer_t testTimer;
static void loop(os_event_t *events);

// variable modified indirectly by interrupt handler
volatile uint8 direction, lastDirection, wajcha;

// gpio interrupt handler. See below
LOCAL void  gpio_intr_handler(int * dummy);

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}

static void ICACHE_FLASH_ATTR  loop(os_event_t *events)
{
   if(direction != lastDirection)
   {
        if(direction != DIR_UNDEFINED)
        {
            os_printf("%s \r\n", direction == DIR_CW ? "CW" : "CCW");   
        }
        
        lastDirection = direction;
   }
   
   // turn again
   system_os_post(user_procTaskPrio, 0, 0 );
}

void timerElapsed(void *arg)
{
    if(wajcha)
    {
        //user_display_on();
        user_display_test();

        os_printf("display test\n");
    }
    else
    {
        user_display_clear();
        //user_display_off();
        

        os_printf("display off\n");
    }

    wajcha = !wajcha;
}

void encoder_pins_init()
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
}


void ICACHE_FLASH_ATTR  user_init()
{
    uart_div_modify(0, UART_CLK_FREQ / 9600);

    direction = DIR_UNDEFINED;
    lastDirection = DIR_UNDEFINED;

    os_printf("Initializing...");

    //Initialize the GPIO subsystem.
    gpio_init();
    encoder_pins_init();

    user_display_init();

    os_timer_disarm(&testTimer);
    os_timer_setfn(&testTimer, (os_timer_func_t *)timerElapsed, NULL);
    os_timer_arm(&testTimer, 1000, 1);
    wajcha = 0;

    os_printf("Done !!!\n");  
    
    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );
}


LOCAL void  gpio_intr_handler(int * arg)
{   
    ETS_GPIO_INTR_DISABLE();

    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    os_delay_us(500);

    uint32 inputs = gpio_input_get();

    uint8 p1 = (inputs >> 4) & BIT0;
    uint8 p2 = (inputs >> 5) & BIT0;

    direction = user_encoder_process(p1, p2);

    //clear interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

    ETS_GPIO_INTR_ENABLE();   
}



