/*
 * DC_PWM.h
 *
 *  Created on: Apr 2, 2020
 *      Author: meelg
 */

#ifndef HAL_INC_DC_PWM_H_
#define HAL_INC_DC_PWM_H_
#include "inc/tm4c123gh6pm.h"
#include "stdint.h"

void DC_PWM_init(void);
void DC_PWM_Forward(void);
void DC_PWM_Backward(void);
void DC_PWM_Left(void);
void DC_PWM_Right(void);
void DC_PWM_Stop(void);




#endif /* HAL_INC_DC_PWM_H_ */
