
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "LIB/cJSON.h"
#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"
#include "MCAL/RCC/RCC_int.h"
#include "MCAL/SYSTICK/SYSTICK_int.h"
#include "MCAL/NVIC/NVIC_int.h"
#include "MCAL/SCB/SCB_Interface.h"
#include "MCAL/GPIO/GPIO_int.h"
#include "MCAL/UART/UART_Interface.h"
//#include "MCAL/ADC/ADC_Interface.h"
#include "HAL/LM35 sensor/LM35_Interface.h"
#include "HAL/LDR sensor/LDR_Interface.h"
#include "HAL/Relay/Relay_Interface.h"

#define MAX_REC_STR_SIZE    500

volatile u16 reading ;
u16 adc_lm35_reading;
u16 adc_ldr_reading;
u8 temperature ;
u16 illuminance;
u8 M_byte;
u8 L_byte;
int main(void)
{
    MRCC_vInit();
	MRCC_vEnableClock(RCC_AHB1, RCC_GPIO_A);
	MRCC_vEnableClock(RCC_APB2, RCC_USART1);
	MRCC_vEnableClock(RCC_APB1, RCC_USART2);
	MRCC_vEnableClock(RCC_APB2, RCC_ADC1);

    MSTK_vInit();
	MUART_vInit(UART2);

    Relay_Config_t relay={
    		.Relay_Port = GPIO_Port_A,
			.Relay_Pin  = GPIO_Pin0
    };


	//JSON PART
	//CREATING JSON OBJECT
	cJSON * json_object = cJSON_CreateObject();
	cJSON_AddStringToObject(json_object,"command", "ENA");
	cJSON_AddNumberToObject(json_object,"nodeID",0x80);
	cJSON_AddStringToObject(json_object,"data", "NULL");

	//CONVERTING cJSON object to a string
	char * json_str = cJSON_Print(json_object);
//	MUART_vTransmitString(UART2,(u8*)json_str);

	cJSON_free(json_str);
    cJSON_Delete(json_object);

    MGPIO_Config_t My_GPIO_ADC = {.Port_ID         = GPIO_Port_A,
      		                  .Pin_ID          = GPIO_Pin1,
  							  .mode            = Analog_input,
      		                  .output_type     = Push_pull,
  							  .output_speed    = Low,
  							  .input_pull_type = No_resistor,
  							  .Alt_func        = Not_AF };
      MGPIO_vInit(&My_GPIO_ADC);

      MGPIO_Config_t My_GPIO_ADC2 = {.Port_ID         = GPIO_Port_A,
        		                  .Pin_ID          = GPIO_Pin4,
    							  .mode            = Analog_input,
        		                  .output_type     = Push_pull,
    							  .output_speed    = Low,
    							  .input_pull_type = No_resistor,
    							  .Alt_func        = Not_AF };
        MGPIO_vInit(&My_GPIO_ADC2);

    MNVIC_vNvicEnablePerInterrupt(ADC_INTERRUPT_PRIORITY);

    ADC_Config_t ADC_LM35 = {
    		.Resolution      = ADC_Resolution12Bits,
			.Interrupt       = ADC_Interrupt_Disabled,
			.Conversion      = ADC_Single_Conversion,
			.Prescaler       = ADC_PCLK_2,
			.Align           = ADC_Right_Align,
			.Num_Conversions = 1,
			.Scan_Mode       = ADC_Scan_Disabled,
			.EOC_sel         = EOC_Conversion
    };

    MADC_vInit(&ADC_LM35);

    Node_t Temp_Sensor_Node =
    {
    		.ID       = Temp_Sensor_ID,
			.Status   = Node_Disabled,
			.Data     = 0,
			.Duration = 2
    };

    Node_t Light_Sensor_Node =
    {
    		.ID     = Light_Sensor_ID,
			.Status = Node_Disabled,
			.Data   = 0,
			.Duration = 2
    };

    Node_t Relay_Node =
    {
    		.ID     = Relay_ID,
			.Status = Node_Disabled,
			.Data   = 0,
			.Duration = 0
    };

//    MUART_vTransmitString(UART2,(u8*)"aloooo");

	while(1)
	{
	    MADC_vGetReading();

            cJSON* Rec_json_object;


	    if(Temp_Sensor_Node.Status == Node_Enabled)
	    {
	    	cJSON* Tx_json_object = cJSON_CreateObject();
	    	temperature = LM35_getTemperature(&ADC_LM35);
	    	Temp_Sensor_Node.Data = temperature;

			cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(Temp_Sensor_Node.ID));

			cJSON_AddNumberToObject(Tx_json_object,(const char*)"data",(const double)(Temp_Sensor_Node.Data));


    		char* test3 = cJSON_Print(Tx_json_object);
    		MUART_vTransmitString(UART2,(u8*)test3);
    		MUART_vTransmitByteSynch(UART2,13);

    		cJSON_free(test3);
    	    cJSON_Delete(Tx_json_object);

    		MSTK_vDelayms((u32)(1000*Temp_Sensor_Node.Duration));
	    }

	    if(Light_Sensor_Node.Status == Node_Enabled)
	    {
	    	cJSON* Tx_json_object = cJSON_CreateObject();

	    	illuminance = LDR_GetIlluminance(&ADC_LM35);
	    	Light_Sensor_Node.Data = illuminance;

			cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(Light_Sensor_Node.ID));

			cJSON_AddNumberToObject(Tx_json_object,(const char*)"data",(const double)(Light_Sensor_Node.Data));


    		char* test3 = cJSON_Print(Tx_json_object);
    		MUART_vTransmitString(UART2,(u8*)test3);
    		MUART_vTransmitByteSynch(UART2,13);//ENDLINE

    		cJSON_free(test3);
    	    cJSON_Delete(Tx_json_object);

    		MSTK_vDelayms((u32)(1000*Temp_Sensor_Node.Duration));
	    }


		const char* Rec_string[MAX_REC_STR_SIZE];
		MUART_vReceiveStringSynchNonBlocking(UART2,(u8*)Rec_string);

		MSTK_vDelayms(1000);
        Rec_json_object = cJSON_Parse((const char*)(Rec_string));
        if(Rec_json_object == NULL)
        {

        }
        else
        {
        	const char* test = cJSON_Print(Rec_json_object);
        	MUART_vTransmitString(UART2,(u8*)test);
        	MUART_vTransmitByteSynch(UART2,13);

        	cJSON *command = cJSON_GetObjectItem(Rec_json_object, "command");
        	cJSON *node_id = cJSON_GetObjectItem(Rec_json_object, "nodeID");
        	cJSON *data    = cJSON_GetObjectItem(Rec_json_object, "data");


        	if(!strcmp((const char*)(command->valuestring), (const char*)(Enable_Command)))
        	{
        		cJSON* Tx_json_object = cJSON_CreateObject();
        		if(cJSON_IsString(node_id))
        		{
        			long node_long = strtol((const char*)(node_id->valuestring),NULL,16);

        			if((u8)node_long == Temp_Sensor_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
        				Temp_Sensor_Node.Status = Node_Enabled;
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"data", (const char*)"DONE");
        			}
        			else if ((u8)node_long == Light_Sensor_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
                        Light_Sensor_Node.Status = Node_Enabled;
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"data", (const char*)"DONE");
        			}
        			else if((u8)node_long == Relay_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NA");
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"data", (const char*)"DONE");
        			    HRELAY_vInit(&relay);
        			    HRELAY_vTurnOff(&relay);
        				Relay_Node.Status = Node_Enabled;
        			}
        		}

        		char* test2 = cJSON_Print(Tx_json_object);
        		MUART_vTransmitString(UART2,(u8*)test2);
        		MUART_vTransmitByteSynch(UART2,13);

        		cJSON_free(test2);
        	    cJSON_Delete(Tx_json_object);
        	}
        	else if (!strcmp((const char*)(command->valuestring), (const char*)(Disable_Command)))
        	{
        		cJSON* Tx_json_object = cJSON_CreateObject();
        		if(cJSON_IsString(node_id))
        		{
        			long node_long = strtol((const char*)(node_id->valuestring),NULL,16);

        			if((u8)node_long == Temp_Sensor_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
        				Temp_Sensor_Node.Status = Node_Disabled;
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"data", (const char*)"DONE DIS");
        			}
        			else if ((u8)node_long == Light_Sensor_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
                        Light_Sensor_Node.Status = Node_Disabled;
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"data", (const char*)"DONE DIS");
        			}
        			else if((u8)node_long == Relay_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NA");
                        Relay_Node.Status = Node_Disabled;
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"data", (const char*)"DONE DIS");
        			}
        		}

        		char* test2 = cJSON_Print(Tx_json_object);
        		MUART_vTransmitString(UART2,(u8*)test2);
        		MUART_vTransmitByteSynch(UART2,13);

        		cJSON_free(test2);
        	    cJSON_Delete(Tx_json_object);
        	}
        	else if (!strcmp((const char*)(command->valuestring), (const char*)(Action_Command)))
        	{
        		if(cJSON_IsString(node_id))
        		{
        			long node_long = strtol((const char*)(node_id->valuestring),NULL,16);
        			long data_long = strtol((const char*)(data->valuestring),NULL,10);

        			if((u8)node_long == Relay_ID)
        			{
        				if(data_long == 1)
        				{
        					if(Relay_Node.Status == Node_Enabled)
        					{
        						Relay_Node.Data = 1;
        						HRELAY_vTurnOn(&relay);
        					}
        					else
        					{

        					}
        				}
        				else
        				{
        					if(Relay_Node.Status == Node_Enabled)
        					{
        						Relay_Node.Data = 0;
        						HRELAY_vTurnOff(&relay);
        					}
        					else
        					{

        					}
        				}
        			}
        			else
        			{
        			}
        		}
        	}
        	else if (!strcmp((const char*)(command->valuestring), (const char*)(Status_Command)))
        	{
        		cJSON* Tx_json_object = cJSON_CreateObject();
        		if(cJSON_IsString(node_id))
        		{
        			long node_long = strtol((const char*)(node_id->valuestring),NULL,16);

        			if((u8)node_long == Temp_Sensor_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"data", Temp_Sensor_Node.Status);
        			}
        			else if ((u8)node_long == Light_Sensor_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NS");
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
        				cJSON_AddNumberToObject(Tx_json_object,(const char*)"data", Light_Sensor_Node.Status);
        			}
        			else if((u8)node_long == Relay_ID)
        			{
        				cJSON_AddStringToObject(Tx_json_object,(const char*)"nodeType", (const char*)"NA");
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"nodeID",(const double)(node_long));
            			cJSON_AddNumberToObject(Tx_json_object,(const char*)"data", Relay_Node.Data);
        			}
        			else
        			{

        			}
        		}

        		char* test2 = cJSON_Print(Tx_json_object);
        		MUART_vTransmitString(UART2,(u8*)test2);
        		MUART_vTransmitByteSynch(UART2,13);

        		cJSON_free(test2);
        	    cJSON_Delete(Tx_json_object);
        	}
        	else if (!strcmp((const char*)(command->valuestring), (const char*)(Change_Dur_Command)))
        	{
           		if(cJSON_IsString(node_id))
            		{
            			long node_long = strtol((const char*)(node_id->valuestring),NULL,16);
            			long dur_long  = strtol((const char*)(data->valuestring),NULL,10);

            			if((u8)node_long == Temp_Sensor_ID)
            			{
            				Temp_Sensor_Node.Duration = (u8)dur_long;
            			}
            			else if ((u8)node_long == Light_Sensor_ID)
            			{
            				Light_Sensor_Node.Duration = dur_long;
            			}
            			else
            			{
            				Relay_Node.Duration = dur_long;
            			}
            		}
        	}
        }
	}
}

void ADC_IRQHandler(void)
{

}


