/*
 * ADC_prv.h
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */

#ifndef MCAL_ADC_ADC_PRV_H_
#define MCAL_ADC_ADC_PRV_H_

#include "../../LIB/STD_TYPES.h"

#define CR1_RES_BIT      24
#define CR1_SCAN_BIT     8
#define CR1_EOCIE_BIT    5

#define CR2_CONT_BIT     1
#define CR2_SWSTART_BIT  30
#define CR2_EOCS_BIT     10
#define CR2_ALIGN_BIT    11

#define SR_EOC_BIT       1

#define CCR_ADCPRE_BIT   16

#define SQR1_LENGTH_BIT  20

typedef struct {
	u32 SR;
	u32 CR1;
	u32 CR2;
	u32 SMPR1;
	u32 SMPR2;
	u32 JOFR1;
	u32 JOFR2;
	u32 JOFR3;
	u32 JOFR4;
	u32 HTR;
	u32 LTR;
	u32 SQR1;
	u32 SQR2;
	u32 SQR3;
	u32 JSQR;
	u32 JDR1;
	u32 JDR2;
	u32 JDR3;
	u32 JDR4;
    u32 DR;
}ADC_MemMap_t;

typedef struct{
	u32  CCR;
}ADC_CommReg_t;

#define ADC_BASE_ADDR    (0x40012000)

#define ADC1             ((volatile ADC_MemMap_t*)(ADC_BASE_ADDR))
#define ADC_CCR          ((volatile  ADC_CommReg_t*)(ADC_BASE_ADDR + 0x304))

#endif /* MCAL_ADC_ADC_PRV_H_ */
