#include <lpc214x.h>
#include "interrupt_med.h"
#include "config_med.h"

/* variables defined in main.c */
extern int i_flag1;
extern int med_ack;
// initialize external interrupts for switches
// SW1 -> menu trigger, SW2 -> medicine acknowledge
void Interrupt_Init(void)
{
    /* configure SW1 pin as EINT0 function */
    PINSEL0 &= ~(3<<2);     // clear pin function bits
    PINSEL0 |= (3<<2);      // select EINT0 function
    /* configure SW2 pin as EINT1 function */
    PINSEL0 &= ~(3<<6);     // clear pin function bits
    PINSEL0 |= (3<<6);      // select EINT1 function
    /* clear any pending interrupt flags */
    EXTINT = (1<<0) | (1<<1);
    /* configure interrupts as edge triggered */
    EXTMODE |= (1<<0) | (1<<1);
    /* select falling edge trigger */
    EXTPOLAR &= ~((1<<0)|(1<<1));
    /* select IRQ mode instead of FIQ */
    VICIntSelect &= ~((1<<14)|(1<<15));
    /* configure vector slot for EINT0 */
    VICVectCntl0 = (1<<5) | 14;           // enable slot and assign interrupt number
    VICVectAddr0 = (unsigned int)eint0_isr; // address of ISR
    /* configure vector slot for EINT1 */
    VICVectCntl1 = (1<<5) | 15;           // enable slot and assign interrupt number
    VICVectAddr1 = (unsigned int)eint1_isr;
    /* enable EINT0 and EINT1 interrupts */
    VICIntEnable |= (1<<14) | (1<<15);
}

/* SW1 interrupt service routine
   used to open menu */
void eint0_isr(void) __irq
{
	// inform main program to open menu
    i_flag1 = 1; 
// clear EINT0 interrupt flag	
    EXTINT = (1<<0);  
// signal end of interrupt to VIC  
    VICVectAddr = 0;    
}

/* SW2 interrupt service routine
   used to acknowledge medicine alert */
void eint1_isr(void) __irq
{
	// medicine acknowledged
    med_ack = 1;  
 // stop buzzer	
	  IOCLR0 = (1<<BUZZER_ALERT);     
// clear EINT1 interrupt flag  
   EXTINT = (1<<1);        
 // end of interrupt	
    VICVectAddr = 0;                  
}
