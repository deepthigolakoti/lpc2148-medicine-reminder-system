#ifndef CONFIG_MED_H
#define CONFIG_MED_H

/* interrupt pin select */

#define SW1_PINSEL (3<<2)   // P0.1  EINT0
#define SW2_PINSEL (3<<6)   // P0.3  EINT1


#define EINT0_VIC_CHNO 14
#define EINT1_VIC_CHNO 15

#define BUZZER_ALERT 23

#endif

