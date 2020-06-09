#ifndef __SPD_SNSR__
#define __SPD_SNSR__

#include "stdint.h"
#include "TM4C123GH6PM.h"
#include "inc/tm4c123gh6pm.h"
#include "GPIO.h"

uint32_t 
	vGetCounterValue(void);
uint32_t 
	ui32GetSpeed_M_S( uint32_t ui32Radius_cm ,uint32_t ui32NUMofSLOTS,uint32_t ui32intervalTime_ms );
uint32_t 
	ui32Convert_MStoKmH(uint32_t ui32speed );
uint32_t 
	ui32speedAvg(uint32_t ui32Speed ,uint32_t ui32No_ofElem);
void 
	vInit_Speed_Sensor(GPIOA_Type* gpSS_PORT_Name , uint8_t ui8SpS_input_pin);
void 
	vUpdate_Speed_Sensor_PulsesCount(GPIOA_Type* gpSS_PORT_Name , uint8_t ui8pin);
uint32_t 
	ui32map_value(int32_t ui32Val, int32_t si32CurrentMinVal,int32_t si32CurrentMaxVal, int32_t si32DesiredMinVal,int32_t si32DesiredMaxVal);

#endif
