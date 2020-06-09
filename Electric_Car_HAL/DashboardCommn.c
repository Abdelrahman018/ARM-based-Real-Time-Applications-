#include "DashboardCommn.h"

static UART0_Type *psUART;	

void vDashboardConnect_Init(UART0_Type *UART)
{
	/* Initialize the UART used to communicate with the dashboard */
	psUART = UART;
	UART_Init(psUART,DashBoard_BaudRate);
}

void vDashboardSend(uint8_t* pui8_header ,uint8_t value)
{
	/*	Sends one byte of data with it's header to the dashboard	*/
	int8_t BufferMsg[MaxBufferMsg_Size] = {0};
	sprintf(BufferMsg, "%s%d",pui8_header,value);
	UART_TX_Str_P(psUART, BufferMsg);
}

void vDashboardSendTwoBytes(uint8_t* pui8_header ,uint8_t value)
{
	/*	Sends Two byte of data with it's header to the dashboard	*/
	int8_t BufferMsg[MaxBufferMsg_Size] = {0};
	if(value > 9)
		sprintf(BufferMsg, "%s%d",pui8_header,value);
	else
		sprintf(BufferMsg, "%s0%d",pui8_header,value);
	UART_TX_Str_P(psUART, BufferMsg);
}

void vDashboardSendThreeBytes(uint8_t* pui8_header ,uint8_t value)
{
	/*	Sends Three byte of data with it's header to the dashboard	*/
	int8_t BufferMsg[MaxBufferMsg_Size] = {0};
	if(value > 99)
		sprintf(BufferMsg, "%s%d",pui8_header,value);
	else if(value > 9)
		sprintf(BufferMsg, "%s0%d",pui8_header,value);
	else
		sprintf(BufferMsg, "%s00%d",pui8_header,value);
	UART_TX_Str_P(psUART, BufferMsg);
}

void vDashboardSendControl(uint8_t* pui8_header ,uint8_t Alarmbtn,uint8_t FrontLBtn,uint8_t LeftDirBtn,uint8_t RightDirBtn,uint8_t BrakesBtn)
{
	/*	Sends the buttons controls 5 bytes with it's header to the dashboard	*/
	int8_t BufferMsg[MaxBufferMsg_Size] = {0};
	sprintf(BufferMsg, "%s%d%d%d%d%d",pui8_header,Alarmbtn,FrontLBtn,LeftDirBtn,RightDirBtn,BrakesBtn);
	UART_TX_Str_P(psUART, BufferMsg);
}

