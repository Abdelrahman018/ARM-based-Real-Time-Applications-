/*
 * DC.c
 *
 *  Created on: Mar 31, 2020
 *      Author: meelg
 */


#include "inc/tm4c123gh6pm.h"
#include "DC.h"




void DC_init(void)
{
    //Set PF0 >> Forward IN ,Set PF4 >> Backward IN
    //GPIOF_Init(GPIO_BIT0|GPIO_BIT4, GPIO_DIR_INPUT , GPIO_BIT0|GPIO_BIT4);
    //Set PF3 Forward Out , PF2 Backward OUT , PF1 Stop
    GPIOF_Init(GPIO_BIT1|GPIO_BIT2|GPIO_BIT3, GPIO_DIR_OUTPUT, GPIO_NO_PULLUP);
}


void DC_Forward(void)
{
    GPIOF_Write(GPIO_BIT1, 0);
    GPIOF_Write(GPIO_BIT2, 0);
    GPIOF_Write(GPIO_BIT3, 1);
}
void DC_Backward(void)
{
    GPIOF_Write(GPIO_BIT1, 0);
    GPIOF_Write(GPIO_BIT2, 1);
    GPIOF_Write(GPIO_BIT3, 0);
}
void DC_Left(void)
{

}
void DC_Right(void)
{

}
void DC_Stop(void)
{
   // GPIOF_Write(GPIO_BIT1, 1);
    GPIOF_Write(GPIO_BIT2, 0);
    GPIOF_Write(GPIO_BIT3, 0);
}


void Btn_init(uint32_t pins)
{
    GPIOF_Init(pins, GPIO_DIR_INPUT, pins);
}
uint32_t Btn_Read(uint32_t pins)
{
    return GPIO_PORTF_DATA_R&pins ;

}





