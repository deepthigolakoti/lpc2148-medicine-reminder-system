#ifndef INTERRUPT_MED_H
#define INTERRUPT_MED_H

void Interrupt_Init(void);
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

#endif
