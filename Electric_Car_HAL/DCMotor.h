#ifndef __SPD_SNSR__
#define __SPD_SNSR__

#include "stdint.h"
#include "TM4C123GH6PM.h"
#include "inc/tm4c123gh6pm.h"
#include "GPIO.h"


void 
	vinitMotor(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative);
void 
	vMotor_CW(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative);
void 
	vMotor_CCW(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative);
void 
	vMotor_Stop(GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative);
void 
	vSetDirection(uint8_t ui8DirectionSet);
uint8_t 
	ui8GetDirection(void);
void 
	vMotor_Emergency_Stop(uint32_t ui32CurrentSpeed, GPIOA_Type* SS_PORT_Name , uint8_t pin_Positive , uint8_t pin_Negative );


#endif
