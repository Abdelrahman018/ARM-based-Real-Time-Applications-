/*
 * GPIO.c
 *
 *  Created on: Mar 31, 2020
 *      Author: meelg
 */

#include "GPIO.h"


void GPIOF_Init(uint32_t Pins,GPIO_DIR DIR,uint32_t PullupPins)
{
    volatile unsigned long delay;
    vInitGPIOclk(GPIOF);									// 1) F clock Enable
    GPIO_PORTF_LOCK_R   = 0x4C4F434B;     // 2) unlock PortF PF0
    GPIO_PORTF_CR_R     = 0xFF;           // 3) allow changes to selected pins
		GPIO_PORTF_AMSEL_R  	&= ~Pins;       // 4) disable analog function
    GPIO_PORTF_PCTL_R   	&= ~Pins;       // 5) GPIO clear bit PCTL
    // 6) Select Input/Output pins
    switch(DIR)
    {
    case GPIO_DIR_INPUT:
        GPIO_PORTF_DIR_R &= ~Pins;
        break;
    case GPIO_DIR_OUTPUT:
        GPIO_PORTF_DIR_R |= Pins;
        break;
    default:
        break;
    }

    GPIO_PORTF_AFSEL_R  &= ~Pins;         // 7) no alternate function
    //GPIO_PORTF_PUR_R    = PullupPins;   // 8) enable Pullup resistors to selected pins
    GPIO_PORTF_DEN_R    |= Pins;          // 9) enable digital to selected pins
}

void GPIOF_Write(uint32_t pins,bool Data)
{
    if(Data == 1) GPIO_PORTF_DATA_R |=  pins;
    else          GPIO_PORTF_DATA_R &= ~pins;
}
uint32_t GPIOF_Read(uint32_t pin)
{
    return GPIO_PORTF_DATA_R &= pin;
}


void vInitGPIOclk(GPIOA_Type* SS_PORT_Name )
{
    switch((uint32_t)SS_PORT_Name)
    {
        case (uint32_t)GPIOA:           SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R0;         break;
        case (uint32_t)GPIOB:           SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R1;         break;
        case (uint32_t)GPIOC:           SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R2;         break;
        case (uint32_t)GPIOD:           SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R3;         break;
        case (uint32_t)GPIOE:           SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R4;         break;
        case (uint32_t)GPIOF:           SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R5;         break;
        default:                                                                                                                            break;
    }
}

