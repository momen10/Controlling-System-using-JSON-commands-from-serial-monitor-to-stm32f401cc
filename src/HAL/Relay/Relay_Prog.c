/*
 * Relay_Prog.c
 *
 *  Created on: Dec 11, 2024
 *      Author: Mo'men
 */

#include "../../LIB/STD_TYPES.h"
#include "Relay_Interface.h"

void HRELAY_vInit(Relay_Config_t* relay_struct)
{
	MGPIO_Config_t My_GPIO = {.Port_ID         = relay_struct->Relay_Port,
	    		                  .Pin_ID          = relay_struct->Relay_Pin,
								  .mode            = Digital_output,
	    		                  .output_type     = Push_pull,
								  .output_speed    = Low,
								  .input_pull_type = No_resistor,
								  .Alt_func        = Not_AF };
	    MGPIO_vInit(&My_GPIO);
}

void HRELAY_vTurnOn(Relay_Config_t* relay_struct)
{
	MGPIO_vSetPin(relay_struct->Relay_Port, relay_struct->Relay_Pin, LOGIC_ONE);
}

void HRELAY_vTurnOff(Relay_Config_t* relay_struct)
{
	MGPIO_vSetPin(relay_struct->Relay_Port, relay_struct->Relay_Pin, LOGIC_ZERO);
}

