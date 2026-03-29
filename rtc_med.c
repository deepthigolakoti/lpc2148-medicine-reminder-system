#include <lpc214x.h>
#include "rtc_med.h"
#include "lcd_med.h"
#include "lcd_defines_med.h"
#include "kpm_med.h"
#include "delay_med.h"
/* weekday names for display */
s8 week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
// initialize RTC module
// reset RTC and configure prescaler
void RTC_Init(void)
{
    CCR = RTC_RESET;      // reset RTC counters
#ifndef _LPC2148
    PREINT = PREINT_VAL;  // set integer part of prescaler
    PREFRAC = PREFRAC_VAL;// set fractional part of prescaler
    CCR = RTC_ENABLE;     // enable RTC
#else
    CCR = RTC_ENABLE | RTC_CLKSRC;   // enable RTC with external clock
#endif
}
// read current time from RTC registers
void GetRTCTimeInfo(s32 *h,s32 *m,s32 *s)
{
    *h = HOUR;   // read hour register
    *m = MIN;    // read minute register
    *s = SEC;    // read second register
}
// display time on LCD in HH:MM:SS format
void DisplayRTCTime(u32 h,u32 m,u32 s)
{
    Cmd_LCD(GOTO_LINE1_POS0);   // move cursor to first line
    Char_LCD(h/10 + 48);   // display hour tens
    Char_LCD(h%10 + 48);   // display hour units
    Char_LCD(':');
    Char_LCD(m/10 + 48);   // display minute tens
    Char_LCD(m%10 + 48);   // display minute units
    Char_LCD(':');
    Char_LCD(s/10 + 48);   // display second tens
    Char_LCD(s%10 + 48);   // display second units
}
// write time values into RTC registers
void SetRTCTimeInfo(u32 h,u32 m,u32 s)
{
    HOUR = h;   // update hour register
    MIN  = m;   // update minute register
    SEC  = s;   // update second register
}
// read date from RTC registers
void GetRTCDateInfo(s32 *d,s32 *m,s32 *y)
{
    *d = DOM;       // read day of month
    *m = MONTH;     // read month
    *y = YEAR;      // read year
}
// display date on LCD
void DisplayRTCDate(u32 d,u32 m,u32 y)
{
    Cmd_LCD(GOTO_LINE2_POS0);   // move cursor to second line
    Char_LCD(d/10 + 48);
    Char_LCD(d%10 + 48);
    Char_LCD('/');
    Char_LCD(m/10 + 48);
    Char_LCD(m%10 + 48);
    Char_LCD('/');
    // display year digit by digit
    Char_LCD((y/1000)%10 + 48);
    Char_LCD((y/100)%10 + 48);
    Char_LCD((y/10)%10 + 48);
    Char_LCD((y%10) + 48);
    Char_LCD(' ');
}
// write date values to RTC registers
void SetRTCDateInfo(u32 d,u32 m,u32 y)
{
    DOM   = d;      // set day of month
    MONTH = m;      // set month
    YEAR  = y;      // set year
}
// read weekday register
void GetRTCDay(s32 *dow)
{
    *dow = DOW;   // read day of week register
}
// display weekday next to time
void DisplayRTCDay(u32 dow)
{
    Cmd_LCD(GOTO_LINE1_POS0 + 8);   // position after time
    Char_LCD(' ');
    Str_LCD(week[dow]);             // print weekday string
}
// write weekday value to RTC
void SetRTCDay(u32 dow)
{
    DOW = dow;   // update day of week register
}
// update RTC time using keypad input
void RTC_UpdateTime(void)
{
    int val;
    /* hour input */
    do{
        Cmd_LCD(CLEAR_LCD);     // clear display
        Str_LCD("HOUR(0-23):");
        val = ReadNum();        // read number from keypad
        if(val == -2)           // exit to menu
            return;
        if(val == -3)           // skip hour entry
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");   // no digits entered
            delay_ms(500);
        }
        else if(val < 0 || val > 23)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid hour
            delay_ms(500);
        }
                                else
        {
            hour = val;         // store valid hour
            break;
        }
    }while(1);
    /* minute input */
    do{
        Cmd_LCD(CLEAR_LCD);
        Str_LCD("MIN(0-59):");
        val = ReadNum();
        if(val == -2)
            return;
        if(val == -3)
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
			 Str_LCD("ENTER INPUT");
            delay_ms(500);
        }
        else if(val < 0 || val > 59)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid minute
            delay_ms(500);
        }
        else
        {
            min = val;          // store minute
            break;
        }
    }while(1);
    /* second input */
    do{
        Cmd_LCD(CLEAR_LCD);
        Str_LCD("SEC(0-59):");
        val = ReadNum();
        if(val == -2)
            return;
        if(val == -3)
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");
            delay_ms(500);
        }
        else if(val < 0 || val > 59)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid second
            delay_ms(500);
        }
        else
        {
            sec = val;          // store second
            break;
        }
    }while(1);
    SetRTCTimeInfo(hour,min,sec);  // update RTC registers
}
// update RTC date using keypad
void RTC_UpdateDate(void)
{
    int val;
    /* date input */
    do{
        Cmd_LCD(CLEAR_LCD);
        Str_LCD("DATE(1-31):");
        val = ReadNum();
        if(val == -2)
            return;
        if(val == -3)
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");
            delay_ms(500);
        }
        else if(val < 1 || val > 31)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid date
            delay_ms(500);
        }
        else
        {
            date = val;         // store date
            break;
        }
    }while(1);
    /* month input */
    do{
        Cmd_LCD(CLEAR_LCD);
        Str_LCD("MONTH(1-12):");
        val = ReadNum();
        if(val == -2)
            return;
        if(val == -3)
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");
            delay_ms(500);
			 }
        else if(val < 1 || val > 12)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid month
            delay_ms(500);
        }
        else
        {
            month = val;        // store month
            break;
        }
    }while(1);
    /* year input */
    do{
        Cmd_LCD(CLEAR_LCD);
        Str_LCD("YEAR:");
        val = ReadNum();
        if(val == -2)
            return;
        if(val == -3)
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");
            delay_ms(500);
        }
        else if(val < 2000 || val > 2099)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid year
            delay_ms(500);
        }
        else
        {
            year = val;         // store year
            break;
        }
    }while(1);
    SetRTCDateInfo(date,month,year);  // update RTC date
}
// update weekday using keypad
void RTC_UpdateDay(void)
{
    int val;
    do{
	
        Cmd_LCD(CLEAR_LCD);
        Str_LCD("DAY(0-6): S0 M1");
        Cmd_LCD(GOTO_LINE2_POS0);
        Str_LCD("T2 W3 T4 F5 S6");
        val = ReadNum();
        if(val == -2)
            return;
        if(val == -3)
            break;
        if(val == -1)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("ENTER INPUT");
            delay_ms(500);
        }
        else if(val < 0 || val > 6)
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT"); // invalid weekday
            delay_ms(500);
        }
                                else
        {
            day = val;   // store weekday
            break;
        }
    }while(1);
    SetRTCDay(day);   // update RTC weekday register
}
