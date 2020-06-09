#include "mqtt.h"


static uint8_t gs_pui8MQTT_Buffer[200];
static uint16_t gs_ui16MQTT_BufferIndex;
static char const gsc_pcProtocolName[] = "MQTT";
static uint32_t gs_ui32RemLen;
static uint8_t gs_ui8RemLen_Size;
static uint16_t gs_ui16PacketID = 10;

uint32_t ui32MQTT_Connect_Prepare (char *pcClientID, char *pcUserName, char *pcPassword) {
    uint32_t ui32Temp;
    uint8_t ui8Index;
    gs_ui16MQTT_BufferIndex = 0;
    gs_ui32RemLen = 0;
    gs_ui8RemLen_Size = 0;
    MQTT_ConnectFlags_t xConnectFlags = {0};

    /* Prepare the "Fixed Header" */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = 0x10;      /* Connect + "No Flags" */
    ++gs_ui16MQTT_BufferIndex;
    gs_ui16MQTT_BufferIndex += 4;                            /* wait till the remaining length is calculated */

    /* Prepare the "Variable Header" */
    /* Protocol Length */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = strlen(gsc_pcProtocolName) >> 8;
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = strlen(gsc_pcProtocolName);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Protocol Name */
    strcpy((char *)(gs_pui8MQTT_Buffer + gs_ui16MQTT_BufferIndex), gsc_pcProtocolName);
    gs_ui16MQTT_BufferIndex += strlen(gsc_pcProtocolName);
    gs_ui32RemLen += strlen(gsc_pcProtocolName);
    /* Protocol Level */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = 0x04;      /* MQTT-3.1.1 */
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Connect Flags */
    xConnectFlags .CleanSession = 1;
    if   ( pcUserName  !=  NULL )                                   { xConnectFlags .UserName = 1; }
    if ( ( pcPassword  !=  NULL )  &&  ( pcUserName  !=  NULL ) )   { xConnectFlags .Password = 1; }
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = *(  (uint8_t *) &xConnectFlags  );
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Keep Alive Time  in Seconds */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (MQTT_CONNECT_KEEP_ALIVE_SEC >> 8);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) MQTT_CONNECT_KEEP_ALIVE_SEC;
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;

    /* Prepare the "Payload" */
    /* Client ID Length */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (strlen(pcClientID) >> 8);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) strlen(pcClientID);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Client ID */
    strcpy((char *)(gs_pui8MQTT_Buffer + gs_ui16MQTT_BufferIndex), pcClientID);
    gs_ui16MQTT_BufferIndex += strlen(pcClientID);
    gs_ui32RemLen += strlen(pcClientID);
    /* User Name */
    if ( pcUserName  !=  NULL ) {
        /* User Name Length */
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (strlen(pcUserName) >> 8);
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) strlen(pcUserName);
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        /* User Name */
        strcpy((char *)(gs_pui8MQTT_Buffer + gs_ui16MQTT_BufferIndex), pcUserName);
        gs_ui16MQTT_BufferIndex += strlen(pcUserName);
        gs_ui32RemLen += strlen(pcUserName);
    }
    /* Password */
    if ( pcPassword  !=  NULL ) {
        /* Password Length */
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (strlen(pcPassword) >> 8);
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) strlen(pcPassword);
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        /* Password */
        strcpy((char *)(gs_pui8MQTT_Buffer + gs_ui16MQTT_BufferIndex), pcPassword);
        gs_ui16MQTT_BufferIndex += strlen(pcPassword);
        gs_ui32RemLen += strlen(pcPassword);
    }
    /* if there is a user name but no password */
    else if ( pcUserName  !=  NULL ) {
        /* Password Length */
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = 0;
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = 0;
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
    }

    /* Prepare the "Remaining Length" */
    if      (gs_ui32RemLen <= 0x7F)         { gs_ui8RemLen_Size = 1; }
    else if (gs_ui32RemLen <= 0x3FFF)       { gs_ui8RemLen_Size = 2; }
    else if (gs_ui32RemLen <= 0x1FFFFF)     { gs_ui8RemLen_Size = 3; }
    else                                    { gs_ui8RemLen_Size = 4; }
    for (ui8Index = gs_ui8RemLen_Size, ui32Temp = gs_ui32RemLen; ui8Index > 0; --ui8Index) {
        gs_pui8MQTT_Buffer[1 + 4 - ui8Index] = ui32Temp & 0x7F;  /* 1: Fixed Header,     4: Max Length */
        if (gs_ui8RemLen_Size > 1)
            gs_pui8MQTT_Buffer[1 + 4 - ui8Index] |= (1 << 7);
        ui32Temp >>= 7;
    }

    /* Replace the "Fixed Header" in its correct place */
    //             MaxLength
    gs_pui8MQTT_Buffer[4 - gs_ui8RemLen_Size] = gs_pui8MQTT_Buffer[0];

    /* Return the whole "Packet Size" */
    return ( 1 + gs_ui8RemLen_Size + gs_ui32RemLen );       /* 1: Fixed Header */
}
void vMQTT_Connect_Send (void) {
    /* Send The Packet */
    UART_TX_Data_P(g_psWIFI_UART,
                   gs_pui8MQTT_Buffer + (4 - gs_ui8RemLen_Size),    /* address of the first byte: "Fixed Header" */
                   1 + gs_ui8RemLen_Size + gs_ui32RemLen);          /* 1: Fixed Header */
}

