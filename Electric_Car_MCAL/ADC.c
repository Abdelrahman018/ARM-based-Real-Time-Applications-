



#include "ADC.h"

/*
 This initialization function sets up the ADC according to the
 following parameters.  Any parameters not explicitly listed
 below are not modified:
 Max sample rate: <=125,000 samples/second
 Sequencer 0 priority: 1st (Lowest)
 Sequencer 1 priority: 2nd
 Sequencer 2 priority: 3rd
 Sequencer 3 priority: 4th (Highest)
 SS3 triggering event: software trigger
 SS3 1st sample source: User input CH0:CH2
 SS3 interrupts: enabled but not promoted to controller
*/

void ADC0_Init(ADCChannel CH)
{
    uint32_t dummyDelay;

    SYSCTL_RCGCGPIO_R   |= 0x10;    //Enable clock to GPIOE
    while((SYSCTL_PRGPIO_R&0x010)==0);//Wait for ADC0 Peripheral to be ready
    dummyDelay = SYSCTL_RCGCGPIO_R; //Delay for initializing the clock
    switch(CH)
    {
    case CH0:                       //AIN0 is on PE3
        GPIO_PORTE_DIR_R   &=~0x08;  //Make PE3 input
        GPIO_PORTE_AFSEL_R |= 0x08; //Enable alternate function on PE3
        GPIO_PORTE_DEN_R   &=~0x08;  //Disable digital I/O on PE3
        GPIO_PORTE_AMSEL_R |= 0x08; //Enable analog functionality on PE3
        break;
    case CH1:                       //AIN11 is on PE2
        GPIO_PORTE_DIR_R   &=~0x04;  //Make PE2 input
        GPIO_PORTE_AFSEL_R |= 0x04; //Enable alternate function on PE2
        GPIO_PORTE_DEN_R   &=~0x04;  //Disable digital I/O on PE2
        GPIO_PORTE_AMSEL_R |= 0x04; //Enable analog functionality on PE2
        break;
    case CH2:                       //AIN2 is on PE1
        GPIO_PORTE_DIR_R   &=~0x02; //Make PE1 input
        GPIO_PORTE_AFSEL_R |= 0x02; //Enable alternate function on PE1
        GPIO_PORTE_DEN_R   &=~0x02; //Disable digital I/O on PE1
        GPIO_PORTE_AMSEL_R |= 0x02; //Enable analog functionality on PE1
        break;
    default:
        break;
    }

    SYSCTL_RCGCADC_R    |= 0x01;    //Enable Clock For ADC0 Module
    while((SYSCTL_PRADC_R&0x01)==0);//Wait for ADC0 Peripheral to be ready
    ADC0_PC_R           &= ~0xF;    //Clear max sample rate field
    ADC0_PC_R           |= 0x1;     //Configure for 125K samples/Sec
    ADC0_SSPRI_R         = 0x0123;  //Sequencer 3 is Highest priority
}

/*------------ADC0_InSeq3------------
 Busy-wait Analog to digital conversion
 Input: Channel to be Sampled
 Output: 12-bit result of ADC conversion in a Uint32_t variable */

uint32_t ADC0_Read(ADCChannel CH)
{
    volatile uint32_t ADC0Value;
    ADC0_ACTSS_R        &= 0x08;    //Disable Sample Sequencer 3 to configure
    ADC0_EMUX_R         &= 0xF000;  //Use software trigger
    ADC0_SSMUX3_R       &= 0x0F;    //Clear SSMUX before Choosing The Channel
    ADC0_SSMUX3_R        = CH;      //Choose The input Channel
    if(CH ==0)
        ADC0_SSCTL3_R    = 0x0E;    //Single End/one conversion/raw interrupt/temperature Sensor
    else
        ADC0_SSCTL3_R    = 0x06;    //Single End/one conversion/raw interrupt/No temperature Sensor
    ADC0_IM_R           &= ~0x08;   //Disable SS3 interrupts
    ADC0_ACTSS_R        |= 0x08;    //Enable Sample Sequencer 3 to configure
    ADC0_PSSI_R         |= 0x08;    //Start conversion
    while((ADC0_RIS_R&0x08 )== 0);  //Wait for conversion to be Done
    ADC0Value= ADC0_SSFIFO3_R&0xFFF;//Store conversion value
    ADC0_ISC_R           = 0x08;    //Clear interrupt flag for new conversion

    return ADC0Value;               //Return ADC conversion Value
}
