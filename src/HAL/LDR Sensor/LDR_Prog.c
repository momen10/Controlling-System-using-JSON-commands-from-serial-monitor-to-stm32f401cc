/*
 * LM35_Prog.c
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */


#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/ADC/ADC_Interface.h"
#include "LDR_Interface.h"

extern u16 adc_ldr_reading;

u16 LDR_GetIlluminance(ADC_Config_t* adc_struct)
{
	u16 L_illuminance;
    u8 volt_out=0;
    u16 ldr_max_reading;

    float R_LDR;

    switch(adc_struct -> Resolution)
    {
    case ADC_Resolution6Bits:
    	ldr_max_reading = (u16)(ADC_MAX_6_BIT*LDR_MAX_VOLT / ADC_VREF);
    	break;
    case ADC_Resolution8Bits:
    	ldr_max_reading = (u16)(ADC_MAX_8_BIT*LDR_MAX_VOLT / ADC_VREF);
    	break;
    case ADC_Resolution10Bits:
    	ldr_max_reading = (u16)(ADC_MAX_10_BIT*LDR_MAX_VOLT / ADC_VREF);
    	break;
    case ADC_Resolution12Bits:
    	ldr_max_reading = (u16)(ADC_MAX_12_BIT*LDR_MAX_VOLT / ADC_VREF);
    	break;
    default:
    	ldr_max_reading = (u16)(ADC_MAX_10_BIT*LDR_MAX_VOLT / ADC_VREF);
    	break;
    }

    volt_out =(u8) ((u32)(LDR_MAX_VOLT*adc_ldr_reading) / ldr_max_reading);

    R_LDR = (LDR_RFIXED * ADC_VREF / volt_out) - LDR_RFIXED;

    L_illuminance = (u16)((u32)(LDR_MAX_ILL * LDR_RDARK)/R_LDR);


	return L_illuminance;
}

