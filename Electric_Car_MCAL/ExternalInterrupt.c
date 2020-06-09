#include "ExternalInterrupt.h"

extern volatile uint32_t _ui32PulseCount ;


void SpeedSensorIntHandler(void)
{
	GPIOIntClear(GPIO_PORTE_BASE,GPIO_INT_PIN_4);
	_ui32PulseCount++;
	//GPIOF->DATA_Bits[1<<1] 		^= (1<<1);
}


void Init_Interrupt(void)
{
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }
		
		GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,GPIO_PIN_4 );

		GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTE_BASE, SpeedSensorIntHandler);

    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4 );
		IntMasterEnable();
	}
