/*
 * GPIO.h
 *
 *  Created on: Mar 31, 2020
 *      Author: meelg
 */

#ifndef MCAL_INC_GPIO_H_
#define MCAL_INC_GPIO_H_

#include "TM4C123GH6PM.h"
#include "inc/tm4c123gh6pm.h"
#include "stdint.h"
#include "stdbool.h"


#define GPIO_BIT0       0x01
#define GPIO_BIT1       0x02
#define GPIO_BIT2       0x04
#define GPIO_BIT3       0x08
#define GPIO_BIT4       0x10
#define GPIO_BIT5       0x20
#define GPIO_BIT6       0x40
#define GPIO_BIT7       0x80

#define GPIO_NO_PULLUP  0x00

typedef enum{
    GPIO_DIR_INPUT,
    GPIO_DIR_OUTPUT

}GPIO_DIR;


void GPIOF_Init(uint32_t pins,GPIO_DIR DIR,uint32_t PullupPins);
void GPIOF_Write(uint32_t pins,bool Data);
uint32_t GPIOF_Read(uint32_t pin);
void
	vInitGPIOclk(GPIOA_Type* SS_PORT_Name );






















#endif /* MCAL_INC_GPIO_H_ */
