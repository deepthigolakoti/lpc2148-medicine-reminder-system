#include <lpc214x.h>
#include "rtc_med.h"
#include "lcd_med.h"
#include "lcd_defines_med.h"
#include "kpm_med.h"
#include "delay_med.h"
#include "medicine_med.h"
#include "config_med.h"
#include "interrupt_med.h"
#include "menu_med.h"
// RTC variables used across modules
s32 hour,min,sec;
s32 date,month,year;
s32 day;
// flags set by interrupt service routines
int i_flag1 = 0;   // set when SW1 is pressed
int med_ack = 0;     // set when SW2 acknowledges medicine
int last_min;
int main()
{
    RTC_Init();          // start RTC
    Init_LCD();          // initialize LCD display
    initKPM();           // initialize keypad
    Interrupt_Init();    // configure SW1 and SW2 interrupts
    IODIR0 |= (1<<BUZZER_ALERT);   // configure buzzer pin as output
    IOCLR0 = (1<<BUZZER_ALERT);    // buzzer OFF initially Active low
    while(1)
    {
        // check if SW1 requested menu
        if(i_flag1)
        {
            i_flag1 = 0;          // clear request
           // VICIntEnClr = (1<<14);  // disable SW1 interrupt temporarily
            Menu_Setup();           // open configuration menu
            Cmd_LCD(CLEAR_LCD);     // clear screen after menu exit
           // EXTINT = (1<<0);        // clear interrupt flag
           // VICIntEnable = (1<<14); // enable SW1 interrupt again
        }
        // read current RTC time
        GetRTCTimeInfo(&hour,&min,&sec);
        // read current weekday
        GetRTCDay(&day);
        // read current date
        GetRTCDateInfo(&date,&month,&year);
        // check if medicine reminder should run
                    if(CheckMedicineReminder(hour,min,&med_ack)==0)
                                {
                                   DisplayRTCTime(hour,min,sec);
                                   DisplayRTCDay(day);
                                   DisplayRTCDate(date,month,year);
                                  }
    }
}
