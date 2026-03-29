#include <lpc214x.h>      // LPC2148 register definitions
#include "medicine_med.h"     // medicine reminder function declarations
#include "lcd_med.h"          // LCD driver functions
#include "lcd_defines_med.h"  // LCD command macros
#include "kpm_med.h"  // keypad definitions
#include "delay_med.h"        // delay functions
#include "config_med.h"       // project configuration macros
#include "types_med.h"        // custom data types like s32
#include "rtc_med.h"          // RTC related functions
#define MED_COUNT 3       // maximum number of medicines supporte
// store medicine hours in array
s32 med_hour[MED_COUNT] = {-1,-1,-1};   // initialize all medicine hours to -1
// store medicine minutes in array
s32 med_min[MED_COUNT]  = {-1,-1,-1};   // initialize medicine minutes to -1
int diff;
// medicine states
// 0 = waiting
// 1 = alert active
// 2 = handled
int med_state[MED_COUNT] = {0,0,0};     // initialize medicine states
// total medicines configured by user
int med_total = 3;                      // default medicine count
s32 start_min[MED_COUNT];
// function to set medicine schedule using keypad
void SetMedicineSchedule(void)
{
    int i,j;                            // loop variables
    // read number of medicines
    do{
        Cmd_LCD(CLEAR_LCD);             // clear LCD display
        Str_LCD("NO OF MED(1-3)");      // ask user to enter number of medicines
        med_total = ReadNum();          // read number from keypad
        if(med_total == -2)             // if A key pressed
            return;                     // exit schedule setup
        if(med_total == -1)             // if '=' pressed without digits
        {
            Cmd_LCD(GOTO_LINE2_POS0);   // move cursor to second line
            Str_LCD("ENTER INPUT");     // show message
            delay_ms(500);              // delay for readability
        }
        else if(med_total < 1 || med_total > 3)   // check valid range
        {
            Cmd_LCD(GOTO_LINE2_POS0);
            Str_LCD("INVALID INPUT");   // show invalid message
            delay_ms(500);
        }
    }while(med_total < 1 || med_total > 3);  // repeat until valid value
    // configure each medicine schedule
    for(i=0;i<med_total;i++)
	 {
        // read medicine hour
        do{
            Cmd_LCD(CLEAR_LCD);        // clear LCD
            Str_LCD("MED");            // display MED text
            Uint_LCD(i+1);             // show medicine number
            Str_LCD(" HR(0-23)");      // prompt hour input
            med_hour[i] = ReadNum();   // read hour from keypad
            if(med_hour[i] == -2)      // exit if A pressed
                return;
            if(med_hour[i] == -1)      // if no digits entered
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("ENTER INPUT");
                delay_ms(500);
            }
            else if(med_hour[i] < 0 || med_hour[i] > 23) // invalid hour
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("INVALID INPUT");
                delay_ms(500);
            }
        }while(med_hour[i] < 0 || med_hour[i] > 23); // repeat until valid hour
        // read medicine minute
        do{
            Cmd_LCD(CLEAR_LCD);        // clear LCD
            Str_LCD("MED");
            Uint_LCD(i+1);             // show medicine number
            Str_LCD(" MIN(0-59)");     // prompt minute input
            med_min[i] = ReadNum();    // read minute from keypad
            if(med_min[i] == -2)       // exit
                return;
            if(med_min[i] == -1)       // no digits
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("ENTER INPUT");
                delay_ms(500);
            }
            else if(med_min[i] < 0 || med_min[i] > 59) // invalid minute
            {
                Cmd_LCD(GOTO_LINE2_POS0);
                Str_LCD("INVALID INPUT");
                delay_ms(500);
   }
            else
            {
                // check duplicate medicine time
                for(j=0;j<i;j++)
				 {
                    if(med_hour[i] == med_hour[j] &&
                       med_min[i]  == med_min[j])   // same hour and minute
                    {
                        Cmd_LCD(GOTO_LINE2_POS0);
                        Str_LCD("TIME ALREADY SET"); // show duplicate message
                        delay_ms(800);
                        i--;        // repeat same medicine entry
                        break;
                    }
                }
                if(j != i)          // if duplicate found
                    continue;       // repeat input
            }
        }while(med_min[i] < 0 || med_min[i] > 59 || med_hour[i] == -1);
        med_state[i] = 0;           // set medicine state to waiting
    }
    // disable unused medicine slots
    for(i=med_total;i<MED_COUNT;i++)
    {
        med_hour[i] = -1;           // mark unused
        med_min[i]  = -1;
        med_state[i] = 0;
    }
    Cmd_LCD(CLEAR_LCD);             // clear display
    Str_LCD("MED SET DONE");        // show success message
    delay_ms(800);                  // wait for user to read
}
// Function to check if medicine reminder should activate
int CheckMedicineReminder(int hour,int min,int *med_ack)
{
    int i;                         // loop variable for checking medicines
    for(i=0;i<med_total;i++)       // loop through all configured medicines
    {
        // check if current time matches medicine schedule
        if(med_hour[i] != -1 && hour == med_hour[i] && min == med_min[i] && med_state[i] == 0)
         {
              med_state[i] = 1;
              start_min[i] = min;   // store start minute
            Cmd_LCD(CLEAR_LCD);
                        }
                        if(med_state[i] ==1)
                        {
                                    Cmd_LCD(GOTO_LINE1_POS0);
             Str_LCD("TAKE MED ");
              Uint_LCD(i+1);
              IOSET0 = (1<<BUZZER_ALERT);
                         }
        // check if alert is currently active
        if(med_state[i] == 1)
        {
            if(*med_ack)                              // check if user acknowledged medicine
            {
                IOCLR0 = (1<<BUZZER_ALERT);           // stop buzzer
                Cmd_LCD(CLEAR_LCD);                   // clear LCD
                Str_LCD("MED ");                      // display MED text
                Uint_LCD(i+1);                        // show medicine number
                Str_LCD(" TAKEN");                    // display taken confirmation
                delay_ms(800);                        // wait so user can read message
                Cmd_LCD(CLEAR_LCD);                   // clear LCD again
                med_state[i] = 2;                     // mark medicine as handled
                *med_ack = 0;                         // reset acknowledgement flag
                Cmd_LCD(CLEAR_LCD);                   // clear display
                return 0;                             // return 0 (no active alert now)
            }
            // check if medicine time passed without acknowledgement
            if(min >= start_min[i])
               diff = min - start_min[i];
           else
                diff = 60 - start_min[i] + min;
             if(diff >= 1)                     // if minute changed
            {
                IOCLR0 = (1<<BUZZER_ALERT);           // stop buzzer
                Cmd_LCD(CLEAR_LCD);                   // clear LCD
                Str_LCD("MISSED MED     ");               // show missed medicine message
                Uint_LCD(i+1);                        // show medicine number
                delay_ms(800);                        // display message for short time
                med_state[i] = 2;                     // mark medicine as handled
                Cmd_LCD(CLEAR_LCD);                   // clear screen
              //  return 0;                             // return 0 (alert finished)
               }
            return 1;                                 // alert still active
        }
        // reset state when minute changes after handling
       if((hour != med_hour[i] || min != med_min[i]) && med_state[i] == 2)
       {
              med_state[i] = 0;
        }                         // reset state so system is ready for next day
    }
return 0;                                         // return 0 if no medicine alert active
}

						