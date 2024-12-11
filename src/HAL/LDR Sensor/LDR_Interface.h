/*
 * LM35_Interface.h
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */

#ifndef HAL_LM35_SENSOR_LM35_INTERFACE_H_
#define HAL_LM35_SENSOR_LM35_INTERFACE_H_

#include "../../MCAL/ADC/ADC_Interface.h"
#include "../../LIB/STD_TYPES.h"

#define  LDR_RFIXED       1000
#define  LDR_MAX_VOLT     2.46
#define  LDR_MAX_ILL      1000
#define  LDR_RDARK        341

u16 LDR_GetIlluminance(ADC_Config_t* adc_struct);

#endif /* HAL_LM35_SENSOR_LM35_INTERFACE_H_ */
