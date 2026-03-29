#include <lpc214x.h>
#include "lcd_defines_med.h"
#include "lcd_med.h"
#include "delay_med.h"
// initialize lcd and configure display mode
// sets lcd to 8-bit and 2 line display
void Init_LCD(void)
{
        IODIR0 |= 0xFF << LCD_DATA;
        IODIR0 |= 1 << RS;
        IODIR0 |= 1 << RW;
        IODIR0 |= 1 << EN;
        delay_ms(15);
        Cmd_LCD(MODE_8BIT_1LINE);
        delay_ms(5);
        Cmd_LCD(MODE_8BIT_1LINE);
        delay_us(100);
        Cmd_LCD(MODE_8BIT_1LINE);
        Cmd_LCD(MODE_8BIT_2LINE);
        //Cmd_LCD(DISP_ON_CUR_ON);
        Cmd_LCD(DISP_ON);
        Cmd_LCD(CLEAR_LCD);
        Cmd_LCD(SHIFT_CUR_RIGHT);
}
// send command to lcd
void Cmd_LCD(char cmd)
{
        IOCLR0 = 1 << RS;
        Write_LCD(cmd);
}
// write data or command to lcd
// generates enable pulse for lcd
void Write_LCD(char data)
{
        IOCLR0 = 1 << RW;
        IOPIN0 = IOPIN0 & ~(0xFF << LCD_DATA) | data << LCD_DATA;
        IOSET0 = 1 << EN;
        delay_us(1);
        IOCLR0 = 1 << EN;
        delay_ms(2);
}
// display single character on lcd
void Char_LCD(char ch)
{
        IOSET0 = 1 << RS;
        Write_LCD(ch);
}
// display string on lcd
// prints characters until null character
void Uint_LCD(unsigned int n)
{
        char a[10];
        int i = 0;
        if( n == 0)
        {
                Char_LCD('0');
        }
        else
        {
                /* store digits in reverse order
                   then display in correct order */
                while(n)
                {
                        a[i++] = (n%10) + 48;
                        n/=10;
                }
                for(--i; i >= 0; i--)
                        Char_LCD(a[i]);
        }
}
// display signed integer on lcd
void Sint_LCD(int n)
{
        if(n<0)
        {
                Char_LCD('-');
                n=-n;
                Uint_LCD(n);
        }
}
// display floating point value
// prints required decimal points
void Float_LCD(float num,char nDP)
{
        unsigned int n;
        int i;
        if(num < 0)
        {
                Char_LCD('-');
        }
        n = num;
        Uint_LCD(n);
        Char_LCD('.');
        for(i=0; i < nDP; i++)
        {
                num = (num - n)*10;
				  n = num;
                Char_LCD(n+48);
        }
}
// display hexadecimal value on lcd
void Hex_LCD(unsigned int n)
{
        char a[8],rem;
        int i=0;
        if(n == 0)
        {
                Char_LCD('0');
        }
        else
        {
                /* convert number to hex digits
                   store in reverse order */
                while(n)
                {
                        rem = n % 16;
                        (rem < 10)?(rem+=48):(rem+=55);
                        a[i++]=rem;
                        n/=16;
                }
                for(--i; i >= 0; i--)
                {
                        Char_LCD(a[i]);
                }
        }
}
// display binary value on lcd
// prints specified number of bits
void Bin_LCD(unsigned int n, char nBB)
{
        int i;
        for(i=(nBB-1); i >= 0; i--)
        {
                Char_LCD(((n>>i)&1)+48);
        }
}
// create custom character in lcd cgram
void Build_CGRAM(char *p,char nb)
{
        int i;
        Cmd_LCD(GOTO_CGRAM);
        for(i=0; i <=nb;i++)
		 {
                Char_LCD(p[i]);
        }
        Cmd_LCD(GOTO_LINE1_POS0);
}