uint32_t ui32MQTT_Subscribe_Prepare (char *pcTopic, uint8_t ui8QoS_Max) {
    uint32_t ui32Temp;
    uint8_t ui8Index;
    gs_ui16MQTT_BufferIndex = 0;
    gs_ui32RemLen = 0;
    gs_ui8RemLen_Size = 0;

    /* Prepare the "Fixed Header" */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = 0x82;      /* Subscribe: 8 + "No Flags": Reserved(2) */
    ++gs_ui16MQTT_BufferIndex;
    gs_ui16MQTT_BufferIndex += 4;                            /* wait till the remaining length is calculated */

    /* Prepare the "Variable Header" */
    /* Packet ID */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (gs_ui16PacketID >> 8);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) gs_ui16PacketID;
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Increment Packet ID */
    ++gs_ui16PacketID;

    /* Prepare the "Payload" */
    /* Topic Length */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (strlen(pcTopic) >> 8);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) strlen(pcTopic);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Topic Name */
    strcpy((char *)(gs_pui8MQTT_Buffer + gs_ui16MQTT_BufferIndex), pcTopic);
    gs_ui16MQTT_BufferIndex += strlen(pcTopic);
    gs_ui32RemLen += strlen(pcTopic);
    /* Requested QoS */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = ui8QoS_Max;
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;

    /* Prepare the "Remaining Length" */
    if      (gs_ui32RemLen <= 0x7F)         { gs_ui8RemLen_Size = 1; }
    else if (gs_ui32RemLen <= 0x3FFF)       { gs_ui8RemLen_Size = 2; }
    else if (gs_ui32RemLen <= 0x1FFFFF)     { gs_ui8RemLen_Size = 3; }
    else                                    { gs_ui8RemLen_Size = 4; }
    for (ui8Index = gs_ui8RemLen_Size, ui32Temp = gs_ui32RemLen; ui8Index > 0; --ui8Index) {
        gs_pui8MQTT_Buffer[1 + 4 - ui8Index] = ui32Temp & 0x7F;  /* 1: Fixed Header,     4: Max Length */
        if (gs_ui8RemLen_Size > 1)
            gs_pui8MQTT_Buffer[1 + 4 - ui8Index] |= (1 << 7);
        ui32Temp >>= 7;
    }

    /* Replace the "Fixed Header" in its correct place */
    //             MaxLength
    gs_pui8MQTT_Buffer[4 - gs_ui8RemLen_Size] = gs_pui8MQTT_Buffer[0];

    /* Return the whole "Packet Size" */
    return ( 1 + gs_ui8RemLen_Size + gs_ui32RemLen );       /* 1: Fixed Header */
}
void vMQTT_Subscribe_Send (void) {
    /* Send The Packet */
    UART_TX_Data_P(g_psWIFI_UART,
                   gs_pui8MQTT_Buffer + (4 - gs_ui8RemLen_Size),    /* address of the first byte: "Fixed Header" */
                   1 + gs_ui8RemLen_Size + gs_ui32RemLen);          /* 1: Fixed Header */
}

