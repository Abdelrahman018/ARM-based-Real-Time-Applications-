#ifndef __SPI_H__
#define __SPI_H__

#include <stdbool.h>
#include <stdint.h>

#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */

#define SPI_DATA_REVERSE 				1
#define SPI_DATA_Normal					0

typedef enum SPI_MODES_T{
	SPI_MODE_0 , SPI_MODE_1 , SPI_MODE_2 , SPI_MODE_3   
	}SPI_modes_t;
	

unsigned char Reverse(unsigned char ucNumber);
void  SSI_Init(SSI0_Type *psSSI , uint32_t ui32Clock ,SPI_modes_t SPI_MODE , uint8_t BITs_Num , uint8_t Reversebits);
uint32_t  SPI_MasterTransmit(SSI0_Type *psSSI,unsigned char cData);


#endif  // __SPI_H__