#ifndef KPM_DEFINES_MED_H
#define KPM_DEFINES_MED_H 

#define R0 16
#define R1 17
#define R2 18
#define R3 19
#define C0 20
#define C1 21
#define C2 22
#define C3 23

void initKPM(void);
unsigned int colscan(void);
unsigned int rowcheck(void);
unsigned int colcheck(void);
unsigned int keyscan(void);
int ReadNum(void);

#endif
