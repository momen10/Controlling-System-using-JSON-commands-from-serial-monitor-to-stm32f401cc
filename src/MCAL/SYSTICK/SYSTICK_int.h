/*
   NAME        : SYSTICK_int.h

   AUTHOR      : MO'MEN AHMED

   DATE        : 17/7/2023

   DESCRIPTION : Interface header file for Systick driver
 */

#ifndef MCAL_SYSTICK_SYSTICK_INT_H_
#define MCAL_SYSTICK_SYSTICK_INT_H_

#include "SYSTICK_prv.h"
#include "../../LIB/STD_TYPES.h"

void MSTK_vInit(void);
void MSTK_vDelayms(u32 A_u32DelayMilliSec);
void MSTK_vSetIntervalSingle(u32 A_u32DelayMilliSec, void (*ptr) (void));
void MSTK_vSetIntervalPeriodic(u32 A_u32DelayMilliSec, void (*ptr) (void));
void MSTK_vDelayus(u32 A_u32DelayMicroSec);
void MSTK_vSetIntervalSingleMicro(u32 A_u32DelayMicroSec, void (*ptr) (void));
void MSTK_vSetIntervalPeriodicMicro(u32 A_u32DelayMicroSec, void (*ptr) (void));
u32 MSTK_u32GetElapsedTime (void);
u32 MSTK_u32GetRemainingTime (void);
void MSTK_vStartTimer(u32 A_u32PreloadValue);
void MSTK_vStopTimer(void);
void MSTK_vSetCallBack(void (*copy_ptr)(void));

#endif /* MCAL_SYSTICK_SYSTICK_INT_H_ */