uint32_t ui32MQTT_Publish_Prepare (char *pcTopic, uint8_t ui8MsgSize, uint8_t ui8Duplicate, uint8_t ui8QoS, uint8_t ui8Retain) {
    uint32_t ui32Temp;
    uint8_t ui8Index;
    gs_ui16MQTT_BufferIndex = 0;
    gs_ui32RemLen = 0;
    gs_ui8RemLen_Size = 0;

    /* Prepare the "Fixed Header" */
    //                                           PUBLISH      DUPLICATE             QoS             RETAIN
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (3 << 4) | (ui8Duplicate << 3) | (ui8QoS << 1) | (ui8Retain << 0);
    ++gs_ui16MQTT_BufferIndex;
    gs_ui16MQTT_BufferIndex += 4;                            /* wait till the remaining length is calculated */

    /* Prepare the "Variable Header" */
    /* Topic Length */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (strlen(pcTopic) >> 8);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) strlen(pcTopic);
    ++gs_ui16MQTT_BufferIndex;
    ++gs_ui32RemLen;
    /* Topic Name */
    strcpy((char *)(gs_pui8MQTT_Buffer + gs_ui16MQTT_BufferIndex), pcTopic);
    gs_ui16MQTT_BufferIndex += strlen(pcTopic);
    gs_ui32RemLen += strlen(pcTopic);
    if (ui8QoS > 0) {
        /* Packet ID */
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) (gs_ui16PacketID >> 8);
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = (uint8_t) gs_ui16PacketID;
        ++gs_ui16MQTT_BufferIndex;
        ++gs_ui32RemLen;
        /* Increment Packet ID */
        ++gs_ui16PacketID;
    }

    /* Prepare the "Payload" */
    gs_ui16MQTT_BufferIndex += ui8MsgSize;
    gs_ui32RemLen += ui8MsgSize;

    /* Prepare the "Remaining Length" */
    if      (gs_ui32RemLen <= 0x7F)         { gs_ui8RemLen_Size = 1; }
    else if (gs_ui32RemLen <= 0x3FFF)       { gs_ui8RemLen_Size = 2; }
    else if (gs_ui32RemLen <= 0x1FFFFF)     { gs_ui8RemLen_Size = 3; }
    else                                    { gs_ui8RemLen_Size = 4; }
    for (ui8Index = gs_ui8RemLen_Size, ui32Temp = gs_ui32RemLen; ui8Index > 0; --ui8Index) {
        gs_pui8MQTT_Buffer[1 + 4 - ui8Index] = ui32Temp & 0x7F; /* 1: Fixed Header,     4: Max Length */
        if (gs_ui8RemLen_Size > 1)
            gs_pui8MQTT_Buffer[1 + 4 - ui8Index] |= (1 << 7);
        ui32Temp >>= 7;
    }

    /* Replace the "Fixed Header" in its correct place */
    //             MaxLength
    gs_pui8MQTT_Buffer[4 - gs_ui8RemLen_Size] = gs_pui8MQTT_Buffer[0];

    /* Return the whole "Packet Size" */
    return ( 1 + gs_ui8RemLen_Size + gs_ui32RemLen );       /* 1: Fixed Header */
}
void vMQTT_Publish_Send (uint8_t *pui8Msg, uint8_t ui8MsgSize) {
    /* Send The Packet - "Pay-load Data" */
    UART_TX_Data_P(g_psWIFI_UART,
                   gs_pui8MQTT_Buffer + (4 - gs_ui8RemLen_Size),        /* address of the first byte: "Fixed Header" */
                   1 + gs_ui8RemLen_Size + gs_ui32RemLen - ui8MsgSize); /* 1: Fixed Header */
    /* Send The "Pay-load Data" */
    UART_TX_Data_P(g_psWIFI_UART,
                   pui8Msg,
                   ui8MsgSize);
}


