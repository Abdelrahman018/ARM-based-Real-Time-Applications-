#include "PS2JS.h"
	 
/*
*		This is responsible for communicating with the Playstation 2 Joystick
*/

const static uint8_t _uiEnter_Configuration_mode[] = {0x01,0x43,0x00,0x01,0x00,'#'} ;
const static uint8_t _uiSet_Analog_mode[] = {0x01,0x44,0x00,0x01,0x03,0x00 ,0x00 ,0x00 ,0x00,'#'} ;
const static uint8_t _uiExit_Configuration_mode[] = {0x01,0x43,0x00,0x00,0x5A,0x5A ,0x5A ,0x5A ,0x5A,'#'} ;
static uint8_t Bufferdata[6];
static PS2_data_t* pPS2_data = Bufferdata;
static bool DataAv = false; 
	 
	 
	 
static void PS2_Send_Command(SSI0_Type *psSSI , GPIOA_Type* SS_PORT_Name,  uint8_t SS_num , uint8_t* command0)
	 {
		 /*
			*	This function sends a predefined commands to configure or get data from Joystick
		 */
		 uint8_t itr = 0;
		 SS_PORT_Name->DATA_Bits[1<<SS_num]=0;
		 while(command0[itr] != '#')										// iterate untill reaching the '#' character meaning the end of command
		 {
			 SPI_MasterTransmit(psSSI ,command0[itr]);
			 itr++;
		 }
		 SS_PORT_Name->DATA_Bits[1<<SS_num]=1<<SS_num;	// Pull down attention pin to communicate with Joystick
	 }
	 
void PS2_Config_Analog_Mode(SSI0_Type *psSSI, GPIOA_Type* SS_PORT_Name , uint8_t SS_num )
 {
	 /* Initialize Attention Pin */
	 
		SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R1; 
		SS_PORT_Name->AMSEL 	&= ~ (1<<SS_num);												
		SS_PORT_Name->DEN 		|= (1<<SS_num);
		SS_PORT_Name->DIR    	|= (1<<SS_num);
	 
	 /**************************************************/
	 /* This command puts Joystick into configuration mode to be able to change it's mode */
	 PS2_Send_Command(psSSI ,SS_PORT_Name, SS_num ,_uiEnter_Configuration_mode );		
	 /* Configure Playstation Joystick to work in Analog no pressure mode */
	 PS2_Send_Command(psSSI ,SS_PORT_Name, SS_num ,_uiSet_Analog_mode );
	 /* Exit configuration mode */
	 PS2_Send_Command(psSSI ,SS_PORT_Name, SS_num ,_uiExit_Configuration_mode );
 }

void PS2_GetPS2Values(PS2_data_t* tempdata)
 {
	 /*	Getter for the current PS2 Readings */
		*tempdata = *pPS2_data;
		ResetDataAvFlag();
 }

 void PS2_UpdatePS2Values(SSI0_Type *psSSI, GPIOA_Type* SS_PORT_Name , uint8_t SS_num )
 {
	  /* Update PS2JSData with new values */
		bool PS2dataAv = false;																							// Data availability flag
		SS_PORT_Name->DATA_Bits[1<<SS_num] = 0;															// Attention pin is pulled down to start communicating
		SPI_MasterTransmit(psSSI,(0x01));																		// First Header byte
		/* Second Header byte and checks if the communication is well running between PS2 joystick and MCU
		 * and checks whether the PS2 Joystick is configured to run in Analog mode or not
		 * if not it reconfigures the PS2 Joystick to work in Analog mode
		 */
	 if(SPI_MasterTransmit(psSSI,(0x42)) == 0x73)												
	 {
			SPI_MasterTransmit(psSSI,(0x00));
			Bufferdata[0]	=	(SPI_MasterTransmit(psSSI,(0x00)));								//  get buttons set 1 8
			Bufferdata[1]	=	(SPI_MasterTransmit(psSSI,(0x00)));								//  get button set 2  8
			Bufferdata[2]	=	(255 - SPI_MasterTransmit(psSSI,(0x00))); 				//  rx
			Bufferdata[3]	=	(255 - SPI_MasterTransmit(psSSI,(0x00))); 				//  ry
			Bufferdata[4]	=	(255 - SPI_MasterTransmit(psSSI,(0x00))); 				//  lx
			Bufferdata[5]	=	(255 - SPI_MasterTransmit(psSSI,(0x00))); 				//  ly
		  SS_PORT_Name->DATA_Bits[1<<SS_num] = 1<<SS_num;
		  PS2dataAv = true;																									//  Change Data availble flag to true
	 }
	 else
	 {
		 /* Reconfiguring PS2 Joystick */
		 	SS_PORT_Name->DATA_Bits[1<<SS_num] = 1<<SS_num;
			PS2_Config_Analog_Mode(psSSI,SS_PORT_Name,SS_num);
	 }
	 DataAv = PS2dataAv;																									//  Data availble flag remains false
 }
 
 
 void ResetDataAvFlag(void)
 {
	 /* Resets data availble flag */
		DataAv = false;
 }
 
 bool GetIsDataAvFlag(void)
 {
	 	/* Gets data availble flag */
		return DataAv;
 }
 