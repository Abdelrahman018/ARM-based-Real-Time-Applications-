/*
 * Engine_Temp.h
 *
 *  Created on: Apr 1, 2020
 *      Author: meelg
 */

#ifndef HAL_INC_TEMP_H_
#define HAL_INC_TEMP_H_

#include "stdint.h"
#include "ADC.h"

void EngineTempInit(void);
uint32_t EngineTempRead(void);


void BatteryInit(void);
uint32_t BatteryRead(void);

void CarTempInit(void);
uint32_t CarTempRead(void);


#endif /* HAL_INC_TEMP_H_ */
