/*
   NAME        : SYSTICK_cfg.h

   AUTHOR      : MO'MEN AHMED

   DATE        : 17/7/2023

   DESCRIPTION : Configuration header file for Systick driver
 */


#ifndef MCAL_SYSTICK_SYSTICK_CFG_H_
#define MCAL_SYSTICK_SYSTICK_CFG_H_

#include "SYSTICK_prv.h"

//systick clock sources: STK_AHB OR STK_AHB_8
#define STK_CLK_SOURCE   STK_AHB_8

//systick options for interrupt enable: STK_INT_EN STK_INT_DIS
#define STK_INT_OPTION   STK_INT_EN


#endif /* MCAL_SYSTICK_SYSTICK_CFG_H_ */
