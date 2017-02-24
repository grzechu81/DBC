#ifndef _USER_DISPLAY_LOGIC_H_
#define _USER_DISPLAY_LOGIC_H_

#include "os_type.h"

// 1) Status - widzaiany caly czas
// H - hotel, S - start

// --------------------
//    Battery status
// H:#######  S:#######
// V: 12.5V   V: 13.1V   
// I: -10.5A
// --------------------    

// 2) Poziom paliwa i wody

// --------------------
//        Tanks
// F: ############  90%  
// W: #######       50%

// --------------------

// 3) Godziny silnika

// --------------------
//      Engine Hrs
// TOTAL: 1859  Hrs
// TRIP:  005.3 Hrs

// --------------------

#define MIN_PAGE 0
#define MAX_PAGE 2

#define STATE_DISPLAY_ON  0
#define STATE_DISPLAY_OFF 1

#define SIG_DISPLAY_OFF 3

void display_init();
void display_show();
void display_on();
void display_off();
void display_refresh_page();
void display_welcome_message();

#endif		