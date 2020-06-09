/*
 * Engine_Temp.c
 *
 *  Created on: Apr 1, 2020
 *      Author: Mahmoud Elghzaly
 */

#include "Temperature.h"



void CarTempInit(void)
{
    ADC0_Init(CH2);
}
uint32_t EngineTempRead(void)
{
    return (1475 -((float)(2475 * ADC0_Read(CH2)) / 4096.0))/10;
}

void BatteryInit(void)
{
    ADC0_Init(CH1);
}

uint32_t BatteryRead(void)
{
    return (uint32_t)(ADC0_Read(CH1));
}

void EngineTempInit(void)
{
    ADC0_Init(CH0);
}

uint32_t CarTempRead(void)
{
    return (uint32_t)((((float)(ADC0_Read(CH0)*3300) /4096.0)) /10);
}


