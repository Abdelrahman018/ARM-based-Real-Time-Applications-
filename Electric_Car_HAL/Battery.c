#include "Battery.h"

static uint8_t _gui8_PinNum;
static GPIOA_Type* SS_PORT_Name;
void 
	BatteryChargInd_Init(GPIOA_Type* SS_PORT , uint8_t pinNum)
	{
		/* Initialize Battery charging pin */
		vInitGPIOclk(SS_PORT);
		SS_PORT->AMSEL &= ~(1<<pinNum);
		SS_PORT->DEN |= (1<<pinNum);
		SS_PORT->DIR &= ~(1<<pinNum);
		SS_PORT->AFSEL &= ~(1<<pinNum);
		SS_PORT->PUR |= (1<<pinNum);
		_gui8_PinNum = pinNum;
		SS_PORT_Name = SS_PORT;
	}

bool 
	GetBatteryChargStatus(void)
	{
		/* Gets the battery charging status */
		return (((SS_PORT_Name->DATA_Bits[1<<_gui8_PinNum]) >> _gui8_PinNum ) & 1);
	}