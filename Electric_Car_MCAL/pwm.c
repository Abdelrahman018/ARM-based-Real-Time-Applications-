#include "PWM.h"



void vidInitPWM_0 (pin_mod_0 pin,CLK_SEL clk)
{
    /* enable clock source to PWM0 module */
    SYSCTL_RCGCPWM_R |= 1;
    //enable clock to PORT
    //enable the appropriate pins for their alternate function
    //Assign PWM signals to the appropriate pins
    /* make pins output */
    /* set pins digital */

    switch (pin)
    {
    case 0://	PB6_0,

        SYSCTL_RCGCGPIO_R |= (1<<1);
        GPIO_PORTB_AFSEL_R=0x40;
        GPIO_PORTB_PCTL_R=0x0400000;
        GPIO_PORTB_DIR_R   |= (1<<6);
        GPIO_PORTB_DEN_R   |= (1<<6);

        break;
    case 1://PB7_0,

        SYSCTL_RCGCGPIO_R |= (1<<1);
        GPIO_PORTB_AFSEL_R=0x80 ;
        GPIO_PORTB_PCTL_R=0x4000000;
        GPIO_PORTB_DIR_R   |= (1<<7);
        GPIO_PORTB_DEN_R   |= (1<<7);
        break;

    case 2://PB4_0,

        SYSCTL_RCGCGPIO_R |= (1<<1);
        GPIO_PORTB_AFSEL_R=0x10;
        GPIO_PORTB_PCTL_R=0x0004000;
        GPIO_PORTB_DIR_R   |= (1<<4);
        GPIO_PORTB_DEN_R   |= (1<<4);
        break;
    case 3://PB5_0,

        SYSCTL_RCGCGPIO_R |= (1<<1);
        GPIO_PORTB_AFSEL_R=0x20;
        GPIO_PORTB_PCTL_R=0x00400000;
        GPIO_PORTB_DIR_R   |= (1<<5);
        GPIO_PORTB_DEN_R   |= (1<<5);
        break;

    case 4://PE4_0,

        SYSCTL_RCGCGPIO_R |= (1<<4);
        GPIO_PORTE_AFSEL_R=0x10;
        GPIO_PORTE_PCTL_R=0x0040000;
        GPIO_PORTE_DIR_R   |= (1<<4);
        GPIO_PORTE_DEN_R   |= (1<<4);
        break;
    case 5://PE5_0,

        SYSCTL_RCGCGPIO_R |= (1<<4);
        GPIO_PORTE_AFSEL_R=0x20;
        GPIO_PORTE_PCTL_R=0x00400000;
        GPIO_PORTE_DIR_R   |= (1<<5);
        GPIO_PORTE_DEN_R   |= (1<<5);
        break;

    case 6://PC4_0,

        SYSCTL_RCGCGPIO_R |= (1<<2);
        GPIO_PORTC_AFSEL_R=0x10;
        GPIO_PORTC_PCTL_R=00040000;
        GPIO_PORTC_DIR_R   |= (1<<4);
        GPIO_PORTC_DEN_R   |= (1<<4);
        break;
    case 7://PD0_0,

        SYSCTL_RCGCGPIO_R |= (1<<3);
        GPIO_PORTD_AFSEL_R=0x01;
        GPIO_PORTD_PCTL_R=0x00000004;
        GPIO_PORTD_DIR_R   |= 1;
        GPIO_PORTD_DEN_R   |= 1;
        break;
    case 8://PC5_0

        SYSCTL_RCGCGPIO_R |= (1<<2);
        GPIO_PORTC_AFSEL_R=0x20;
        GPIO_PORTC_PCTL_R=0x00400000;
        GPIO_PORTC_DIR_R   |= (1<<8);
        GPIO_PORTC_DEN_R   |= (1<<8);
        break;

    case 9://PD1_0
        SYSCTL_RCGCGPIO_R |= (1<<3);
        GPIO_PORTD_AFSEL_R=0x02;
        GPIO_PORTD_PCTL_R=0x00000040;
        GPIO_PORTD_DIR_R   |= (1<<9);
        GPIO_PORTD_DEN_R   |= (1<<9);
        break;
    default:
        break;
    }
    //Configure the Run-Mode Clock Configuration (RCC) register in the System Control module
    SYSCTL_RCC_R|=(clk<<17);

    /* stop counter */
    PWM0_0_CTL_R= 0;
    /* M1PWM7 output set when reload, */
    /* clear when match PWMCMPA */
    PWM0_0_GENB_R = 0x0000008C;

}


