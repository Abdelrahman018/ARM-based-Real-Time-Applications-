#ifndef __MQTT_H__
#define __MQTT_H__


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "esp8266.h"

#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123GH6PM.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */



#define MQTT_CONNECT_KEEP_ALIVE_SEC     60
#define MQTT_MAX_DATA_LENGTH            6
#define MQTT_MAX_TOPIC_LENGTH           18
#define MQTT_NO_OF_TOPICS               3
#define MQTT_TOPIC_MATCH_NO_TOPIC       0xFF

#define CONNECT_RESPONSE_OK                 0x00
#define CONNECT_RESPONSE_PROTOCOL_VERSION   0x01
#define CONNECT_RESPONSE_IDENTIFIER         0x02
#define CONNECT_RESPONSE_SERVER_UNAVAILABLE 0x03
#define CONNECT_RESPONSE_USER_PASSWORD      0x04
#define CONNECT_RESPONSE_NOT_AUTHORIZED     0x05

#define CONNECT_RESPONSE_WRONG_PACKET       0x06


typedef struct {
    uint8_t RESERVED        : 1;
    uint8_t CleanSession    : 1;
    uint8_t Will_Flag       : 1;
    uint8_t Will_QoS        : 2;
    uint8_t Will_Retain     : 1;
    uint8_t Password        : 1;
    uint8_t UserName        : 1;
}MQTT_ConnectFlags_t;

extern UART0_Type *g_psWIFI_UART;

uint32_t ui32MQTT_Connect_Prepare (char *pcClientID, char *pcUsername, char *pcPassword);
void vMQTT_Connect_Send (void);
uint32_t ui32MQTT_Subscribe_Prepare (char *pcTopic, uint8_t ui8QoS_Max);
void vMQTT_Subscribe_Send (void);
uint32_t ui32MQTT_Publish_Prepare (char *pcTopic, uint8_t ui8MsgSize, uint8_t ui8Duplicate, uint8_t ui8QoS, uint8_t ui8Retain);
void vMQTT_Publish_Send (uint8_t *pui8Msg, uint8_t ui8MsgSize);

uint32_t ui32MQTT_Receive (uint8_t *pui8ReceivedPacket, char *pui8Buffer_Topic, uint8_t *pui8Buffer_Data, uint32_t *pui32DataStartIndexInReceivedPacket);
uint8_t ui8MQTT_TopicMAtch (char ppcTopicList[][MQTT_MAX_TOPIC_LENGTH], char *pcTopicName, uint8_t ui8TopicsListLen);

uint8_t vMQTT_Connect_Response_2 (void);


#endif  // __MQTT_H__
