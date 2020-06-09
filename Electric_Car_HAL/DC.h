/*
 * DC.h
 *
 *  Created on: Mar 31, 2020
 *      Author: meelg
 */

#ifndef HAL_INC_DC_H_
#define HAL_INC_DC_H_

#include "inc/tm4c123gh6pm.h"
#include "GPIO.h"


void DC_init(void);
void DC_Forward(void);
void DC_Backward(void);
void DC_Left(void);
void DC_Right(void);
void DC_Stop(void);

void Btn_init(uint32_t pins);
uint32_t Btn_Read(uint32_t pins);

#endif /* HAL_INC_DC_H_ */
