#include "MyUS.h"





volatile unsigned long ** arr;
volatile unsigned long * PD[8]={PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7};
volatile unsigned long * PA[8]={PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7};
volatile unsigned long * PB[8]={PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};
volatile unsigned long * PC[8]={PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7};
volatile unsigned long * PE[8]={PE0,PE1,PE2,PE3,PE4,PE5};
volatile unsigned long * PF[8]={PF0,PF1,PF2,PF3,PF4};




void delayMs(uint32_t ui32Ms) {

	// 1 clock cycle = 1 / SysCtlClockGet() second
	// 1 SysCtlDelay = 3 clock cycle = 3 / SysCtlClockGet() second
	// 1 second = SysCtlClockGet() / 3
	// 0.001 second = 1 ms = SysCtlClockGet() / 3 / 1000
	
	SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000));
}

void delayUs(uint32_t ui32Us) {
	SysCtlDelay(ui32Us * (SysCtlClockGet() / 3 / 1000000));
}



void EN_PORT_CLK(GPIOA_Type* PORT_Name) // return PD or PF ....
{
	if	(PORT_Name == GPIOA)		SYSCTL->RCGCGPIO |= 0x01;	 			// Enable Port (D) Clock 0x01;
	else if	(PORT_Name == GPIOB)	SYSCTL->RCGCGPIO |= 0x02;
	else if	(PORT_Name == GPIOC)	SYSCTL->RCGCGPIO |= 0x04;
	else if	(PORT_Name == GPIOD)	SYSCTL->RCGCGPIO |= 0x08;
	else if	(PORT_Name == GPIOE)	SYSCTL->RCGCGPIO |= 0x10;
	else if	(PORT_Name == GPIOF)	SYSCTL->RCGCGPIO |= 0x20;

}



void get_GPIO_Num(GPIOA_Type* PORT_Name){
  if	(PORT_Name == GPIOA)		arr=PA;
	else if	(PORT_Name == GPIOB)	arr=PB;
	else if	(PORT_Name == GPIOC)	arr=PC;
	else if	(PORT_Name == GPIOD)	arr=PD;
	else if	(PORT_Name == GPIOE)	arr=PE;
	else if	(PORT_Name == GPIOF)	arr=PF;
	

}

//---------------------------------------------------------------------------------------------------
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}

// 800000*12.5ns equals 10ms ---> 80 MHz
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

void
	vUltrasonicInit(GPIOA_Type* PORT_Name,eUS_Pin_Num_t TrigPin , eUS_Pin_Num_t EchoPin)
	{
		
		PORT_Name_1 = PORT_Name;
		//SYSCTL->RCGCGPIO |= 0x08;	 			// Enable Port (D) Clock
		PORT_Name_1->DEN |= (1<<EchoPin) | (1<<TrigPin);	 		// Enable Digital Pin
		PORT_Name_1->AMSEL &= ~(1<<EchoPin) | (1<<TrigPin); 	// Disable Analog Mode 
		PORT_Name_1->AFSEL &= ~(1<<EchoPin) | (1<<TrigPin); 	// Disable Alternative function
		PORT_Name_1->DIR |= (1<<TrigPin);		 									// Set Pin D0 to Output 
		PORT_Name_1->DIR &= ~(1<<EchoPin);		 								// Set Pin D3 to Input
	/*	*/
	
		
		/*volatile unsigned long delay;
		
		sgp_PORT_Name = PORT_Name;
		sgeTrigPin = TrigPin;
		sgeEchoPin = EchoPin;
		//SYSCTL->RCGCGPIO |= Get_GPIO_Num(PORT_Name);	 			// Enable Port (D) Clock 
		SYSCTL_RCGC2_R |= 0x00000008;    											 // activate clock for Port D
		delay = SYSCTL_RCGC2_R;          											 // allow time for clock to start
			
		GPIO_PORTD_LOCK_R = 0x4C4F434B;  												 // unlock GPIO Port D 
    GPIO_PORTD_CR_R = 0x1F;          												 // allow changes to PD4-0 
		
		PORT_Name->AMSEL &= ~(1<<EchoPin) | (1<<TrigPin); 				// Disable Analog Mode 
		
		GPIO_PORTD_PCTL_R = 0x00000000;  											 //  PCTL GPIO on PD4-0 ------

		PORT_Name->DIR |= (1<<TrigPin);		 										// Set Pin D0 to Output (TrigPin)
		PORT_Name->DIR &= ~(1<<EchoPin);		 									// Set Pin D2 to Input  (EchoPin)
		
		PORT_Name->AFSEL &= ~(1<<EchoPin) | (1<<TrigPin); 		// Disable Alternative function
     
		GPIO_PORTD_PDR_R = 0x08;     										     // enable pull-Down on PD3(EchoPin)

		
		PORT_Name->DEN |= (1<<EchoPin) | (1<<TrigPin);	 		// Enable Digital Pin for both
		
		*/

	}

uint32_t ui32_UltrasonicGetDistance(GPIOA_Type* PORT_Name,eUS_Pin_Num_t TrigPin , eUS_Pin_Num_t EchoPin)
	{ 
		uint32_t per = 0;
		// specify the PORT_Name
		get_GPIO_Num(PORT_Name);
	//Trigger....
		
		* Arr[TrigPin] = (1<<TrigPin);	// Set Pin D1		
		delayUs(10);				// Will be replaced by RTOS Delay
		* Arr[TrigPin] = (0<<TrigPin);	// Set Pin D1
				/*
		while(((sgp_PORT_Name->DATA_Bits[(1<<sgeEchoPin)]&1)==0)){
				//	UART_TX_Str_P (UART0, "0");
			  UART_TX_Byte_P(UART0, 'F');

		}	// Wait untill an echo signal high is received
		
		
		
	
		while(((sgp_PORT_Name->DATA_Bits[(1<<sgeEchoPin)]&1)==1))	// Count u Secodnds of the echo signal
		{
			UART_TX_Str_P (UART0, "IN");
			per++;
			delayUs(1);
			//SysTick_Wait(80); // wait for 1 us
		}	*/
		
	
		while(* Arr[EchoPin] == (0<<EchoPin))//echo signal is LOW
			{

		}	// Wait untill an echo signal high is received
				
		while(* Arr[EchoPin] == (1<<EchoPin))	// Count uSecodnds of the echo signal ---//echo signal is high
		{
		
			per++;
			delayUs(1);
		}/**/
			

			return ((per*4)*.0340);
			//return ((per/2)*.0340);
	//	return (per*.0340);
	//	return ((per*1000)*MULTIPLIER);
	}	

	
	