#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"

#include "button.h"
#include "display_logic.h"


#define user_procTaskPrio        0
#define user_procTaskQueueLen    5
#define IS_BUSY read_busy_flag()

os_event_t user_procTaskQueue[user_procTaskQueueLen];
static volatile os_timer_t testTimer;
static void loop(os_event_t *events);
static struct espconn ptrespconn;

// gpio interrupt handler. See below
void gpio_intr_handler(int * dummy);
void ICACHE_FLASH_ATTR softap_config(void);
void ICACHE_FLASH_ATTR udp_listener_init(void);
void ICACHE_FLASH_ATTR button_init();


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

static void ICACHE_FLASH_ATTR  task_handler(os_event_t *e)
{
    switch(e->sig)
    {
        case SIG_BUTTON_SHORT_PRESS:
            os_printf(" - SIG_BUTTON_SHORT_PRESS\n");   
            display_next_page();   
            break;
        case SIG_BUTTON_LONG_PRESS:
            os_printf(" - SIG_BUTTON_LONG_PRESS\n");      
            break;
    }    
}

void timerElapsed(void *arg)
{

}



void ICACHE_FLASH_ATTR user_init()
{
    system_set_os_print(1);
    uart_div_modify(0, UART_CLK_FREQ / 9600);

    os_printf("Initializing...\n");
    
    //Initialize the GPIO subsystem
    gpio_init();
    display_init();
    button_init();

    display_welcome_message();

    //Turn on Soft AP mode
    wifi_set_opmode(SOFTAP_MODE);
    softap_config();

    //Initialize UDP listener
    udp_listener_init();

    // os_timer_disarm(&testTimer);
    // os_timer_setfn(&testTimer, (os_timer_func_t *)timerElapsed, NULL);
    // os_timer_arm(&testTimer, 100, 0);

    os_printf("Done !!!\n");  
    
    //Start os task
    system_os_task(task_handler, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    // system_os_post(user_procTaskPrio, 0, 0 );
}

void ICACHE_FLASH_ATTR udp_recv_handler(void *arg, char *pusrdata, unsigned short length)
{
    //espconn_sent(&ptrespconn, DeviceBuffer, length);
}

void ICACHE_FLASH_ATTR softap_config(void)
{
    struct softap_config config;

    wifi_softap_get_config(&config); // Get config first.

    os_memset(config.ssid, 0, 32);
    os_memset(config.password, 0, 64);
    os_memcpy(config.ssid, "8266", 4);
    os_memcpy(config.password, "Dehler34", 8);
    config.authmode = AUTH_WPA_WPA2_PSK;
    config.ssid_len = 0;// or its actual length
    config.max_connection = 4; // how many stations can connect to ESP8266 softAP at most.

    wifi_softap_set_config(&config);// Set ESP8266 softap config .
}

void ICACHE_FLASH_ATTR udp_listener_init(void)
{
    ptrespconn.type = ESPCONN_UDP;
    ptrespconn.proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
    ptrespconn.proto.udp->local_port = 1025;  // ESP8266 udp port
    espconn_regist_recvcb(&ptrespconn, udp_recv_handler); // register a udp packet receiving callback
    espconn_create(&ptrespconn);   // create udp
}



