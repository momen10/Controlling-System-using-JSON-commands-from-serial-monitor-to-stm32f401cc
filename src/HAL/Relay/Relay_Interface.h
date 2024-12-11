/*
 * Relay_Interface.h
 *
 *  Created on: Dec 11, 2024
 *      Author: Mo'men
 */

#ifndef HAL_RELAY_RELAY_INTERFACE_H_
#define HAL_RELAY_RELAY_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO_int.h"


typedef struct{
	GPIO_PortNum_t Relay_Port;
	GPIO_PinNum_t  Relay_Pin;
}Relay_Config_t;

void HRELAY_vInit(Relay_Config_t* relay_struct);
void HRELAY_vTurnOn(Relay_Config_t* relay_struct);
void HRELAY_vTurnOff(Relay_Config_t* relay_struct);


#endif /* HAL_RELAY_RELAY_INTERFACE_H_ */
