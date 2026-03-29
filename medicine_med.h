#ifndef MEDICINE_MED_H
#define MEDICINE_MED_H


void SetMedicineSchedule(void);
int CheckMedicineReminder(int hour,int min,int *med_ack);
void DisplayNextMedicine(int hour,int min);

#endif
