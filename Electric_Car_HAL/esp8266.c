#include "esp8266.h"


UART0_Type *g_psWIFI_UART;
static char gs_pcWIFI_Buffer[150];
static uint32_t gs_ui32WIFI_CmdSize;
const uint8_t gc_pui8WIFI_ReceiveHeader[] = "+IPD,";
uint8_t g_ui8WIFI_ReceiveHeader_Index;
/*
#define WIFI_UARTprintf(CMD)       gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, CMD);                                \
                                   UART_TX_Data_P (gs_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
*/
/*
static void WIFI_UARTprintf (char *pcStr) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, pcStr);
    UART_TX_Data_P (gs_psWIFI_UART, gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
*/

static uint8_t _ui8HexToNum (uint8_t ui8Hex) {
    uint8_t ui8Num = 0;
    
    if ((ui8Hex >= '0')  &&  (ui8Hex <= '9')) {
        ui8Num = ui8Hex - '0';
    }
    else {
        switch (ui8Hex) {
            case 'A':   case 'a':   ui8Num = 10;    break;
            case 'B':   case 'b':   ui8Num = 11;    break;
            case 'C':   case 'c':   ui8Num = 12;    break;
            case 'D':   case 'd':   ui8Num = 13;    break;
            case 'E':   case 'e':   ui8Num = 14;    break;
            case 'F':   case 'f':   ui8Num = 15;    break;
            default:                                break;
        }
    }
    return ui8Num;
}


void WIFI_Init (UART0_Type *psUART, uint32_t ui32BaudRate) {
    g_psWIFI_UART = psUART;

    UART_Init(g_psWIFI_UART, ui32BaudRate);
}

