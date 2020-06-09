#ifndef __UART_H__
#define __UART_H__


#include <stdbool.h>
#include <stdint.h>

#ifndef _TM4C123_H
#define _TM4C123_H
    #include "TM4C123GH6PM.h"
    #include "inc/tm4c123gh6pm.h"
#endif /* _TM4C123_H */

//                              HSE in UARTCTL = 0
#define UART_BRDVAL (80000000UL / (16.0 * ui32BaudRate))  // UARTSysClk / (ClkDiv * Baud Rate)
#define UART0AHB	0
#define UART1AHB    0
#define UART2AHB    0
#define UART3AHB    0
#define UART4AHB    0
#define UART5AHB    0
#define UART6AHB    0
#define UART7AHB    0

/********************* Initializations ***********************/
void UART_Init (UART0_Type *psUART, uint32_t ui32BaudRate);
/********************* TX, RX using polling **********************/
void 	UART_TX_Byte_P  (UART0_Type *psUART, uint8_t ui8Data);
uint8_t UART_RX_Byte_P  (UART0_Type *psUART);
void    UART_TX_Str_P   (UART0_Type *psUART, char *pui8Str);
void    UART_TX_Data_P  (UART0_Type *psUART, uint8_t *pui8Data, uint32_t ui32DataLen);
/******************* TX, RX without checking *********************/
void    UART_TX_Byte (UART0_Type *psUART, uint8_t ui8Data);
uint8_t UART_RX_Byte (UART0_Type *psUART);
/************* RX with checking only (no polling) ****************/
bool UART_RX_Byte_Unblock (UART0_Type *psUART, uint8_t *pui8Data);
/****** IRQs "Setters", when called - Here - are "Getters" ******/
void UART_SetIRQ_TX (UART0_Type *psUART, void (*pf) (void));
void UART_SetIRQ_RX (UART0_Type *psUART, void (*pf) (void));


#endif  // __UART_H__
