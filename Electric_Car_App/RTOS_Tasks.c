#include "RTOS_Tasks.h"

/* Task Handlers */
TaskHandle_t xTaskHandle_Speedometer;
TaskHandle_t xTaskHandle_MotorControl;
TaskHandle_t xTaskHandle_MainController;
TaskHandle_t xTaskHandle_SensorsReadings;
TaskHandle_t xTaskHandle_UltrasonicSensors;
TaskHandle_t xTaskHandle_MQTT_Publish;

/* Queue Handlers */
QueueHandle_t xQueueHandle_MQTT_Publish;
QueueHandle_t xQueueHandle_Target_Speed;
QueueHandle_t xQueueHandle_Current_Speed;
QueueHandle_t xQueueHandle_Brake;

/* Semaphore Handlers */
xSemaphoreHandle		mxDashboard;

/* Task functions prototypes */
void vSpeedometerTask_Handler 		(void *parameters);
void vMotorControlTask_Handler 		(void *parameters);
void vMainControllerTask_Handler 	(void *parameters);
void SensorsReadings_Handler 			(void *parameters);
void UltrasonicSensorsTask_Handler(void *parameters);
void vMQTT_PublishTask_Handler  	(void *parameters);

/* Global variables */
bool EmergencyFlag = false;
static char gs_pcBufferMsg[60];		/* Experimental */
static uint8_t gs_pui8Buffer_WIFI_RX[400];
static uint32_t gs_ui32Index_MQTT_DataStartIndexInWIFIBuffer;
static char gs_pcBuffer_MQTT_RX_Topic[MQTT_MAX_TOPIC_LENGTH];
char g_ppcMQTT_TopicsList[][MQTT_MAX_TOPIC_LENGTH] = {
                                                                 "US",
                                                                 "E-Car/Speed",
                                                                 "MotorTemperature"
};

/******************* Ultrasonic Part ***********************/

uint32_t Dist_R=0;
uint32_t Dist_L=0;
uint32_t Dist_F=0;
uint32_t Dist_B=0;

uint32_t S;       // index to the current state
uint32_t inputs; 	// U(R)U(L)U(F)U(B)
uint32_t Output;

struct State {
  unsigned long out;        // 5-bits output (4 for motors direction and one for speed)
  unsigned long next[16];};  // Next if 4-bits input is 0-15
typedef const struct State StateType;
StateType fsm[7]={
  {0,  			{ Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}},  // Idle (do nothing)--> control from joystick
  {0b0001, 	{ Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF	,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}},  // turnR
  {0b0100,  { Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF	,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}},  //turnL
	{0b1010,  { Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF	,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}},  //Backword
	{0b0000,  { Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF	,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}},  //Braking
	{0b0101   & 1,  { Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF	,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}},  //speedUpF
	{0b0101   & 0, { Idle, speedUpF,Braking,turnR,turnR,turnR,speedDownF	,turnR,turnL,turnL,speedDownF,turnL,speedUpF,speedUpF,Backword,Braking}}  //speedDownF
};
	



void Car_output(uint32_t out){
	if (out ==0){ // idle case
		// cotrol from joystick
		
	}
	else if(out == 0b0101){ // speed up
		// speed up Forward
		// speedUpF(speedValue);
		// OR 
		// apply 0101 to motor driver pins then speed up 
		DC_Stop();
		//xSemaphoreGive(bsMotorcontrol);

		

	}
	else if (out == 0b0100) { //speed down
		// speed down Forward
		// speedDownF(speedValue);
		// OR 
		// apply 0101 to motor driver pins then speed down 
		DC_Stop();
		

	}
	else{ // others
		
		// apply out to motor driver pin directly
		// EX:
		//out = 0b0100 --> turn left
		DC_Stop();
	}
}


/************************************************************/

