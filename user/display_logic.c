#include "display_logic.h"
#include "oled_4bit.h"
#include "osapi.h"

volatile uint8_t currentPage;
uint8_t currentState;
unsigned char oledBuffer[4][20];
static volatile os_timer_t displayOnTimer;

struct battery_status
{
	char* bat1Voltage[5]; //99,9\0
	char* bat2Voltage[5]; //99,9\0
	char* bat2Current[6]; //+99.9\0
} batteryStatus;

struct tank_status
{
	char* level[4]; //100\0
} fuelTankStatus, waterTankStatus;

struct engine_hours_status
{
	uint32_t totalHours;
	uint32_t tripHours;
} engineHoursStatus;

void _show_page();
void _clear_buffer();
void _show_buffer();
void _next_page();
void _display_battery_status();
void _display_tanks_status();
void _display_engine_hours();
void _startTimer();
void _display_timer_elapsed(void *arg);

void display_init()
{
	oled_init();
	currentPage = 0;
	currentState = STATE_DISPLAY_OFF;

	os_timer_disarm(&displayOnTimer);
    os_timer_setfn(&displayOnTimer, (os_timer_func_t *)_display_timer_elapsed, NULL);

	// os_strcpy(batteryStatus.bat1Voltage, "12.3");
	// os_strcpy(batteryStatus.bat2Voltage, "12.1");
	// os_strcpy(batteryStatus.bat2Current, "-01.5");

	// os_strcpy(fuelTankStatus.level, "98");
	// os_strcpy(waterTankStatus.level, "57");

	// engineHoursStatus.totalHours = 1832;
	// engineHoursStatus.tripHours = 21;

}

void display_show()
{
	switch(currentState)
	{
		case STATE_DISPLAY_ON:
			_startTimer();
			_next_page();
			break;
		case STATE_DISPLAY_OFF:
			_startTimer();
			display_on();
			_show_page();
			break;
	}
}

void display_off()
{
	oled_cmd(CMD_DISPLAY_OFF);
	currentState = STATE_DISPLAY_OFF;
	currentPage = 0;
}

void display_on()
{
	oled_cmd(CMD_DISPLAY_ON);
	currentState = STATE_DISPLAY_ON;
}

void display_refresh_page()
{
	_show_page();
}

void display_welcome_message()
{
	oled_move_xy(1, 2);
	oled_str("-= Dehler 34 =-");
}

void _startTimer()
{
	os_timer_disarm(&displayOnTimer);
	os_timer_arm(&displayOnTimer, 30000, 0);
}

void _next_page()
{
	if(currentPage < MAX_PAGE)
		currentPage++;
	else
		currentPage = MIN_PAGE;

	_show_page();
}

void _show_page()
{
	switch(currentPage)
	{
		case 0:
			_display_battery_status();
			break;
		case 1:
			_display_tanks_status();
			break;
		case 2:
			_display_engine_hours();
			break;
	}
}

void _display_battery_status()
{
	//_clear_buffer();
	
	os_memcpy(oledBuffer[0], "\x03 Power              ", 20);
	os_memcpy(oledBuffer[1], "Bat1: 12.1V -1.1A   ", 20);
	os_memcpy(oledBuffer[2], "Bat2: 13.4V         ", 20);
	os_memset(oledBuffer[3], 0x20, 20);

	_show_buffer();
}

void _display_tanks_status()
{
    os_memcpy(oledBuffer[0], "\x01 Water & Fuel       ", 20);
	os_memcpy(oledBuffer[1], " Fuel:  88%         ", 20);
	os_memcpy(oledBuffer[2], "Water:  60%         ", 20);
	os_memset(oledBuffer[3], 0x20, 20);

	_show_buffer();
}

void _display_engine_hours()  
{
	os_memcpy(oledBuffer[0], "\x00 Engine hours       ", 20);
	os_memcpy(oledBuffer[1], "TOTAL: 1845 Hrs     ", 20);
	os_memcpy(oledBuffer[2], " TRIP: 23   Hrs     ", 20);
	os_memset(oledBuffer[3], 0x20, 20);

	_show_buffer();
}

void _clear_buffer()
{
	uint8_t i;
	for(i=0; i<4; ++i)
	{
		os_memset(oledBuffer[i], 0x20, 20);
	}
}

void _show_buffer()
{
	uint8_t i;

	oled_sync();
	
	for(i=0; i<4; ++i)
	{
		oled_move_xy(i, 0);
		oled_put_buffer(oledBuffer[i], 20);
	}


}

void _display_timer_elapsed(void *arg)
{
	system_os_post(0, SIG_DISPLAY_OFF, 0 );
}