#include "display_logic.h"
#include "oled_4bit.h"

volatile uint8_t current_page;

struct battery_status
{
	uint8_t voltage;
	uint8_t current;
	char currentSign;
};

struct tank_status
{
	uint8_t level;
};

struct engine_hours_status
{
	uint32_t total_hours;
	uint32_t trip_hours;
};

void _show_page();
void _clear_display();
void _display_battery_status();
void _display_tanks_status();
void _display_engine_hours();

void display_init()
{
	oled_init();
	current_page = 0;
}

void display_next_page()
{
	if(current_page < MAX_PAGE)
		current_page++;
	else
		current_page = MIN_PAGE;

	_show_page();
}

void display_prev_page()
{
	if(current_page > MIN_PAGE)
		current_page--;
	else
		current_page = MAX_PAGE;

	_show_page();
}

void display_refresh_page()
{

}

void display_welcome_message()
{
	oled_move_xy(1, 2);
	oled_str("-= Dehler 34 =-");
}

void _show_page()
{
	_clear_display();

	switch(current_page)
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
	oled_cmd(0x01);
	os_delay_us(6000);
}

void _display_battery_status()
{
	oled_move_xy(0, 0);
	oled_str("Battery status");
}

void _display_tanks_status()
{
	oled_move_xy(0, 0);
	oled_str("Tanks status");
}

void _display_engine_hours()
{
	oled_move_xy(0, 0);
	oled_str("Engine hours");
}
