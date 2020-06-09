#include "DCMotor.h"

static uint8_t _ui8Direction;


void vinitMotor(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative)
{
		vInitGPIOclk(GPIOB);
		SS_PORT_Name->AMSEL 	&=~((1<<pin_Positive) | (1<<pin_Negative));												
		SS_PORT_Name->DEN 		|= ((1<<pin_Positive) | (1<<pin_Negative));
		SS_PORT_Name->DIR    	|= ((1<<pin_Positive) | (1<<pin_Negative));
		SS_PORT_Name->DR8R  	|= ((1<<pin_Positive) | (1<<pin_Negative));
}

void vMotor_CW(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative)
{
		SS_PORT_Name->DATA_Bits[1<<pin_Positive] |= 1<<pin_Positive;
		SS_PORT_Name->DATA_Bits[1<<pin_Negative] &= 0;
}

void vMotor_CCW(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative)
{
		SS_PORT_Name->DATA_Bits[1<<pin_Negative] |= 1<<pin_Negative;
		SS_PORT_Name->DATA_Bits[1<<pin_Positive] &= 0;
}
void vMotor_Stop(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative)
{
		SS_PORT_Name->DATA_Bits[1<<pin_Negative] &= 0;
		SS_PORT_Name->DATA_Bits[1<<pin_Positive] &= 0;
}

void vSetDirection(uint8_t ui8DirectionSet)
{
	_ui8Direction = ui8DirectionSet;
}
uint8_t ui8GetDirection(void)
{
	return _ui8Direction;
}

void vMotor_Emergency_Stop(uint32_t ui32CurrentSpeed, GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative )
{
		if (_ui8Direction==1 && ui32CurrentSpeed > 20)
		{
			vMotor_CCW(SS_PORT_Name,pin_Positive,pin_Negative);
		}
		else if(_ui8Direction==0 && ui32CurrentSpeed > 20)
		{
			vMotor_CCW(SS_PORT_Name,pin_Positive,pin_Negative);
		}
		else
		{
			vMotor_Stop(SS_PORT_Name,pin_Positive,pin_Negative);
		}
}
