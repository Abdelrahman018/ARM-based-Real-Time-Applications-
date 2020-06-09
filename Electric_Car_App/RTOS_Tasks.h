#ifndef __RTOS_TASKS__
#define __RTOS_TASKS__

#include "stdint.h"
#include "TM4C123GH6PM.h"
#include "inc/tm4c123gh6pm.h"
#include "OtherServices.h"
#include "SPI.h"
#include "PS2JS.h"
#include "ud_datatypes.h"
#include "ADC.h"
#include "Speed_Sensor.h"
#include "DCMotor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Temperature.h"
#include "DashboardCommn.h"
#include "DC.h"
#include "MyUS.h"
#include "mqtt.h"
#include "ExternalInterrupt.h"
#include "PS2JS_Config.h"


typedef enum {
    Topic_US,
    Topic_Speed,
    Topic_MotorTemperature
}TopicIndex_t;

/* Publish Data Structure */
typedef struct {
    uint8_t ui8Header;
    uint8_t ui8DataLen;
    uint8_t pui8Data[MQTT_MAX_DATA_LENGTH];
}PublishData_t;


void 
	vCreate_Tasks				(void);
void 
	prvSetupHardware    (void);
static void 
	prvSetupGPIO        (void);
static void 
	prvSetupIOT         (void);
void 
	vClockConfiguration	(void);

#endif