void vidInitPWM_1 (pin_mod_1 pin,CLK_SEL clk)
{
    /* enable clock source to PWM0 module */
    SYSCTL_RCGCPWM_R |= 2;
    //enable the appropriate pins for their alternate function
    //Assign PWM signals to the appropriate pins

    switch (pin)
    {
    case 0://PD0_1
        SYSCTL_RCGCGPIO_R |= (1<<3);
        GPIO_PORTD_AFSEL_R=0x01;
        GPIO_PORTD_PCTL_R=0x00000005;
        GPIO_PORTD_DIR_R|=1;
        GPIO_PORTD_DEN_R   |= 1;
        break;

    case 1:// PD1_1
        SYSCTL_RCGCGPIO_R |= (1<<3);
        GPIO_PORTD_AFSEL_R=0x02 ;
        GPIO_PORTD_PCTL_R=0x00000050;
        GPIO_PORTD_DIR_R   |= (1<<1);
        GPIO_PORTD_DEN_R   |= (1<<1);
        break;

    case 2://PA6_1,
        SYSCTL_RCGCGPIO_R |= 1;
        GPIO_PORTA_AFSEL_R=0x40;
        GPIO_PORTA_PCTL_R=0x05000000;
        GPIO_PORTA_DIR_R   |= (1<<6);
        GPIO_PORTA_DEN_R   |= (1<<6);
        break;

    case 3://PE4_1,
        SYSCTL_RCGCGPIO_R |= (1<<4);
        GPIO_PORTE_AFSEL_R=0x10;
        GPIO_PORTE_PCTL_R=0x00050000;
        GPIO_PORTE_DIR_R   |= (1<<4);
        GPIO_PORTE_DEN_R   |= (1<<4);
        break;

    case 4://PA7_1,
        SYSCTL_RCGCGPIO_R |= 1;
        GPIO_PORTA_AFSEL_R=0x80;
        GPIO_PORTA_PCTL_R=0x50000000;
        GPIO_PORTA_DIR_R   |= (1<<7);
        GPIO_PORTA_DEN_R   |= (1<<7);
        break;

    case 5://PE5_1,

        SYSCTL_RCGCGPIO_R |= (1<<4);
        GPIO_PORTE_AFSEL_R=0x20;
        GPIO_PORTE_PCTL_R=0x00500000;
        GPIO_PORTE_DIR_R   |= (1<<5);
        GPIO_PORTE_DEN_R   |= (1<<5);
        break;

    case 6://PF0_1,
        SYSCTL_RCGCGPIO_R |= (1<<5);
        GPIO_PORTF_AFSEL_R=0x01;
        GPIO_PORTF_PCTL_R=0x00000005;
        GPIO_PORTF_DIR_R   |= (1<<0);
        GPIO_PORTF_DEN_R   |= (1<<0);
        break;

    case 7://PF1_1,
        SYSCTL_RCGCGPIO_R |= (1<<5);
        GPIO_PORTF_PCTL_R=0x00000050;
        GPIO_PORTF_DIR_R   |= (1<<0);
        GPIO_PORTF_DEN_R   |= (1<<1);
        break;
    case 8://PF2_1,
        SYSCTL_RCGCGPIO_R |= (1<<5);
        GPIO_PORTF_AFSEL_R=0x04;
        GPIO_PORTF_PCTL_R=0x00000500;
        GPIO_PORTF_DIR_R   |= (1<<2);
        GPIO_PORTF_DEN_R   |= (1<<2);
        break;

    case 9://PF3_1
        SYSCTL_RCGCGPIO_R |= (1<<5);
        GPIO_PORTF_AFSEL_R=8;
        GPIO_PORTF_PCTL_R=0x00005000;
        GPIO_PORTF_DIR_R|= (1<<3);
        GPIO_PORTF_DEN_R   |= (1<<3);

        break;
    default:
        break;
    }
    //Configure the Run-Mode Clock Configuration (RCC) register in the System Control module
    SYSCTL_RCC_R|=(clk<<17);
    /* stop counter */
    PWM1_0_CTL_R= 0;
    /* M1PWM7 output set when reload, */
    /* clear when match PWMCMPA */
    PWM1_0_GENB_R = 0x0000008C;



}


void vidSetDutyandFreq_0(uint16_t duty , uint16_t freq)
{
    PWM0_0_LOAD_R=PWM0_0_LOAD_R;
    //PWM0_0_LOAD_R=80/freq;

    PWM0_0_CMPA_R=((100-(duty) )/100)*PWM1_0_LOAD_R;


}
void vidSetDutyandFreq_1(uint16_t duty , uint16_t freq)
{

    PWM1_0_LOAD_R=80/freq;
    PWM1_0_CMPA_R=((100-(duty) )/100) *PWM1_0_LOAD_R;


}
void vidStartPWM_0(void)
{
    PWM0_0_CTL_R|= 1; /* start timer */
    PWM0_ENABLE_R = 0x80;
    while(1);
}
void vidStartPWM_1(void)
{
    PWM1_0_CTL_R = 1; /* start timer */
    PWM1_ENABLE_R = 0x80; /* start PWM1 ch7 */
    while(1);
}

