#ifndef __BTTERY_H__
#define __BTTERY_H__

#include <stdint.h>
#include <stdbool.h>

#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */

#include "GPIO.h"


void 
	BatteryChargInd_Init(GPIOA_Type* SS_PORT_Name , uint8_t pinNum);
bool 
	GetBatteryChargStatus(void);


#endif