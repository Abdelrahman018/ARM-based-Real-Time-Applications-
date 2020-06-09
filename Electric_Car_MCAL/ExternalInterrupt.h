#ifndef __RTOS_TASKS__
#define __RTOS_TASKS__

#include <stdbool.h>
#include <stdint.h>
#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123GH6PM.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */
#include "driverlib/gpio.h" 
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"

void 
	Init_Interrupt(void);


#endif