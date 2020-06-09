#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "TM4C123gh6pm.h"

//States	
#define Idle 0
#define turnR 1
#define turnL 2
#define Backword 3
#define Braking 4
#define speedUpF 5
#define speedDownF 6

static GPIOA_Type* PORT_Name_1;
static  GPIOA_Type* PORT_Name_2;

//PORTD
#define PD7   ((volatile unsigned long *)0x40007200)
#define PD6   ((volatile unsigned long *)0x40007100)
#define PD5   ((volatile unsigned long *)0x40007080)
#define PD4   ((volatile unsigned long *)0x40007040)
#define PD3   ((volatile unsigned long *)0x40007020)
#define PD2   ((volatile unsigned long *)0x40007010)
#define PD1   ((volatile unsigned long *)0x40007008)
#define PD0   ((volatile unsigned long *)0x40007004)
//PORTB
#define PB7   ((volatile unsigned long *)0x40005200)
#define PB6   ((volatile unsigned long *)0x40005100)
#define PB5   ((volatile unsigned long *)0x40005080)
#define PB4   ((volatile unsigned long *)0x40005040)
#define PB3   ((volatile unsigned long *)0x40005020)
#define PB2   ((volatile unsigned long *)0x40005010)
#define PB1   ((volatile unsigned long *)0x40005008)
#define PB0   ((volatile unsigned long *)0x40005004)
//PORTA
#define PA7   ((volatile unsigned long *)0x40004200)
#define PA6   ((volatile unsigned long *)0x40004100)
#define PA5   ((volatile unsigned long *)0x40004080)
#define PA4   ((volatile unsigned long *)0x40004040)
#define PA3   ((volatile unsigned long *)0x40004020)
#define PA2   ((volatile unsigned long *)0x40004010)
#define PA1   ((volatile unsigned long *)0x40004008)
#define PA0   ((volatile unsigned long *)0x40004004)
//PORTC
#define PC7   ((volatile unsigned long *)0x40006200)
#define PC6   ((volatile unsigned long *)0x40006100)
#define PC5   ((volatile unsigned long *)0x40006080)
#define PC4   ((volatile unsigned long *)0x40006040)
#define PC3   ((volatile unsigned long *)0x40006020)
#define PC2   ((volatile unsigned long *)0x40006010)
#define PC1   ((volatile unsigned long *)0x40006008)
#define PC0   ((volatile unsigned long *)0x40006004)

//PORTE

#define PE5   ((volatile unsigned long *)0x40024080)
#define PE4   ((volatile unsigned long *)0x40024040)
#define PE3   ((volatile unsigned long *)0x40024020)
#define PE2   ((volatile unsigned long *)0x40024010)
#define PE1   ((volatile unsigned long *)0x40024008)
#define PE0   ((volatile unsigned long *)0x40024004)


//PORTF

#define PF4   ((volatile unsigned long *)0x40025040)
#define PF3   ((volatile unsigned long *)0x40025020)
#define PF2   ((volatile unsigned long *)0x40025010)
#define PF1   ((volatile unsigned long *)0x40025008)
#define PF0   ((volatile unsigned long *)0x40025004)



	
typedef enum US_PORT_NUM_T{
	US_PIN0,
	US_PIN1,
	US_PIN2,
	US_PIN3,
	US_PIN4,
	US_PIN5,
	US_PIN6,
	US_PIN7
}eUS_Pin_Num_t;


//static eUS_Pin_Num_t sgeTrigPin , sgeEchoPin;

void 			delayMs(uint32_t ui32Ms);
void 			delayUs(uint32_t ui32Us);

void			vUltrasonicInit(GPIOA_Type* PORT_Name,eUS_Pin_Num_t TrigPin , eUS_Pin_Num_t EchoPin);
uint32_t ui32_UltrasonicGetDistance(GPIOA_Type* PORT_Name1,eUS_Pin_Num_t TrigPin1 , eUS_Pin_Num_t EchoPin1);
void EN_PORT_CLK(GPIOA_Type* PORT_Name);
void     get_GPIO_Num(GPIOA_Type* PORT_Name);



void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);



#endif    /*  ULTRASONIC_H_  */