void vCreate_Tasks(void)
{
		
/* Create "Binary Semaphores" */
		mxDashboard = xSemaphoreCreateMutex();
	
		xTaskCreate(vMQTT_PublishTask_Handler,
                "MQTT_Publish-Task",
                500,
                NULL,
                0,
                &xTaskHandle_MQTT_Publish);
								QueueHandle_t xQueueHandle_Direction;
		xTaskCreate(SensorsReadings_Handler,
                "SR_Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                &xTaskHandle_SensorsReadings);
		xTaskCreate(vSpeedometerTask_Handler,
                "Sp_Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                5,
                &xTaskHandle_Speedometer);
		xTaskCreate(vMainControllerTask_Handler,
                "JS_Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                3,
                &xTaskHandle_MainController);
		xTaskCreate(vMotorControlTask_Handler,
                "MS_Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                1,
                &xTaskHandle_MotorControl);
		xTaskCreate(UltrasonicSensorsTask_Handler,
                "US_Task",
                configMINIMAL_STACK_SIZE,
                NULL,
                4,
                &xTaskHandle_UltrasonicSensors);

		/* Create "Queues" */
    xQueueHandle_MQTT_Publish = xQueueCreate	( 10, sizeof(PublishData_t) );
		xQueueHandle_Target_Speed = xQueueCreate	( 1, sizeof(uint8_t));
		xQueueHandle_Brake = xQueueCreate					( 1, sizeof(uint8_t));
		xQueueHandle_Current_Speed = xQueueCreate	( 1, sizeof(uint8_t));

}

void
vMQTT_PublishTask_Handler (void *parameters) {
    PublishData_t xPublishData;
    uint32_t ui32PacketLen;

	/* Connect to the server */
    ui32PacketLen = ui32MQTT_Connect_Prepare("E-Car: Main MCU", "75WCEAaJ4MFYhTJRZjDONQTHbgSUXnsSickLCikvSNjZuREh08VBOD8OJQyu1Qca", NULL);
    WIFI_SendData_NotifyOnly((uint16_t)ui32PacketLen);
    vTaskDelay( pdMS_TO_TICKS( 100 ) );
    vMQTT_Connect_Send();
    vTaskDelay( pdMS_TO_TICKS( 5000 ) );

    while (1) {
        /*  */
        xQueueReceive(xQueueHandle_MQTT_Publish, &xPublishData, portMAX_DELAY);
        /* Publish some data to a specific topic */
        ui32PacketLen = ui32MQTT_Publish_Prepare("E-Car/Speed", xPublishData .ui8DataLen, 0, 0, 1);
        WIFI_SendData_NotifyOnly((uint16_t)ui32PacketLen);
        vTaskDelay( pdMS_TO_TICKS( 50 ) );
        vMQTT_Publish_Send(xPublishData .pui8Data, xPublishData .ui8DataLen);
				vTaskDelay( pdMS_TO_TICKS( 200 ) );
    }
}


void UltrasonicSensorsTask_Handler(void *parameters)
{
	uint8_t UltrasonicsValY = 0, UltrasonicsValX = 0;
	DC_init();
	EN_PORT_CLK(GPIOD);
	EN_PORT_CLK(GPIOB);
	vUltrasonicInit(GPIOD,US_PIN0, US_PIN1);//TrigPin-->PD0 & ECO-->PD1 ----> Rigt
	vUltrasonicInit(GPIOD,US_PIN7, US_PIN6);//TrigPin-->PD7 & ECO-->PD6 ----> left
	vUltrasonicInit(GPIOB,US_PIN2, US_PIN3);//TrigPin-->PB2 & ECO-->PB3 -----> forward
	vUltrasonicInit(GPIOB,US_PIN6, US_PIN7);//TrigPin-->PB6 & ECO-->PB7 -----> backward
	S = Idle; //initial state is idle
	while (1)
	{
	/*
	 Task one  	(US response handling)	
		 1) get US readings
		 2) send readings to dashboard (put the data in the queue)--->Task 9
	   3) Actions (in case of emergency conditions) ---> you must pause the main movement control task (Task 1 )  		
*/
		// U(R)U(L)U(F)U(B)
		
			
		Dist_B = ui32_UltrasonicGetDistance(GPIOB,US_PIN6, US_PIN7);//-----> backward
		
		
		if(Dist_B <= 15){  /// boundary (emergency condition)
			inputs |= 1<<0; //inputs = 0001
			UltrasonicsValY = 2;
		}
		else if(Dist_B <= 100)
		{
			inputs |= 0<<0; //inputs = 0000
			UltrasonicsValY = 1;
		}
		else{
			inputs |= 0<<0; //inputs = 0000
			UltrasonicsValY = 0;
		}
		vTaskDelay( pdMS_TO_TICKS(30));
		
		Dist_F = ui32_UltrasonicGetDistance(GPIOB,US_PIN2, US_PIN3);//-----> forward
		if(Dist_F <= 15){  /// boundary (emergency condition)
			inputs |= 1<<1; 
			UltrasonicsValY += 20;
		}
		else if(Dist_F <= 100)
		{
			inputs |= 0<<1; 
			UltrasonicsValY += 10;
		}
		else{
			inputs |= 0<<1; 
			UltrasonicsValY += 0;
		}
		if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
		{
			vDashboardSendTwoBytes(UltrasonicY , UltrasonicsValY);
			xSemaphoreGive(mxDashboard);
		}
		vTaskDelay( pdMS_TO_TICKS(30));
		
		Dist_L = ui32_UltrasonicGetDistance(GPIOD,US_PIN7, US_PIN6);//----> left
		if(Dist_L <= 15){  /// boundary (emergency condition)
			inputs |= 1<<2; 
			UltrasonicsValX = 2;
		}
		else if(Dist_L <= 100)
		{
			inputs |= 0<<2; 
			UltrasonicsValX = 1;
		}
		else{
			inputs |= 0<<2; 
			UltrasonicsValX = 0;
		}
		vTaskDelay( pdMS_TO_TICKS(30));

		Dist_R = ui32_UltrasonicGetDistance(GPIOD,US_PIN0, US_PIN1);//----> Right
		if(Dist_R <= 15){  /// boundary (emergency condition)
			inputs |= 1<<3; 
			UltrasonicsValX += 20;
		}
		else if(Dist_R <= 100)
		{
			UltrasonicsValX += 10;
		}
		else{
			inputs |= 0<<3; 
			UltrasonicsValX += 0;
		}
		vTaskDelay( pdMS_TO_TICKS(30));
		if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
		{
			vDashboardSendTwoBytes(UltrasonicX , UltrasonicsValX);
			xSemaphoreGive(mxDashboard);
		}
    S = fsm[S].next[inputs];   	// next depends on input only 
		Output = fsm[S].out;      	// set output from FSM
    Car_output(Output);     		// do output to two motors
		vTaskDelay( pdMS_TO_TICKS(30));		
	 }
}


void vSpeedometerTask_Handler (void *parameters) {
	uint8_t Radius_cm 			= 5 	, 
					intervalTime_ms = 100 ,
					SlotsNo 				= 7		,
					state 					= 1		,
					avgspeed 				= 0		,
					speed 					= 0		;
	uint8_t* TempData = NULL;
	PublishData_t xPublishData;
  xPublishData .ui8Header  = Topic_Speed;
  xPublishData .ui8DataLen = 2;
	
	while(1)
	{
		switch(state)
		{
			case 1:
				/*	
				* First state of Speedometer Task
				*	Get the speed in meter per seconds
				* Map speed values to (0 - 90)
				*	Get the average speed of 20 consecutive speed values
				* Move to Next state
				*/
				speed = (ui32GetSpeed_M_S( Radius_cm ,SlotsNo, intervalTime_ms ));
				speed = ui32map_value(speed,0,120,0,90);
				xQueueSend(xQueueHandle_Current_Speed, &speed, 1);
				avgspeed=ui32speedAvg(speed ,20);																			// Get the average speed of 20 consecutive speed values to smooth the output
				state = 2;
				break;
			case 2:
				/*	
				* Second state of Speedometer Task
				*	Make sure that the speed given to Dashboardnever exceeds the limit of 90
				* Put speed in Current_Speed_Queue 
				* Put avgspeed in MQTT_Publish_Queue so it can be published to the server
				* Send Speed to Dashboard
				* Move to First state
				*/
				if (avgspeed > 90) avgspeed = 90;
				TempData = itoaTwoNum(avgspeed);
				xPublishData.pui8Data[0] = TempData[0];								
				xPublishData.pui8Data[1] = TempData[1];								
				xQueueSend(xQueueHandle_MQTT_Publish, &xPublishData, 1);
				if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
				{
					vDashboardSendTwoBytes(CurrentSpeed,avgspeed);
					xSemaphoreGive(mxDashboard);
				}
				state = 1;
				break;	
			default:
				break;
		}
		vTaskDelay( pdMS_TO_TICKS( intervalTime_ms/2 ) ); // Delay Time is Divided by two because it takes the task two steps to repeat itself
	}
}





void vMainControllerTask_Handler (void *parameters) {
	
	SSI_Init( SSI0_BASE , PS2JS_Operating_Freq , SPI_MODE_3 , PS2JS_Databits_Num , PS2JS_Bits_Order );
	PS2_Config_Analog_Mode(SSI0_BASE ,GPIOA , PS2JS_Attention_Pin );
	PS2_data_t PS2JSData;
	uint8_t DirectionPS2JS = 0 ;
	uint8_t Target_speedPS2JS = 0;
	uint8_t avgspeed = 0;
	uint8_t Brake = 0;
	bool Battery_Charging = 0;
	uint8_t state = 1;
    while (1) {
			
			switch(state)
			{
				case 1:
					
					/*	
					* First state of Joystick Task
					*	Update PS2 Buttons Values
					*	Check if the data is available meaning that there is connection between the mcu and Joystick
					* Move to next state
					*/
				
					PS2_UpdatePS2Values(SSI0_BASE ,GPIOA ,7 );
					if(GetIsDataAvFlag())
					{
						/* Send to the dashboard that there is no failure in the main control system */
						if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
						{
							vDashboardSend(Fault , 1);
							xSemaphoreGive(mxDashboard);
						}
						state = 2;
					}
					else
					{
						/* 
						* Send to the dashboard that there is a failure in the main control system 
						* so it must repeat this state untill the connection returns
						*/
						
						if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
						{
							vDashboardSend(Fault , 0);
							xSemaphoreGive(mxDashboard);
						}
						state = 1;
					}
					break;
				case 2:
					
					/*	
					* Second state of Joystick Task
					*	Get PS2 Buttons new Values
					*	Check if the data is available meaning that there is connection between the mcu and Joystick
					* Move to next state
					*/
				
					PS2_GetPS2Values(&PS2JSData);
					state = 3;
					break;
				case 3:
					
					/*	
					* Third state of Joystick Task
					*	Get the the controllers Target speed
					*	Map Target speed from (0 and 255) to (0 and 90) as the maximum speed is 90
					*	Put Target speed in the Target_Speed_Queue
					* Send Target Speed to Dashboard
					* Move to next state
					*/
				
					Target_speedPS2JS = PS2JSData.RJoy_Y;
					Target_speedPS2JS = ui32map_value(Target_speedPS2JS,0,255,0,90);
					xQueueSend(xQueueHandle_Target_Speed, &Target_speedPS2JS, 1);
					vDashboardSendTwoBytes(TargetSpeed,Target_speedPS2JS);
					state = 4;
					break;
				case 4:
					
					/*	
					* Fourth state of Joystick Task
					*	Get the the controllers Direction
					* Send Direction to Dashboard to simulate movement 	0 -> straight
					*																										1 -> left
					*																										2 -> Right					
					* Move to next state
					*/
					DirectionPS2JS = PS2JSData.LJoy_X ;
					if(DirectionPS2JS < 156 && DirectionPS2JS > 100)					// If Direction value between 100 & 156 send straight " 0 " to Dashboard
					{
						DirectionPS2JS = 0;
					}
					else if(DirectionPS2JS <= 100)														// If Direction value less than 156 send left " 1 " to Dashboard
					{
						DirectionPS2JS = 1;
					}
					else																											// If neither of the above then Direction value > 156 send Right " 2 " to Dashboard
					{
						DirectionPS2JS = 2;
					}
					vDashboardSend(Direction,DirectionPS2JS);
					state = 5;
					break;
				case 5:
					
					/*	
					* Fifth state of Joystick Task
					*	Get the the controllers Buttons Values
					* Save brake Status in a global variable for any action will be taken
					*	Send Buttons values to Dashboard
					* Move to First state
					*/
				
					Brake = PS2JSData.L1;
					xQueueSend(xQueueHandle_Brake, &Brake, 0);
					if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
					{
						vDashboardSendControl(BtnControl,PS2JSData.Triangle,PS2JSData.Cross,PS2JSData.LeftArrow,PS2JSData.RightArrow,Brake);
						xSemaphoreGive(mxDashboard);
					}
					vSetDirection(PS2JSData.R1);
					state = 1;
					break;
				default:
					break;
			}
			vTaskDelay( pdMS_TO_TICKS( 53 ) );
    }
}




void vMotorControlTask_Handler (void *parameters) {
		
	uint8_t TSpeed 	= 0		,
					Speed 	= 0		,
					Brake 	= 0		;
	DC_init();
    while (1) {
			xQueueReceive(xQueueHandle_Target_Speed, &TSpeed, 0);
			xQueueReceive(xQueueHandle_Current_Speed, &Speed, 0);
			xQueueReceive(xQueueHandle_Brake, &Brake, 0);
			if(TSpeed > Speed && Brake == 1)
			{
				if(ui8GetDirection() == 1)
				{
					DC_Forward();	
				}
				else
				{
					DC_Forward();																		// It Should be Backward but the motor used for simulation is mono-directional
				}
			}
			else
			{
				
				DC_Stop();					
			}
			vTaskDelay( pdMS_TO_TICKS( 7 ) );
			}			
}


void SensorsReadings_Handler (void *parameters) {
	  uint32_t 	Etemp						,
							Ctemp						,
							Battery_Level		,
							state = 1				;
		bool Battery_Charging = true;
		
		BatteryChargInd_Init(GPIOF,4);
    EngineTempInit();
    BatteryInit();
    CarTempInit();
	while(1)
		{
			switch(state)
			{
				case 1:
					/*	
					* First state of Sensor readings Task
					*	Get Engine Temp to send to Dashboard
					*	set Maximum value for engine temperature to not Exceed the Gif sequence
					*	Send Engine Temp to Dashboard
					*/
					Etemp = EngineTempRead();
					if(Etemp>100)	Etemp = 100;
					if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
					{
						vDashboardSendThreeBytes(EngineHeat,Etemp);
						xSemaphoreGive(mxDashboard);
					}
					state = 2;
					break;
				case 2:
					/*	
					* Second state of Sensor readings Task
					*	Get inside Car Temp to send to Dashboard
					*	Send inside Car Temp to Dashboard
					*/
					Ctemp = CarTempRead();
					if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
					{
						vDashboardSendThreeBytes(InsideCarTemp,Ctemp);
						xSemaphoreGive(mxDashboard);
					}
					state = 3;
					break;
				case 3:
					/*	
					* Third state of Sensor readings Task
					*	Get Battery Level to send to Dashboard
					*	set Maximum value for Battery Level to not Exceed the Gif sequence for simulation
					*	Send Battery Level to Dashboard
					*/
					Battery_Level = BatteryRead();
					Battery_Level = ui32map_value(Battery_Level,0,3000,0,100);
					if(Battery_Level>100)	Battery_Level = 100;
					if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
					{
						vDashboardSendThreeBytes(BatteryLevel,Battery_Level);
						xSemaphoreGive(mxDashboard);
					}
					state = 4;
					break;
				case 4:
					/*	
					* Fourth state of Sensor readings Task
					*	Get Battery Charging status to send to Dashboard
					*	Save Battery Charging status in a global variable for any action might be taken
					*	Send Battery Charging status to Dashboard
					*/
					Battery_Charging = GetBatteryChargStatus();
					if(xSemaphoreTake(mxDashboard,portMAX_DELAY))
					{
						vDashboardSend(BatteryCharging,Battery_Charging);
						xSemaphoreGive(mxDashboard);
					}
					state = 1;
					break;
				default:
					break;
			}			
			vTaskDelay( pdMS_TO_TICKS( 100 ) );
	}
}



void prvSetupHardware (void) {
		Init_Interrupt();
		vDashboardConnect_Init(UART3);
    /* Setup: Button and RGB LED */
    prvSetupGPIO();
    /* Initialize Console UART : 0 */
    UART_Init(UART0, 115200);
    /* Initialize IOT */
    prvSetupIOT();
}
/******************************************************************************
 *                  This is a "Board-Specific" function
 *****************************************************************************/
static void
prvSetupGPIO (void) {

}

static void
prvSetupIOT (void) {
    /* Initialize WIFI UART : 1 */
    WIFI_Init(UART1, 115200);
    /* Join Access Point */
    WIFI_JoinAccessPoint("Elkomys", "Elkomys_wifi_2011");
    SysCtlDelay(5000 * ( SysCtlClockGet() / (1000 * 3) )); //delay ~ seconds
    /* Start TCP Connection */
    WIFI_EstablishConnection_Single("TCP", "mqtt.flespi.io", 1883, WIFI_TCP_KEEP_ALIVE_MAX);
    SysCtlDelay(2000 * ( SysCtlClockGet() / (1000 * 3) )); //delay ~ seconds
}


void vClockConfiguration(void)
{
	  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    (void) SystemCoreClockUpdate();
    uint32_t volatile clk = SysCtlClockGet();

}
	

void
vApplicationIdleHook (void) {
    /* Send the CPU to "Normal Sleep Mode" */
    __asm volatile ("wfi");
}
