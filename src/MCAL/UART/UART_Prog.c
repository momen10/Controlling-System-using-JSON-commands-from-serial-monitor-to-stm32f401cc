/*
   NAME        : UART_Prog.c

   AUTHOR      : MO'MEN AHMED

   DATE        : 28/8/2023

   DESCRIPTION : Program source file for UART driver
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "UART_Interface.h"
#include "UART_Config.h"
#include "UART_prv.h"

#include "../GPIO/GPIO_int.h"

void (*MUART1_CallBackPtr)(void)=NULL;
void (*MUART2_CallBackPtr)(void)=NULL;
void (*MUART6_CallBackPtr)(void)=NULL;

extern MUART_Config_t My_UART;

void MUART_vInit(volatile UART_MemMap_t* UARTx)
{
// //1-FIRST CONFIGURE TX AND RX BITS AS ALTERNATIVE FUNCTIONS

	if(UARTx == UART1)
	{
		MGPIO_Config_t Tx_Pin ={
				.Port_ID = GPIO_Port_A, .Pin_ID= GPIO_Pin9, .output_type = Push_pull,
				.output_speed = High, .mode = Alternative_func, .Alt_func = AF7
		};

		MGPIO_Config_t Rx_Pin ={
				.Port_ID = GPIO_Port_A, .Pin_ID= GPIO_Pin10, .output_type = Push_pull,
				.output_speed = High, .mode = Alternative_func, .Alt_func = AF7
		};

		MGPIO_vInit(&Tx_Pin);
		MGPIO_vInit(&Rx_Pin);
	}
	else if (UARTx == UART2)
		{
		MGPIO_Config_t Tx_Pin ={
				.Port_ID = GPIO_Port_A, .Pin_ID= GPIO_Pin2, .output_type = Push_pull,
				.output_speed = High, .mode = Alternative_func, .Alt_func = AF7
		};

		MGPIO_Config_t Rx_Pin ={
				.Port_ID = GPIO_Port_A, .Pin_ID= GPIO_Pin3, .output_type = Push_pull,
				.output_speed = High, .mode = Alternative_func, .Alt_func = AF7
		};

		MGPIO_vInit(&Tx_Pin);
		MGPIO_vInit(&Rx_Pin);
		}
	else
	{
		MGPIO_Config_t Tx_Pin ={
				.Port_ID = GPIO_Port_A, .Pin_ID= GPIO_Pin11, .output_type = Push_pull,
				.output_speed = High, .mode = Alternative_func, .Alt_func = AF7
		};

		MGPIO_Config_t Rx_Pin ={
				.Port_ID = GPIO_Port_A, .Pin_ID= GPIO_Pin12, .output_type = Push_pull,
				.output_speed = High, .mode = Alternative_func, .Alt_func = AF7
		};

		MGPIO_vInit(&Tx_Pin);
		MGPIO_vInit(&Rx_Pin);
	}

	//2- SET BAUDE RATE BY MACRO
	switch (My_UART.OverSample)
	{
	case UART_OverSample_8:
		UARTx->BRR = UART_BRR_SAMPLING8(8000000, My_UART.BaudRate);
		break;
	case UART_OverSample_16:
		UARTx->BRR = UART_BRR_SAMPLING16(8000000, My_UART.BaudRate);
		break;
	}

	//3-SET THE REMAINING OPTIONS:
	UARTx -> CR[0] = ((My_UART.OverSample<<UART_OVER8_BIT)    |
			          (My_UART.WordLength<< UART_M_BIT)       |
					  (My_UART.ParityControl << UART_PCE_BIT) |
					  (My_UART.ParitySelect << UART_PS_BIT)   |
					  (My_UART.IntEnableStruct.TXEIE << UART_TXEIE_BIT) | (My_UART.IntEnableStruct.TCIE << UART_TCIE_BIT)|
					  (My_UART.IntEnableStruct.PEIE << UART_PEIE_BIT)	| (My_UART.IntEnableStruct.RXNEIE << UART_RXNEIE_BIT)|
					  (My_UART.TransFlow <<  UART_RE_BIT));

	UARTx -> CR[1] |= (My_UART.StopBits << UART_STOP0_BIT) ;

	UARTx -> CR[2] |= (My_UART.SampleBitMethod << UART_ONEBIT_BIT);

	MUART_voidClearFlags(UARTx);
	MUART_vEnable(UARTx);
}

void MUART_vEnable(volatile UART_MemMap_t* UARTx)
{
	SET_BIT(UARTx -> CR[0] , UART_UE_BIT);
}

void MUART_vDisable(volatile UART_MemMap_t* UARTx)
{
	CLR_BIT(UARTx -> CR[0] , UART_UE_BIT);
}

void MUART_vTransmitByteSynch(volatile UART_MemMap_t* UARTx , s8 A_u8Byte)
{
	//1-FIRST CHECK THAT TRANSMIT REGISTER IS EMPTY
	while(GET_BIT(UARTx -> SR , UART_TXE_BIT) == 0){
		asm("NOP");
	}
	//2-PUT THE DATA IN THE SATA REGISTER
    if(A_u8Byte == '\n')
    {
    	UARTx->DR = 10;
    }
    else{
	(UARTx->DR) = A_u8Byte;
    }
	//3-WAIT UNTIL TRASNMISSION IS COMPLETE
	while(GET_BIT(UARTx -> SR , UART_TC_BIT) == 0)
	{
		asm("NOP");
	}
	//4-SEQUENCE TO CLEAR THE TC FLAG
//	u8 Local_u8 = UARTx -> SR; // READ SR REGISTER
//	UARTx -> DR = 0;           //WRITE INTO DR
	//OR
	 CLR_BIT(UARTx -> SR , UART_TC_BIT);
}


u8 MUART_vTransmitByteAsynch(volatile UART_MemMap_t* UARTx , u8 A_u8Byte)
{
	if(GET_BIT(UARTx -> SR , UART_TXE_BIT) == 1)
	{
       UARTx -> DR = A_u8Byte;
       return UART_SUCCESS;
	}
	else
	{
		return UART_FAIL;
	}
}

void MUART_vTransmitString(volatile UART_MemMap_t* UARTx , u8* A_ptru8string)
{
	u8 iterator = 0;
	while(A_ptru8string[iterator] != '\0')
	{
		MUART_vTransmitByteSynch(UARTx, A_ptru8string[iterator]);
		iterator++;
	}
	MUART_vTransmitByteSynch(UARTx, A_ptru8string[iterator]);
}

u8 MUART_u8ReceiveByteSynchNonBlocking(volatile UART_MemMap_t *UARTx)
{
 u32 timeout = 0;
 u8 ReceivedByte;
	while(GET_BIT(UARTx -> SR , UART_RXNE_BIT) == 0 )
	{
		if(timeout++ == 500000)
			return 255;
	}

	ReceivedByte = UARTx -> DR;
	return ReceivedByte;
}

void MUART_vReceiveStringSynchNonBlocking(volatile UART_MemMap_t *UARTx ,u8 A_u8str[])
{
	u32 i = 0;
//	u32 timeout_counter = 0;

    while(1)
    {
		A_u8str[i] = MUART_u8ReceiveByteSynchNonBlocking(UARTx);
		if (A_u8str[i] == 255) {
			break; // Timeout occurred, stop receiving
		}

		// Check for newline (ASCII 13, carriage return)
		if (A_u8str[i] == 13) {
			break; // End of string
		}

		i++;
    }
    A_u8str[i] = '\0';

//	while(timeout_counter != TIMEOUT_REC_STR_MAX)
//	{
//		timeout_counter++;
//		A_u8str[i] = MUART_u8ReceiveByteSynchNonBlocking(UARTx);
////		MUART_u8ReceiveByteAsych_(UARTx, &A_u8str[i]);
//		while(A_u8str[i] != 13 && A_u8str[i] != 255)
//		{
//			//    	MUART_vTransmitByteAsynch(UARTx, A_u8str[i]);
////			if(A_u8str[i] == 255) //TIMEOUT
////			{
////				break;
////			}
//			i++;
//			A_u8str[i] = MUART_u8ReceiveByteSynchNonBlocking(UARTx);
////			MUART_u8ReceiveByteAsych_(UARTx, &A_u8str[i]);
//		}
//		A_u8str[i+1] = '\0';
//	}

//    MUART_vTransmitString(UARTx,A_u8str);
//	while(A_u8str[i] != '\n')
//	{
//		i++;
//		A_u8str[i] = MUART_u8ReceiveByteSynchNonBlocking(UARTx);
//	}
//
//	A_u8str[i+1] = '\0';


//	for (; i<A_u32Size ;i++)
//	{
//		A_u8str[i] = MUART_u8ReceiveByteSynchNonBlocking(UARTx);
////		if(A_u8str[i]== 255 )  //TIMEOUT
////		{
//////			A_u8str[i] = '\0';
////			break;
////		}
//	}

//A_u8str[i-1] = '\0';
}

u8 MUART_u8ReceiveByteSynchBlocking(volatile UART_MemMap_t *UARTx )
{
	while(GET_BIT(UARTx -> SR , UART_RXNE_BIT) == 0)
	{
		//DATA HAS NOT BEEN RECEIVED YET
		asm("NOP");
	}

	return UARTx -> DR;
}

void MUART_voidClearFlags(volatile UART_MemMap_t *UARTx)
{
	UARTx -> SR = 0;
}

u8 MUART_u8ReadDataRegister(volatile UART_MemMap_t *UARTx)
{
	return UARTx -> DR;
}

void MUART1_vSetCallBack(void (*ptr) (void) )
{
	ptr = MUART1_CallBackPtr;
}

void MUART2_vSetCallBack(void (*ptr) (void) )
{
	ptr = MUART2_CallBackPtr;
}

void MUART6_vSetCallBack(void (*ptr) (void) )
{
	ptr = MUART6_CallBackPtr;
}

void USART1_IRQHandler(void)
{
	MUART_voidClearFlags(UART1);
	if(MUART1_CallBackPtr != NULL)
	{
		MUART1_CallBackPtr();
	}
}

void USART2_IRQHandler(void)
{
	MUART_voidClearFlags(UART2);
	if(MUART2_CallBackPtr != NULL)
	{
		MUART2_CallBackPtr();
	}
}

void USART6_IRQHandler(void)
{
	MUART_voidClearFlags(UART6);
	if(MUART6_CallBackPtr != NULL)
	{
		MUART6_CallBackPtr();
	}
}

u8 MUART_u8ReceiveByteAsych_(volatile UART_MemMap_t *UARTx, u8* A_pu8Ptr)
{
	u8 ReceiveStatus = 0;

	if(GET_BIT(UARTx -> SR , UART_RXNE_BIT) == 0)
	{
		//DATA HAS NOT BEEN RECEIVED YET
	}
	else
	{
		// UART HAS RECEIVED A BYTE
		ReceiveStatus = 1;
		*A_pu8Ptr = UARTx -> DR;
	}

   return ReceiveStatus;
}


