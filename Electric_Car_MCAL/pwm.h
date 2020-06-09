
#include "stdint.h"
#include "stdbool.h"
#include "inc/tm4c123gh6pm.h"

typedef enum{
    PB6_0,
    PB7_0,
    PB4_0,
    PB5_0,
    PE4_0,
    PE5_0,
    PC4_0,
    PD0_0,
    PC5_0,
    PD1_0
}pin_mod_0;

typedef enum{
    PD0_1,
    PD1_1,
    PA6_1,
    PE4_1,
    PA7_1,
    PE5_1,
    PF0_1,
    PF1_1,
    PF2_1,
    PF3_1
}pin_mod_1;



typedef enum {
    PWM_NO_DIV,
    CLK_DEV_2 = 0x08,
    CLK_DEV_4,
    CLK_DEV_8,
    CLK_DEV_16,
    CLK_DEV_32,
    CLK_DEV_64_1,
    CLK_DEV_64_2,
    CLK_DEV_64_DEFAULT
}CLK_SEL;


void vidInitPWM_0 (pin_mod_0 pin,CLK_SEL clk);
void vidInitPWM_1 (pin_mod_1 pin,CLK_SEL clk);
void vidSetDutyandFreq_0(uint16_t duty , uint16_t freq);
void vidSetDutyandFreq_1(uint16_t duty , uint16_t freq);
void vidStartPWM_0(void);
void vidStartPWM_1(void);


