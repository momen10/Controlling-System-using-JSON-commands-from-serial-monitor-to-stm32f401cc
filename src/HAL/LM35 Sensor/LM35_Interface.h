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

#define LM35_MAX_TEMP                  150
#define LM35_MAX_VOLT                  1.5


u8 LM35_getTemperature (ADC_Config_t * adc_struct);



#endif /* HAL_LM35_SENSOR_LM35_INTERFACE_H_ */
