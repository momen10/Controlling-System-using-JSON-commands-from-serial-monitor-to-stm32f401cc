/*
 * main.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Dell
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "LIB/STD_TYPES.h"

u8 * Enable_Command     = (u8*)"ENA";
u8 * Disable_Command    = (u8*)"DIS";
u8 * Action_Command     = (u8*)"ACT";
u8 * Status_Command     = (u8*)"STA";
u8 * Change_Dur_Command = (u8*)"DUR";

u8 * Done_Data          = (u8*)"DONE";

u8 * Temp_Sensor_ID_str  = (u8*)"0x80";
u8 * Light_Sensor_ID_str = (u8*)"0x81";
u8 * Relay_ID_str        = (u8*)"0x50";

typedef enum{
	Temp_Sensor_ID  = 0x80,
	Light_Sensor_ID = 0x81,
	Relay_ID        = 0x50
}Node_ID;

typedef enum{
	Node_Disabled,
	Node_Enabled
}Node_Status;

typedef struct{
	Node_ID      ID;
	Node_Status  Status;
	u16          Data;
	u8           Duration;
}Node_t;

#endif /* MAIN_H_ */
