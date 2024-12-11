/*
 * ADC_Prog.c
 *
 *  Created on: Dec 7, 2024
 *      Author: Dell
 */
#include "../../LIB/STD_TYPES.h"
#include "ADC_Interface.h"
#include "ADC_prv.h"
#include "ADC_Config.h"

extern u16 adc_lm35_reading;
extern u16 adc_ldr_reading;

void MADC_vInit(ADC_Config_t* ADC_Struct)
{
//ENABLING THE ADC
   ADC1->CR2 |= 0x01;

//SETTING THE RESOLUTION OF THE ADC
   (ADC1->CR1) &= ~ (0b11 << CR1_RES_BIT);
   (ADC1->CR1) |= ((ADC_Struct->Resolution) << CR1_RES_BIT);

//SETTING THE SCAN MODE
   (ADC1->CR1) &= ~(1 << CR1_SCAN_BIT);
   (ADC1->CR1) |=  (ADC_Struct->Scan_Mode << CR1_SCAN_BIT);

//SETTING THE ADC CLOCK PRESCALER
   (ADC_CCR->CCR) &= ~(0b11 << CCR_ADCPRE_BIT);
   (ADC_CCR->CCR) |= (ADC_Struct->Prescaler << CCR_ADCPRE_BIT);

//SETTING THE INTERRUPT
   switch(ADC_Struct->Interrupt)
   {
   case ADC_Interrupt_Disabled:
	   ADC1->CR1 &= ~(1 << CR1_EOCIE_BIT);
	   break;
   case ADC_Interrupt_Enabled:
	   ADC1->CR1 |= (1 << CR1_EOCIE_BIT);
	   break;
   }

//SETTING THE TOTAL NUMBER OF CONVERSIONS IN THE SEQUENCE
   (ADC1->SQR1)  &= ~(0b1111 << SQR1_LENGTH_BIT);
   (ADC1->SQR1)  |=  ((ADC_Struct -> Num_Conversions) << SQR1_LENGTH_BIT);

//SETTING THE EOC selection
   (ADC1->CR2)   &= ~(1<<CR2_EOCS_BIT);
   (ADC1->CR2)   |= ((ADC_Struct->EOC_sel)  << CR2_EOCS_BIT );

//CONFIGURING THE CHANNELS
   ADC_Channel_t Channel_LM35 = {
		   .Channel_Num       = ADC_CHANNEL_1,
		   .Channel_Sequence  = 1,
		   .Sampling_Time     = SMP_144_cycles
   };

  MADC_vChannelInit(&Channel_LM35);

//  ADC_Channel_t Channel_LDR = {
//		   .Channel_Num       = ADC_CHANNEL_4,
//		   .Channel_Sequence  = 1,
//		   .Sampling_Time     = SMP_144_cycles
//  };
//
// MADC_vChannelInit(&Channel_LDR);

//SETTING THE CONVERSION TYPE
   switch((ADC_Struct -> Conversion))
   {
   case ADC_Single_Conversion:
	   (ADC1->CR2) &= ~(1 << CR2_CONT_BIT);
	   break;
   case ADC_Cont_Conversion:
	   (ADC1->CR2) |= (1 << CR2_CONT_BIT);
	   break;
   }

//SETTING THE DATA ALIGNMENT OF THE ADC
   (ADC1->CR2)  &= ~(1 << CR2_ALIGN_BIT);
   (ADC1->CR2)  |= ((ADC_Struct ->Align) << CR2_ALIGN_BIT);
}

void MADC_vGetReading(void)
{
//FIRST START THE CONVERSION
	(ADC1->CR2) |= (1<<CR2_SWSTART_BIT);

//	WAIT FOR CONVERSION COMPLETE
	while((ADC1->SR & (1<<SR_EOC_BIT)) == 0)
	{
		asm("NOP");
	}

	//READ THE ADC CHANNEL READING AND CLEAR THE EOC INTERRUPT
	adc_lm35_reading = (ADC1->DR) & 0x0fff ;

//	for (volatile u16 i=0; i<1000;i++)
//	{
//		asm("NOP");
//	}
	//FIRST START THE CONVERSION
//	(ADC1->CR2) |= (1<<CR2_SWSTART_BIT);

	//WAIT FOR CONVERSION COMPLETE
//	while((ADC1->SR & (1<<SR_EOC_BIT)) == 0)
//	{
//		asm("NOP");
//	}
//
////	//READ THE ADC CHANNEL READING AND CLEAR THE EOC INTERRUPT
//	adc_ldr_reading = (ADC1->DR) & 0x0fff ;

}


void MADC_vChannelInit(ADC_Channel_t* Channel_struct)
{
	//SETTING THE SEQUENCE OF THE CHANNEL
	if((Channel_struct -> Channel_Sequence) < 7)
	{
		(ADC1->SQR3) &= ~(0b11111 << (Channel_struct -> Channel_Sequence - 1));
		(ADC1->SQR3) |= ((Channel_struct-> Channel_Num) << 5*(Channel_struct -> Channel_Sequence - 1));
	}
	else if ((Channel_struct -> Channel_Sequence) < 13)
	{
		(ADC1->SQR2) &= ~(0b11111 << (Channel_struct -> Channel_Sequence - 1));
		(ADC1->SQR2) |= ((Channel_struct-> Channel_Num) << 5*(Channel_struct -> Channel_Sequence - 1));
	}
	else
	{
		(ADC1->SQR1) &= ~(0b11111 << (Channel_struct -> Channel_Sequence - 1));
		(ADC1->SQR1) |= ((Channel_struct-> Channel_Num) << 5*(Channel_struct -> Channel_Sequence - 1));
	}

    //SETTING THE SAMPLING TIME OF THE CHANNEL
	if ((Channel_struct->Channel_Num) < ADC_CHANNEL_10)
	{
	   ADC1->SMPR2 &= ~(7 << (Channel_struct->Channel_Num * 3));
	   ADC1->SMPR2 |= (Channel_struct->Sampling_Time << (Channel_struct->Channel_Num* 3));
	}
	else
	{
	   ADC1->SMPR1 &= ~(7 << ((Channel_struct->Channel_Num%10) * 3));
	   ADC1->SMPR1 |= (Channel_struct->Sampling_Time << ((Channel_struct->Channel_Num%10)* 3));
	}
}

