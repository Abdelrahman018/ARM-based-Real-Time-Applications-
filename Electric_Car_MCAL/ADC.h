/*
 * ADC.h
 *
 *  Created on: Apr 1, 2020
 *      Author: meelg
 */

#ifndef MCAL_INC_ADC_H_
#define MCAL_INC_ADC_H_

#include "stdint.h"
#include "stdbool.h"
#include "inc/TM4C123GH6PM.h"


typedef enum
{
    CH0,CH1,CH2,CH3,CH4,CH5,CH6,CH7,CH8,CH9,CH10,CH11
}ADCChannel;


void ADC0_Init(ADCChannel CH);
uint32_t ADC0_Read(ADCChannel CH);


#endif /* MCAL_INC_ADC_H_ */
