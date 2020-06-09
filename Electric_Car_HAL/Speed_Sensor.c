#include "Speed_Sensor.h"

/*
*		This is responsible for speed measuring
*/

volatile uint32_t _ui32PulseCount = 0;																																					// the counter updated by interrupt

uint32_t vGetCounterValue(void)
{
	/* Gets Counter value from interrupt */
	uint32_t count = 0;
	count = _ui32PulseCount;
	_ui32PulseCount = 0;
	return count;
}

uint32_t ui32GetSpeed_M_S( uint32_t ui32Radius_cm ,uint32_t ui32NUMofSLOTS,uint32_t ui32intervalTime_ms )
{
	/* Calculates speed based on counter value and disk radius and number of slots and the interval time of the pulses count */
	uint32_t ui32Speed = 0;
	ui32Speed = (2*314*vGetCounterValue())/(ui32NUMofSLOTS*ui32intervalTime_ms);						// the equation that results in the speed value
	return ui32Speed;
}
uint32_t ui32Convert_MStoKmH(uint32_t ui32Speed )
{
	/* Converts millimeter per hour to Kilometer per hour */
	ui32Speed=(ui32Speed*36)/10;
	return ui32Speed;
}




uint32_t ui32speedAvg(uint32_t ui32Speed ,uint32_t ui32No_ofElem)
{
	/* Gets the average of given consecutive speed reads */
	static uint32_t speeds[20] , ElemCount ;
	uint8_t tempElemCount = ui32No_ofElem;
	speeds[ElemCount] = ui32Speed;
	ElemCount++;
	ElemCount%=ui32No_ofElem;
	ui32Speed = 0;
		while(tempElemCount != 0)
		{
			ui32Speed += speeds[tempElemCount-1];
			tempElemCount--;
		}
		ui32Speed/=ui32No_ofElem;
	return ui32Speed;
}


void vInit_Speed_Sensor(GPIOA_Type* gpSS_PORT_Name , uint8_t ui8SpS_input_pin)
{
	/* Initialize the speed sensor pin */
	vInitGPIOclk(gpSS_PORT_Name);
	gpSS_PORT_Name->AMSEL 		&= ~(1<<ui8SpS_input_pin);
	gpSS_PORT_Name->DEN 			|= 	(1<<ui8SpS_input_pin);
	gpSS_PORT_Name->DIR   		&= ~(1<<ui8SpS_input_pin);
}




void vUpdate_Speed_Sensor_PulsesCount(GPIOA_Type* gpSS_PORT_Name , uint8_t ui8pin)
{
	/* Updates the speed count when an interrupt is not used */
	static uint8_t SpeedRepetitionFlag=1;
	if( SpeedRepetitionFlag == 1)
				{
					_ui32PulseCount++;
					SpeedRepetitionFlag = 0;
				}
				else
				{
					SpeedRepetitionFlag = 0;
				}
}



uint32_t ui32map_value(int32_t ui32Val, int32_t si32CurrentMinVal,int32_t si32CurrentMaxVal, int32_t si32DesiredMinVal,int32_t si32DesiredMaxVal)
{
	/* map any value between two ranges to the same value in another two ranges */
	ui32Val = si32DesiredMinVal+((ui32Val-si32CurrentMinVal)*(si32DesiredMaxVal - si32DesiredMinVal))/(si32CurrentMaxVal - si32CurrentMinVal);
	return ui32Val;
}