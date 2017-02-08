#include "display_logic.h"
#include "oled_4bit.h"
#include "osapi.h"

volatile uint8_t currentPage;
unsigned char oledBuffer[4][20];

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
void _clear_display();
void _display_battery_status();
void _display_tanks_status();
void _display_engine_hours();

void display_init()
{
	oled_init();
	currentPage = 0;

	os_strcpy(batteryStatus.bat1Voltage, "12.3");
	os_strcpy(batteryStatus.bat2Voltage, "12.1");
	os_strcpy(batteryStatus.bat2Current, "-01.5");

	os_strcpy(fuelTankStatus.level, "98");
	os_strcpy(waterTankStatus.level, "57");

	engineHoursStatus.totalHours = 1832;
	engineHoursStatus.tripHours = 21;

}

void display_next_page()
{
	if(currentPage < MAX_PAGE)
		currentPage++;
	else
		currentPage = MIN_PAGE;

	_show_page();
}

void display_prev_page()
{
	if(currentPage > MIN_PAGE)
		currentPage--;
	else
		currentPage = MAX_PAGE;

	_show_page();
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

void _show_page()
{
	//os_printf("_show_page %d\n", currentPage);   
	//_clear_display();

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

void _clear_display()
{
	oled_cmd(0x02);
	// oled_cmd(0x01);
	// os_delay_us(30000);
}

void _display_battery_status()
{
	//_clear_buffer();
	
	os_memcpy(oledBuffer[0], "   Battery status   ", 20);
	os_memcpy(oledBuffer[1], "H:#######  S:#####  ", 20);
	os_memcpy(oledBuffer[2], "\x00: 12.1V   \x01:12.5V  ", 20);
	os_memcpy(oledBuffer[3], "I: -1.1A            ", 20);

	_show_buffer();
}

void _display_tanks_status()
{
	//_clear_buffer();

	os_memcpy(oledBuffer[0], "     Tank status    ", 20);
	os_memcpy(oledBuffer[1], "\x04: ############  88%", 20);
	os_memcpy(oledBuffer[2], "\x05: #########     60%", 20);
	os_memset(oledBuffer[3], 0x20, 20);

	_show_buffer();
}

void _display_engine_hours()
{
	//_clear_buffer();
	
	os_memcpy(oledBuffer[0], "     Engine hours   ", 20);
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
	for(i=0; i<4; ++i)
	{
		oled_move_xy(i, 0);
		oled_put_buffer(oledBuffer[i], 20);
	}
}