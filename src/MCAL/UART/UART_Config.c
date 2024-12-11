/*
   NAME        : UART_Config.c

   AUTHOR      : MO'MEN AHMED

   DATE        : 28/8/2023

   DESCRIPTION : Configuration source file for UART driver
 */


#include "UART_Interface.h"

MUART_Config_t My_UART = {
	.WordLength = UART_8bits, .TransFlow= UART_TxRx, .StopBits = UART_1StopBit,
	.SampleBitMethod = UART_1SampleMethod, .ParitySelect = UART_EvenParity,
	.ParityControl = UART_ParityDisabled	, .OverSample = UART_OverSample_16
	,.BaudRate = 9600
};

