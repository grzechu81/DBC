#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "encoder.h"
#include "display_logic.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
#define IS_BUSY read_busy_flag()

os_event_t user_procTaskQueue[user_procTaskQueueLen];
static volatile os_timer_t testTimer;
static void loop(os_event_t *events);

// variable modified indirectly by interrupt handler
volatile uint8 direction, lastDirection;

// gpio interrupt handler. See below
void  gpio_intr_handler(int * dummy);

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

            if(direction == DIR_CW)
                display_next_page();
            else
                display_prev_page();
        }
        
        lastDirection = direction;
   }
   
   // turn again
   system_os_post(user_procTaskPrio, 0, 0 );
}

void timerElapsed(void *arg)
{
   
}

void ICACHE_FLASH_ATTR  user_init()
{
    uart_div_modify(0, UART_CLK_FREQ / 9600);

    direction = DIR_UNDEFINED;
    lastDirection = DIR_UNDEFINED;

    os_printf("Initializing...\n");

    //Initialize the GPIO subsystem.
    gpio_init();
    encoder_init();
    display_init();

    display_welcome_message();

    // os_timer_disarm(&testTimer);
    // os_timer_setfn(&testTimer, (os_timer_func_t *)timerElapsed, NULL);
    // os_timer_arm(&testTimer, 1000, 1);
    // wajcha = 0;

    os_printf("Done !!!\n");  
    
    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );
}


void  gpio_intr_handler(int * arg)
{   
    ETS_GPIO_INTR_DISABLE();

    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    os_delay_us(500);

    uint32 inputs = gpio_input_get();

    uint8 p1 = (inputs >> 4) & BIT0;
    uint8 p2 = (inputs >> 5) & BIT0;

    direction = encoder_process(p1, p2);

    //clear interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

    ETS_GPIO_INTR_ENABLE();   
}


