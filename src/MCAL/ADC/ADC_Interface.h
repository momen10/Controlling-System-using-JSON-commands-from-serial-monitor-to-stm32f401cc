/*
 * ADC_Interface.h
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */

#ifndef MCAL_ADC_ADC_INTERFACE_H_
#define MCAL_ADC_ADC_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

#define ADC_MAX_10_BIT          1023
#define ADC_MAX_12_BIT          4095
#define ADC_MAX_8_BIT           255
#define ADC_MAX_6_BIT           63
#define ADC_INTERRUPT_PRIORITY  18
#define ADC_VREF                3.3f

typedef enum{
	ADC_Scan_Disabled,
	ADC_Scan_Enabled
}ADC_Scan_Mode;

typedef enum{
	ADC_Resolution12Bits,
	ADC_Resolution10Bits,
	ADC_Resolution8Bits,
	ADC_Resolution6Bits
}ADC_Resolution_t;

typedef enum{
	ADC_PCLK_2,
	ADC_PCLK_4,
	ADC_PCLK_6,
	ADC_PCLK_8,
}ADC_Prescaler_t;

typedef enum{
	ADC_CHANNEL_0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
	ADC_CHANNEL_8,
	ADC_CHANNEL_9,
	ADC_CHANNEL_10,
	ADC_CHANNEL_11,
	ADC_CHANNEL_12,
	ADC_CHANNEL_13,
	ADC_CHANNEL_14,
	ADC_CHANNEL_15,
	ADC_CHANNEL_16,
	ADC_CHANNEL_17,
	ADC_CHANNEL_18,
}ADC_Channel_Num;

typedef enum{
	SMP_3_cycles,
	SMP_15_cycles,
	SMP_28_cycles,
	SMP_56_cycles,
	SMP_84_cycles,
	SMP_112_cycles,
	SMP_144_cycles,
	SMP_480_cycles
}ADC_Channel_Samp_time;

typedef enum{
	EOC_Sequence,
	EOC_Conversion
}ADC_EOC_Sel;


typedef struct{
	ADC_Channel_Num          Channel_Num;
	u8                       Channel_Sequence;
	ADC_Channel_Samp_time    Sampling_Time;
}ADC_Channel_t;


typedef enum{
	ADC_Interrupt_Disabled,
	ADC_Interrupt_Enabled
}ADC_Interrupt_t;

typedef enum{
	ADC_Single_Conversion,
	ADC_Cont_Conversion
}ADC_Conversion_t;

typedef enum{
	ADC_Right_Align,
	ADC_Left_Align
}ADC_DataAlign_t;

typedef struct{
	ADC_Resolution_t Resolution;
	ADC_Interrupt_t  Interrupt;
	ADC_Conversion_t Conversion;
	u8               Num_Conversions;
	ADC_Prescaler_t  Prescaler;
	ADC_DataAlign_t  Align;
	ADC_Scan_Mode    Scan_Mode;
	ADC_EOC_Sel      EOC_sel;
}ADC_Config_t;

void MADC_vInit(ADC_Config_t* ADC_Struct);
void MADC_vChannelInit(ADC_Channel_t* Channel_struct);
void MADC_vGetReading(void);

#endif /* MCAL_ADC_ADC_INTERFACE_H_ */