uint32_t ui32MQTT_Receive (uint8_t *pui8ReceivedPacket, char *pui8Buffer_Topic, uint8_t *pui8Buffer_Data, uint32_t *pui32DataStartIndexInReceivedPacket) {
    uint32_t ui32TopicNameLen, ui32Index;
    uint32_t ui32RemLen = 0, ui32ReceivedPacketIndex = 0;

    /* Assert that it's a "Publish" Packet */
    if ( pui8ReceivedPacket[0]  !=  0x30 ) {
        return 0;
    }

    /* Extract the "Remaining Length" */
    do {
        ++ui32ReceivedPacketIndex;
        ui32RemLen <<= 7;
        ui32RemLen += pui8ReceivedPacket[ui32ReceivedPacketIndex] & 0x7F;
    }
    while ( pui8ReceivedPacket[ui32ReceivedPacketIndex]  &  (1 << 7) );
    ++ui32ReceivedPacketIndex;

    /* Extract "Topic Name" Length */
    ui32TopicNameLen = 0;
    ui32TopicNameLen += pui8ReceivedPacket[ui32ReceivedPacketIndex];
    ++ui32ReceivedPacketIndex;
    --ui32RemLen;
    ui32TopicNameLen <<= 8;
    ui32TopicNameLen += pui8ReceivedPacket[ui32ReceivedPacketIndex];
    ++ui32ReceivedPacketIndex;
    --ui32RemLen;

    /* if a "Topic Name Buffer" is provided  =>  fill it with the topic name */
    if ( pui8Buffer_Topic  !=  NULL ) {
        /* Extract "Topic Name" */
        for (ui32Index = 0; ui32Index < ui32TopicNameLen; ++ui32Index) {
            pui8Buffer_Topic[ui32Index] = pui8ReceivedPacket[ui32ReceivedPacketIndex];
            ++ui32ReceivedPacketIndex;
        }
        ui32RemLen -= ui32TopicNameLen;
        pui8Buffer_Topic[ui32Index] = '\0';
    }
    else {
        ui32ReceivedPacketIndex += ui32TopicNameLen;
        ui32RemLen -= ui32TopicNameLen;
    }

    /* if an "Index Buffer" is provided  =>  fill it with the data start index */
    if ( pui32DataStartIndexInReceivedPacket  !=  NULL ) {
        /* Indicate the start index of data in original packet */
        *pui32DataStartIndexInReceivedPacket = ui32ReceivedPacketIndex;
    }

    /* Now: ui32RemLen  =>  Data Length */

    /* if a "Data Buffer" is provided  =>  fill it with the data */
    if ( pui8Buffer_Data  !=  NULL ) {
        /* Extract "Pay-load Data" */
        for (ui32Index = 0; ui32Index < ui32RemLen; ++ui32Index) {
            pui8Buffer_Data[ui32Index] = pui8ReceivedPacket[ui32ReceivedPacketIndex];
            ++ui32ReceivedPacketIndex;
        }
    }
    else {
        ui32ReceivedPacketIndex += ui32RemLen;
    }

    /* return Data Length */
    return ui32RemLen;
}

uint8_t ui8MQTT_TopicMAtch (char ppcTopicList[][MQTT_MAX_TOPIC_LENGTH], char *pcTopicName, uint8_t ui8TopicsListLen) {
    uint8_t ui8Index;

    for (ui8Index = 0; ui8Index <= ui8TopicsListLen; ++ui8Index) {
        if ( strcmp(ppcTopicList[ui8Index], pcTopicName)  !=  0 ) {
            break;
        }
    }
    if (ui8Index > ui8TopicsListLen) {
        ui8Index = MQTT_TOPIC_MATCH_NO_TOPIC;
    }

    return ui8Index;
}

uint8_t ui8MQTT_Connect_Response_2 (void) {
    uint8_t ui8Return = 0;
    gs_ui16MQTT_BufferIndex = 0;

    /* Receive the "Fixed Header" */
    /* Control Packet */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = UART_RX_Byte_P(g_psWIFI_UART);
    ++gs_ui16MQTT_BufferIndex;
    if (gs_pui8MQTT_Buffer[0] != 0x20)  { ui8Return = CONNECT_RESPONSE_WRONG_PACKET; }
    /* Remaining Length */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = UART_RX_Byte_P(g_psWIFI_UART);
    ++gs_ui16MQTT_BufferIndex;

    /* Receive the "Variable Header" */
    /* "Session Present" flag */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = UART_RX_Byte_P(g_psWIFI_UART);
    ++gs_ui16MQTT_BufferIndex;
    /* Return Code */
    gs_pui8MQTT_Buffer[gs_ui16MQTT_BufferIndex] = UART_RX_Byte_P(g_psWIFI_UART);
    ++gs_ui16MQTT_BufferIndex;

    /* if "CONNACK" packet => return the "Return Code" */
    if (ui8Return == 0)    { ui8Return = gs_pui8MQTT_Buffer[3]; }

    return ui8Return;
}
