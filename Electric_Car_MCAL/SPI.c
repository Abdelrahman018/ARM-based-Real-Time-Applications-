#include "SPI.h"

static uint8_t bitsreversed = 0;

unsigned char 
	Reverse(unsigned char ucNumber) 
		{  
			unsigned short ucIndex;  
			unsigned short ucReversedNumber = 0;  
			for(ucIndex=0; ucIndex<8; ucIndex++)  {   
			ucReversedNumber = ucReversedNumber << 1;   
			ucReversedNumber |= ((1 << ucIndex) & ucNumber) >> ucIndex;  
			}  
return ucReversedNumber; }

void 
	SSI_Init(SSI0_Type *psSSI , uint32_t ui32Clock ,SPI_modes_t SPI_MODE , uint8_t BITs_Num , uint8_t Reversebits) {  
		bitsreversed	= Reversebits;	
		switch ((uint32_t) psSSI) {
			case (uint32_t) SSI0_BASE:
				SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R0;								// enable clock for SSI0     
				SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R0;      				// enable clock for GPIOA
				GPIOA->AMSEL 	&= ~0b10111100;												// disable analog mode for SSI0 pins
				GPIOA->DEN 		|= 0b10111100;         								// enable digital pins for SSI0 pins    
				GPIOA->AFSEL 	|= 0b00111100;												// enable Alternate function for SSI0 pins
				GPIOA -> PCTL &= ~(GPIO_PCTL_PA2_M
				| GPIO_PCTL_PA3_M | GPIO_PCTL_PA4_M | 
				GPIO_PCTL_PA5_M);											     					// Clear SSI0 Pins Alternative function
				GPIOA -> PCTL |=  GPIO_PCTL_PA2_SSI0CLK 
				| GPIO_PCTL_PA3_SSI0FSS | GPIO_PCTL_PA4_SSI0RX 
				| GPIO_PCTL_PA5_SSI0TX;  														// Set SSI0 Pins Alternative function	
				GPIOA->DIR &= ~(1<<4);															// make SSI0RX pin input
				GPIOA->PUR |= (1<<4);																// Pullup SSI0RX pin
				GPIOA->DIR |= (1<<2) | (1<<3) | (1<<5) |(1<<7);							// make SSI0TX & SSI0CLK & SSI0SS pins output
				break;
			default:
				break;
		}
		psSSI->CR1 = 0; 																				// set SPI to Master and keep SSI Disabled before setting any configurations   
		psSSI->CR0 |= (1<<8) ;																	// set SPI Clock Prescaler Div to 2
		psSSI->CR0 |= (SPI_MODE<<6) ;														// set SPI mode
		psSSI->CR0 |= (BITs_Num - 1) ;  												// set SPI Data frame num
		psSSI->CC = 0x0;           															// use system clock 	
		psSSI->CPSR = (ui32Clock/2);         										// set SPI clock   
		psSSI->CR1 |= 2;         																// Enable SSI
	}

	
	
uint32_t 
	SPI_MasterTransmit(SSI0_Type *psSSI,unsigned char cData) {
		/* Start transmission */
		if(bitsreversed == 0)
		{
			psSSI->DR = cData;
			/* Wait for transmission complete */
			while(((psSSI->SR)>>4)&1);
			return psSSI->DR;
		}
		else
		{
			psSSI->DR = Reverse(cData);
			/* Wait for transmission complete */
			while(((psSSI->SR)>>4)&1);
			return Reverse(psSSI->DR);
		}
	}
	