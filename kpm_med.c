#include <lpc21xx.h>
#include "lcd_defines_med.h"
#include "lcd_med.h"
#include "kpm_med.h"
#include "delay_med.h"

// lookup table for keypad values
unsigned int kpmLUT[4][4]={ {'1','2','3','A'},
                            {'4','5','6','B'},
                            {'7','8','9','C'},
                            {'*','0','=','D'}};

// initialize keypad pins
void initKPM(void)
{
        IODIR1 |=  0xF<<R0;
        IOSET1 |= 0xF<<C0;
}

// check if any column key is pressed
unsigned int colscan(void)
{
        if(((IOPIN1>>C0)&0xF)<15)
		       return 0;
        else
                return 1;
}

// find which row key is pressed
unsigned int rowcheck(void)
{
        unsigned int  rn;
        for(rn=0;rn<4;rn++)
        {
                IOPIN1 = ((IOPIN1 &~(0xF<<R0))|(~(1<<rn)<<R0));
                if(colscan()==0)
                        break;
        }
        IOPIN1 = ((IOPIN1 &~(0xF<<R0))|(0<<R0));
        return rn;
}

// find which column key is pressed
unsigned int colcheck(void)
{
     unsigned int  cn;
		 for(cn=0;cn<4;cn++)
        {
                if(((IOPIN1>>(C0+cn))&1)==0)
                        break;
        }
        return cn;
}

// scan keypad and return pressed key value
unsigned int  keyscan(void)
{
        unsigned int  rn,cn,keyV;
        while(colscan());        // wait until key is pressed                              
        rn=rowcheck();           // detect row
        cn=colcheck();           // detect column
        keyV=kpmLUT[rn][cn];     // get key value from lookup table
        while(!colscan());       // wait until key is released
         delay_ms(200);
        return keyV;             // return pressed key	
}

// read number from keypad
// supports digit entry, backspace, skip and exit
int ReadNum(void)
{
    char key;        // store key pressed from keypad
    int sum = 0;     // store the final numeric value
    int digits = 0;  // count number of digits entered

    while(1)         // continuously check keypad input
    {
      key = keyscan();   // read pressed key
        if(key >= '0' && key <= '9')   // check if key is a digit
        {
            Char_LCD(key);             // display digit on LCD
            sum = (sum * 10) + (key - '0'); // update number value
            digits++;                  // increase digit count
        }
        else if(key == 'B')     // backspace key pressed
        {
            if(digits > 0)      // check if any digit exists
            {
                sum = sum / 10; // remove last digit from number				
                digits--;       // reduce digit count
                Cmd_LCD(0x10);  // move cursor left
                Char_LCD(' ');  // erase character on LCD
                Cmd_LCD(0x10);  // move cursor left again
            }
        }
        else if(key == 'A')     // exit key pressed
        {
            return -2;          // return exit code
        }
        else if(key == 'C')     // skip key pressed
        {
            return -3;          // skip for current input
        }
        else if(key == '=')     // enter key pressed
        {
            if(digits == 0)     // check if no digits entered
                return -1;      // return invalid input
            break;              // finish input
        }
    }
  return sum;   // return final number entered
}
