#ifndef __DASHB_COMMN__
#define __DASHB_COMMN__

#include "stdint.h"
#include "TM4C123GH6PM.h"
#include "inc/tm4c123gh6pm.h"
#include "uart.h"

/* Dashboard UART configuration */
#define DashBoard_BaudRate 9600
#define MaxBufferMsg_Size 10

/* Dashboard Data Headers	*/
#define CurrentSpeed 		"Sp"
#define BatteryLevel 		"Bl"
#define EngineHeat 			"Te"
#define Brakes 					"Br"
#define BtnControl			"BC"
#define CheckEngine		 	"Ce"
#define Direction 			"Dr"
#define BatteryCharging "Bc"
#define Gearshift 			"Gs"
#define UltrasonicX 		"Ux"
#define UltrasonicY 		"Uy"
#define Fault 					"Cf"
#define TargetSpeed 		"Ts"
#define InsideCarTemp 	"Ti"

void 
	vDashboardConnect_Init(UART0_Type *UART);
void 
	vDashboardSend(uint8_t* pui8_header ,uint8_t value);

void 
	vDashboardSendTwoBytes(uint8_t* pui8_header ,uint8_t value);
void 
	vDashboardSendThreeBytes(uint8_t* pui8_header ,uint8_t value);
void 
	vDashboardSendControl(uint8_t* pui8_header ,uint8_t Alarmbtn,uint8_t FrontLBtn,uint8_t LeftDirBtn,uint8_t RightDirBtn,uint8_t BrakesBtn);
uint8_t* 
	itoa(int num, char* str, int base);


#endif