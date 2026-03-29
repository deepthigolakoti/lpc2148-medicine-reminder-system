#include "delay_med.h"
void delay_us(unsigned int us)
{
    for(us *= 12; us > 0; us--);
}
void delay_ms(unsigned int ms)
{
    for(ms *= 12000; ms > 0; ms--);
}
void delay_s(unsigned int s)
{
    for(s *= 12000000; s > 0; s--);
}
