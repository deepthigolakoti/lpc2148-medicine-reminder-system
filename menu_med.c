#include "lcd_med.h"
#include "lcd_defines_med.h"
#include "kpm_med.h"
#include "rtc_med.h"
#include "medicine_med.h"
#include "delay_med.h"
// display menu and handle user selection
// exit if user selects exit or menu timeout occurs
void Menu_Setup(void)
{
    char key;                 // store pressed keypad key
    int start_sec,cur_sec;    // used for timeout calculation
    int diff;                 // difference between start and current second
    // read current time when menu opens
    GetRTCTimeInfo(&hour,&min,&sec);
    start_sec = sec;          // save starting second for timeout
    Cmd_LCD(CLEAR_LCD);       // clear lcd before showing menu
    while(1)                  // stay inside menu until exit or timeout
    {
        // display menu options
        Cmd_LCD(GOTO_LINE1_POS0);
        Str_LCD("1.TIME 2.DAY     ");
        Cmd_LCD(GOTO_LINE2_POS0);
        Str_LCD("3.MED 4.EXIT      ");
        // check if any key is pressed
        if(colscan()==0)
        {
            key = keyscan();      // read pressed key
            if(key=='1')
            {
                RTC_UpdateTime(); // open time setting screen
                // reset timeout after returning from submenu
                GetRTCTimeInfo(&hour,&min,&sec);
                start_sec = sec;
                continue;
            }
            else if(key=='2')
            {
                RTC_UpdateDate(); // open date setting
                RTC_UpdateDay();  // open weekday setting
                // restart timeout counter
                GetRTCTimeInfo(&hour,&min,&sec);
                start_sec = sec;
                continue;
            }
            else if(key=='3')
            {
                SetMedicineSchedule(); // configure medicine schedule
                // restart timeout counter
				 GetRTCTimeInfo(&hour,&min,&sec);
                start_sec = sec;
                continue;
            }
            else if(key=='4')
            {
                break;      // exit menu
            }
        }
        // read current rtc second
        GetRTCTimeInfo(&hour,&min,&sec);
        cur_sec = sec;
        /* calculate time difference
           handle second rollover case */
        if(cur_sec >= start_sec)
            diff = cur_sec - start_sec;
        else
            diff = 60 - start_sec + cur_sec;
        // check if menu idle for 30 seconds
        if(diff >= 30)
        {
            Cmd_LCD(CLEAR_LCD);
            Str_LCD("MENU TIMEOUT");  // show timeout message
            delay_ms(500);
            break;                    // exit menu
        }
    }
}
