#ifndef __SPI_H__
#define __SPI_H__

#include "SPI.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */
#include "ud_datatypes.h"

void
	PS2_Config_Analog_Mode(SSI0_Type *psSSI, GPIOA_Type* SS_PORT_Name , uint8_t SS_num );
void
	PS2_GetPS2Values(PS2_data_t* tempdata);
void
	PS2_UpdatePS2Values(SSI0_Type *psSSI, GPIOA_Type* SS_PORT_Name , uint8_t SS_num );
 void 
	 ResetDataAvFlag(void);
 bool 
	 GetIsDataAvFlag(void);
#endif  // __SPI_H__