#ifndef __ESP8622_H__
#define __ESP8622_H__


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"

#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123GH6PM.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */

#define WIFI_TCP_KEEP_ALIVE_MAX     7200


//void WIFI_UARTprintf (char *pcStr);

void WIFI_Init (UART0_Type *psUART, uint32_t ui32BaudRate);
void WIFI_Test (uint8_t *pui8Buffer);
void WIFI_Restart (void);
void WIFI_FirmwareVersion (void);
void WIFI_ListAccessPoints (void);
void WIFI_JoinAccessPoint (char *pui8SSID, char *pui8Password);
void WIFI_QuitAccessPoint (void);
void WIFI_GetIPaddress (void);
void WIFI_GetParametersofAccessPoint_UnSaved (void);
void WIFI_SetParametersofAccessPoint_UnSaved (char *pui8SSID, char *pui8Password, uint8_t ui8ChannelID, uint8_t ui8Encryption);
void WIFI_GetParametersofAccessPoint_Saved (void);
void WIFI_SetParametersofAccessPoint_Saved (char *pui8SSID, char *pui8Password, uint8_t ui8ChannelID, uint8_t ui8Encryption);
void WIFI_GetWiFiMode_UnSaved (void);
void WIFI_SetWiFiMode_UnSaved (uint8_t ui8Mode);
void WIFI_GetWiFiMode_Saved (void);
void WIFI_SetWiFiMode_Saved (uint8_t ui8Mode);
void WIFI_EstablishConnection_Single (char *pui8Type, char *pui8IP, uint16_t ui16Port, uint16_t ui16KeepAliveTime);
void WIFI_EstablishConnection_Multiple (uint8_t ui8ConnectionID, char *pui8Type, char *pui8IP, uint16_t ui16Port, uint16_t ui16KeepAliveTime);
void WIFI_GetMultipleConnectionsStatus (void);
void WIFI_SetMultipleConnectionsStatus (bool bMultipleConnections);
void WIFI_GetJoinedStationsIP (void);
void WIFI_GetConnectionStatus (void);
void WIFI_SendData_NotifyOnly (uint16_t ui16DataLen);
void WIFI_SendData_SingleConnection (uint8_t *pui8Data, uint8_t ui8DataLen);
void WIFI_SendData_MultipleConnections (uint8_t ui8ConnectionID, uint8_t *pui8Data, uint8_t ui8DataLen);
void WIFI_CloseConnection_Single (void);
void WIFI_CloseConnection_Multiple (uint8_t ui8ConnectionID);
void WIFI_SetAsServer (uint16_t ui16Port);
void WIFI_DeleteServer (uint16_t ui16Port);
void WIFI_GetServerTimeout (void);
void WIFI_SetServerTimeout (uint16_t ui16ServerTimeOut_Sec);
void WIFI_BaudRate (void);
void WIFI_GetLocalIP (void);
void WIFI_FirmwareUpdate (void);
uint16_t WIFI_ReceiveData_SingleConnection (uint8_t *pui8Buffer);
uint16_t WIFI_ReceiveData_SingleConnection_2 (char * pui8ReceivedPacket, char *pui8Buffer);
void WIFI_ReceiveData_MultipleConnections (uint8_t ui8ConnectionID, char * pui8ReceivedPacket, char *pui8Buffer);



void WIFI_EnterDeepSleep (uint32_t ui32SleepTime_ms);
void WIFI_CommandsEchoing (bool bEchoCommandsBack);
void WIFI_RestoreFactoryDefaultSettings (void);
void WIFI_GetUARTconfig_UnSaved (void);
void WIFI_SetUARTconfig_UnSaved (uint32_t ui32BaudRate, uint8_t ui8DataBits, uint8_t ui8StopBits, uint8_t ui8ParityBit, uint8_t ui8FlowControl);
void WIFI_GetUARTconfig_Saved (void);
void WIFI_SetUARTconfig_Saved (uint32_t ui32BaudRate, uint8_t ui8DataBits, uint8_t ui8StopBits, uint8_t ui8ParityBit, uint8_t ui8FlowControl);
void WIFI_GetSleepMode (void);
void WIFI_SetSleepMode (uint8_t ui8SleepMode);


#endif  // __ESP8622_H__
