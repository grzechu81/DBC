#ifndef _USER_DISPLAY_LOGIC_H_
#define _USER_DISPLAY_LOGIC_H_

#include "os_type.h"

// 1) Status - widzaiany caly czas
// H - hotel, S - start

// --------------------
//    10:22 08/05/16
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

void display_init();
void display_next_page();
void display_prev_page();
void display_refresh_page();
void display_welcome_message();

#endif		