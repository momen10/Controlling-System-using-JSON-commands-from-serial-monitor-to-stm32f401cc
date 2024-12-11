/*
 * LM35_Prog.c
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */


#include "../../LIB/STD_TYPES.h"
#include "LM35_Interface.h"
#include "../../MCAL/ADC/ADC_Interface.h"

extern u16 adc_lm35_reading;

u8 LM35_getTemperature (ADC_Config_t * adc_struct)
{
	u8 Temp_Read ;
	u16 lm35_max_reading;

	switch(adc_struct->Resolution)
	{
	case ADC_Resolution6Bits:
		lm35_max_reading = (u16)((u32)ADC_MAX_6_BIT*LM35_MAX_VOLT/ADC_VREF);
		break;
	case ADC_Resolution8Bits:
		lm35_max_reading = (u16)((u32)ADC_MAX_8_BIT*LM35_MAX_VOLT/ADC_VREF);
		break;
	case ADC_Resolution10Bits:
		lm35_max_reading = (u16)((u32)ADC_MAX_10_BIT*LM35_MAX_VOLT/ADC_VREF);
		break;
	case ADC_Resolution12Bits:
		lm35_max_reading = (u16)((u32)ADC_MAX_12_BIT*LM35_MAX_VOLT/ADC_VREF);
		break;
	}

	Temp_Read = (u8) ((u32)(adc_lm35_reading*LM35_MAX_TEMP)/lm35_max_reading)  ;

	return Temp_Read;
}