void WIFI_Test (uint8_t *pui8Buffer) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_Restart (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+RST\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_FirmwareVersion (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+GMR\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_ListAccessPoints (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWLAP\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_JoinAccessPoint (char *pui8SSID, char *pui8Password) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", pui8SSID, pui8Password);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_QuitAccessPoint (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWQAP\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetIPaddress (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIFSR\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetParametersofAccessPoint_UnSaved (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWSAP_CUR?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetParametersofAccessPoint_UnSaved (char *pui8SSID, char *pui8Password, uint8_t ui8ChannelID, uint8_t ui8Encryption) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWSAP_CUR=\"%s\",\"%s\",%i,%i,8\r\n", pui8SSID, pui8Password, ui8ChannelID, ui8Encryption);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetParametersofAccessPoint_Saved (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWSAP_DEF?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetParametersofAccessPoint_Saved (char *pui8SSID, char *pui8Password, uint8_t ui8ChannelID, uint8_t ui8Encryption) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWSAP_DEF=\"%s\",\"%s\",%i,%i,8\r\n", pui8SSID, pui8Password, ui8ChannelID, ui8Encryption);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetWiFiMode_UnSaved (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWMODE_CUR?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetWiFiMode_UnSaved (uint8_t ui8Mode) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWMODE_CUR=%i", ui8Mode);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetWiFiMode_Saved (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWMODE_DEF?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetWiFiMode_Saved (uint8_t ui8Mode) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWMODE_DEF=%i\r\n", ui8Mode);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_EstablishConnection_Single (char *pui8Type, char *pui8IP, uint16_t ui16Port, uint16_t ui16KeepAliveTime) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSTART=\"%s\",\"%s\",%i,%i\r\n", pui8Type, pui8IP, ui16Port, ui16KeepAliveTime);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_EstablishConnection_Multiple (uint8_t ui8ConnectionID, char *pui8Type, char *pui8IP, uint16_t ui16Port, uint16_t ui16KeepAliveTime) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSTART=%i,\"%s\",\"%s\",%i,%i\r\n", ui8ConnectionID, pui8Type, pui8IP, ui16Port, ui16KeepAliveTime);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetMultipleConnectionsStatus (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPMUX?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetMultipleConnectionsStatus (bool bMultipleConnections) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPMUX=%i\r\n", bMultipleConnections);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetJoinedStationsIP (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CWLIF\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetConnectionStatus (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSTATUS\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SendData_NotifyOnly (uint16_t ui16DataLen) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSEND=%i\r\n", ui16DataLen);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SendData_SingleConnection (uint8_t *pui8Data, uint8_t ui8DataLen) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSEND=%i\r\n", ui8DataLen);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
    UART_TX_Data_P (g_psWIFI_UART, pui8Data, ui8DataLen);
}
void WIFI_SendData_MultipleConnections (uint8_t ui8ConnectionID, uint8_t *pui8Data, uint8_t ui8DataLen) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSEND=%i,%i\r\n", ui8ConnectionID, ui8DataLen);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
    UART_TX_Data_P (g_psWIFI_UART, pui8Data, ui8DataLen);
}
void WIFI_CloseConnection_Single (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPCLOSE\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_CloseConnection_Multiple (uint8_t ui8ConnectionID) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPCLOSE=%i\r\n", ui8ConnectionID);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetAsServer (uint16_t ui16Port) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSERVER=1,%i\r\n", ui16Port);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_DeleteServer (uint16_t ui16Port) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSERVER=0,%i\r\n", ui16Port);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetServerTimeout (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSTO?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetServerTimeout (uint16_t ui16ServerTimeOut_Sec) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIPSTO=%i\r\n", ui16ServerTimeOut_Sec);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_BaudRate (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetLocalIP (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIFSR\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_FirmwareUpdate (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+CIUPDATE\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
uint16_t WIFI_ReceiveData_SingleConnection_2 (char *pui8ReceivedPacket, char *pui8Buffer) {
    char *pui8Str = NULL;
    uint16_t ui16DataLen = 0, ui16CopyIndex;
    
    pui8Str = strstr("+IPD,", pui8ReceivedPacket);
    //ASSERT (pui8Str);
    pui8Str += strlen("+IPD,");
    while (*pui8Str != ':') {
        ui16DataLen *= 16;
        ui16DataLen += _ui8HexToNum(*pui8Str);
        ++pui8Str;
    }
    ui16CopyIndex = strlcpy (pui8Buffer, pui8Str, ui16DataLen + strlen("\0"));
    return ui16CopyIndex;
}
void WIFI_ReceiveData_MultipleConnections (uint8_t ui8ConnectionID, char * pui8ReceivedPacket, char *pui8Buffer) {
    
}



void WIFI_EnterDeepSleep (uint32_t ui32SleepTime_ms) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+GSLP=%li\r\n", ui32SleepTime_ms);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_CommandsEchoing (bool bEchoCommandsBack) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+ATE%i\r\n", bEchoCommandsBack);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_RestoreFactoryDefaultSettings (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+RESTORE\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetUARTconfig_UnSaved (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+UART_CUR?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetUARTconfig_UnSaved (uint32_t ui32BaudRate, uint8_t ui8DataBits, uint8_t ui8StopBits, uint8_t ui8ParityBit, uint8_t ui8FlowControl) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+UART_CUR=%li,%i,%i,%i,%i\r\n", ui32BaudRate, ui8DataBits, ui8StopBits, ui8ParityBit, ui8FlowControl);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetUARTconfig_Saved (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+UART_DEF?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetUARTconfig_Saved (uint32_t ui32BaudRate, uint8_t ui8DataBits, uint8_t ui8StopBits, uint8_t ui8ParityBit, uint8_t ui8FlowControl) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+UART_DEF=%li,%i,%i,%i,%i\r\n", ui32BaudRate, ui8DataBits, ui8StopBits, ui8ParityBit, ui8FlowControl);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_GetSleepMode (void) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+SLEEP?\r\n");
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}
void WIFI_SetSleepMode (uint8_t ui8SleepMode) {
    gs_ui32WIFI_CmdSize = sprintf(gs_pcWIFI_Buffer, "AT+SLEEP=%i", ui8SleepMode);
    UART_TX_Data_P (g_psWIFI_UART, (uint8_t *)gs_pcWIFI_Buffer, gs_ui32WIFI_CmdSize);
}


uint16_t WIFI_ReceiveData_SingleConnection (uint8_t *pui8Buffer) {
    uint8_t ui8Data;
    uint16_t ui16DataLen = 0, ui16Index;
    g_ui8WIFI_ReceiveHeader_Index = 1;  /* supposing the start of the header is matched */

    /* Skip till the start of receiving header is matched */
    while ( (ui8Data = UART_RX_Byte_P(g_psWIFI_UART))  !=  gc_pui8WIFI_ReceiveHeader[0] )
        ;
    /* This loop till the receiving header is "completely" matched */
    while ( g_ui8WIFI_ReceiveHeader_Index  <  (sizeof(gc_pui8WIFI_ReceiveHeader) - 1) ) {
        ui8Data = UART_RX_Byte_P(g_psWIFI_UART);
        /* if the byte match in the receiving header */
        if ( ui8Data  ==  gc_pui8WIFI_ReceiveHeader[g_ui8WIFI_ReceiveHeader_Index] ) {
            ++g_ui8WIFI_ReceiveHeader_Index;
        }
        /* if the byte "doesn't" match in the receiving header  =>  restart matching process */
        else {
            g_ui8WIFI_ReceiveHeader_Index = 0;
        }
    }
    /* Extract the "Data Length" */
    while ( (ui8Data = UART_RX_Byte_P(g_psWIFI_UART))  !=  ':' ) {
        ui16DataLen *= 10;
        ui16DataLen += ui8Data - '0';
    }

    /* if a buffer is provided  =>  fill it with the data */
    if ( pui8Buffer  !=  NULL ) {
        /* Receive the data */
        for (ui16Index = 0; ui16Index < ui16DataLen; ++ui16Index) {
            pui8Buffer[ui16Index] = UART_RX_Byte_P(g_psWIFI_UART);
        }
    }

    /* Return the number of received bytes */
    return ui16Index;
}
