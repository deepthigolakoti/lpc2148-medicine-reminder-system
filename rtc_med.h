#ifndef RTC_MED_H

#define RTC_MED_H


#include "types_med.h"


#define FOSC 12000000

#define CCLK (5 * FOSC)

#define PCLK (CCLK / 4)


#define PREINT_VAL (s32)((PCLK / 32768) - 1)

#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768))


#define RTC_ENABLE (1<<0)

#define RTC_RESET  (1<<1)

#define RTC_CLKSRC (1<<4)


#define SUN 0

#define MON 1

#define TUE 2

#define WED 3

#define THU 4

#define FRI 5

#define SAT 6


extern s32 hour,min,sec,date,month,year,day,key;


void RTC_Init(void);

void GetRTCTimeInfo(s32*,s32*,s32*);
void DisplayRTCTime(u32,u32,u32);

void GetRTCDateInfo(s32*,s32*,s32*);
void DisplayRTCDate(u32,u32,u32);

void GetRTCDay(s32*);
void DisplayRTCDay(u32);

void SetRTCTimeInfo(u32,u32,u32);
void SetRTCDateInfo(u32,u32,u32);
void SetRTCDay(u32);

void RTC_UpdateTime(void);
void RTC_UpdateDate(void);
void RTC_UpdateDay(void);

#endif

