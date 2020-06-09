#include "uart.h"


static void (* gs_pfIRQ_UART_TX_0) (void);
static void (* gs_pfIRQ_UART_TX_1) (void);
static void (* gs_pfIRQ_UART_TX_2) (void);
static void (* gs_pfIRQ_UART_TX_3) (void);
static void (* gs_pfIRQ_UART_TX_4) (void);
static void (* gs_pfIRQ_UART_TX_5) (void);
static void (* gs_pfIRQ_UART_TX_6) (void);
static void (* gs_pfIRQ_UART_TX_7) (void);
static void (* gs_pfIRQ_UART_RX_0) (void);
static void (* gs_pfIRQ_UART_RX_1) (void);
static void (* gs_pfIRQ_UART_RX_2) (void);
static void (* gs_pfIRQ_UART_RX_3) (void);
static void (* gs_pfIRQ_UART_RX_4) (void);
static void (* gs_pfIRQ_UART_RX_5) (void);
static void (* gs_pfIRQ_UART_RX_6) (void);
static void (* gs_pfIRQ_UART_RX_7) (void);

/********************* Initializations ***********************/
void UART_Init(UART0_Type *psUART, uint32_t ui32BaudRate)
{
	switch ((uint32_t) psUART) {
		case (uint32_t) UART0_BASE:
			// enable clock for block UART
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R0;       
			// enable clock for block GPIOA
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R0;      
		#if UART0AHB == 0
			// disable Analog Mode        disabled by default
			GPIOA -> AMSEL &= ~((1 << 0) | (1 << 1)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOA -> AFSEL |= (1 << 0) | (1 << 1);
			// select which special peripheral
			//                  U0Rx   PMC0  U0Tx   PMC1
			GPIOA -> PCTL &= ~(GPIO_PCTL_PA0_M    | GPIO_PCTL_PA1_M);     // default
			GPIOA -> PCTL |=   GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;   // default
			// enable digital function for GPIO Port A Pin 0, 1
			GPIOA -> DEN |= (1 << 0) | (1 << 1);
		#elif UART0AHB == 1
			// enable AHB for GPIO Port A
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTA;
			// disable Analog Mode        disabled by default
			GPIOA_AHB -> AMSEL &= ~((1 << 0) | (1 << 1)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOA_AHB -> AFSEL |= (1 << 0) | (1 << 1);
			// select which special peripheral
			//                    	U0Rx   PMC0  U0Tx   PMC1
			GPIOA_AHB -> PCTL &= ~(GPIO_PCTL_PA0_M    | GPIO_PCTL_PA1_M);     // default
			GPIOA_AHB -> PCTL |=   GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;   // default
			// enable digital function for GPIO Port A Pin 0, 1
			GPIOA_AHB -> DEN |= (1 << 0) | (1 << 1);
		#endif  // UART0AHB
			break;
		case (uint32_t) UART1_BASE:
			// enable clock for block UART1
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R1;       
			// enable clock for block GPIOB
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R1;      
		#if UART1AHB == 0
			// disable Analog Mode        disabled by default
			GPIOB -> AMSEL &= ~((1 << 0) | (1 << 1)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOB -> AFSEL |= (1 << 0) | (1 << 1);
			// select which special peripheral
			//                U1Rx   PMC0  U1Tx   PMC1
			GPIOB -> PCTL &= ~(GPIO_PCTL_PB0_M    | GPIO_PCTL_PB1_M);
			GPIOB -> PCTL |=   GPIO_PCTL_PB0_U1RX | GPIO_PCTL_PB1_U1TX;
			// enable digital function for GPIO Port B Pin 0, 1
			GPIOB -> DEN |= (1 << 0) | (1 << 1);
		#elif UART1AHB == 1
			// enable AHB for GPIO Port B
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTB;
			// disable Analog Mode        disabled by default
			GPIOB_AHB -> AMSEL &= ~((1 << 0) | (1 << 1)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOB_AHB -> AFSEL |= (1 << 0) | (1 << 1);
			// select which special peripheral
			//                    U1Rx   PMC0  U1Tx   PMC1
			GPIOB_AHB -> PCTL &= ~(GPIO_PCTL_PB0_M    | GPIO_PCTL_PB1_M);
			GPIOB_AHB -> PCTL |=   GPIO_PCTL_PB0_U1RX | GPIO_PCTL_PB1_U1TX;
			// enable digital function for GPIO Port B Pin 0, 1
			GPIOB_AHB -> DEN |= (1 << 0) | (1 << 1);
		#endif  // UART1AHB
			break;
		case (uint32_t) UART2_BASE:
			// enable clock for block UART2
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R2;       
			// enable clock for block GPIOD
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R3;      
		#if UART2AHB == 0
			// disable Analog Mode        disabled by default
			GPIOD -> AMSEL &= ~((1 << 6) | (1 << 7)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOD -> AFSEL |= (1 << 6) | (1 << 7);
			// select which special peripheral
			//                U2Rx   PMC6  U2Tx   PMC7
			GPIOD -> PCTL &= ~(GPIO_PCTL_PD6_M    | GPIO_PCTL_PD7_M);
			GPIOD -> PCTL |=   GPIO_PCTL_PD6_U2RX | GPIO_PCTL_PD7_U2TX;
			// enable digital function for GPIO Port D Pin 6, 7
			GPIOD -> DEN |= (1 << 6) | (1 << 7);
		#elif UART2AHB == 1
			// enable AHB for GPIO Port D
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTD;
			// disable Analog Mode        disabled by default
			GPIOD_AHB -> AMSEL &= ~((1 << 6) | (1 << 7)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOD_AHB -> AFSEL |= (1 << 6) | (1 << 7);
			// select which special peripheral
			//                    U2Rx   PMC6  U2Tx   PMC7
			GPIOD_AHB -> PCTL &= ~(GPIO_PCTL_PD6_M    | GPIO_PCTL_PD7_M);
			GPIOD_AHB -> PCTL |=   GPIO_PCTL_PD6_U2RX | GPIO_PCTL_PD7_U2TX;
			// enable digital function for GPIO Port D Pin 6, 7
			GPIOD_AHB -> DEN |= (1 << 6) | (1 << 7);
		#endif  // UART2AHB
			break;
		case (uint32_t) UART3_BASE:
			// enable clock for block UART3
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R3;       
			// enable clock for block GPIOC
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R2;      
		#if UART3AHB == 0
			// disable Analog Mode        disabled by default
			GPIOC -> AMSEL &= ~((1 << 6) | (1 << 7)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOC -> AFSEL |= (1 << 6) | (1 << 7);
			// select which special peripheral
			//                U3Rx   PMC6  U3Tx   PMC7
			GPIOC -> PCTL &= ~(GPIO_PCTL_PC6_M    | GPIO_PCTL_PC7_M);
			GPIOC -> PCTL |=   GPIO_PCTL_PC6_U3RX | GPIO_PCTL_PC7_U3TX;
			// enable digital function for GPIO Port C Pin 6, 7
			GPIOC -> DEN |= (1 << 6) | (1 << 7);
		#elif UART3AHB == 1
			// enable AHB for GPIO Port C
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTC;
			// disable Analog Mode        disabled by default
			GPIOC_AHB -> AMSEL &= ~((1 << 6) | (1 << 7)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOC_AHB -> AFSEL |= (1 << 6) | (1 << 7);
			// see about current level and/or slew rate
			// select which special peripheral
			//                    U3Rx   PMC6  U3Tx   PMC7
			GPIOC_AHB -> PCTL &= ~(GPIO_PCTL_PC6_M    | GPIO_PCTL_PC7_M);
			GPIOC_AHB -> PCTL |=   GPIO_PCTL_PC6_U3RX | GPIO_PCTL_PC7_U3TX;
			// enable digital function for GPIO Port C Pin 6, 7
			GPIOC_AHB -> DEN |= (1 << 6) | (1 << 7);
		#endif  // UART3AHB
			break;
		case (uint32_t) UART4_BASE:
			// enable clock for block UART4
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R4;       
			// enable clock for block GPIOC
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R2;      
		#if UART4AHB == 0
			// disable Analog Mode        disabled by default
			GPIOC -> AMSEL &= ~((1 << 4) | (1 << 5)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOC -> AFSEL |= (1 << 4) | (1 << 5);
			// see about current level and/or slew rate
			// select which special peripheral
			//                U4Rx   PMC4  U4Tx   PMC5
			GPIOC -> PCTL &= ~(GPIO_PCTL_PC4_M    | GPIO_PCTL_PC5_M);
			GPIOC -> PCTL |=   GPIO_PCTL_PC4_U4RX | GPIO_PCTL_PC5_U4TX;
			// enable digital function for GPIO Port C Pin 4, 5
			GPIOC -> DEN |= (1 << 4) | (1 << 5);
		#elif UART4AHB == 1
			// enable AHB for GPIO Port C
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTC;
			// disable Analog Mode        disabled by default
			GPIOC_AHB -> AMSEL &= ~((1 << 4) | (1 << 5)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOC_AHB -> AFSEL |= (1 << 4) | (1 << 5);
			// see about current level and/or slew rate
			// select which special peripheral
			//                    U4Rx   PMC4  U4Tx   PMC5
			GPIOC_AHB -> PCTL &= ~(GPIO_PCTL_PC4_M    | GPIO_PCTL_PC5_M);
			GPIOC_AHB -> PCTL |=   GPIO_PCTL_PC4_U4RX | GPIO_PCTL_PC5_U4TX;
			// enable digital function for GPIO Port C Pin 4, 5
			GPIOC_AHB -> DEN |= (1 << 4) | (1 << 5);
		#endif  // UART4AHB
			break;
		case (uint32_t) UART5_BASE:
			// enable clock for block UART5
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R5;       
			// enable clock for block GPIOE
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R4;      
		#if UART5AHB == 0
			// disable Analog Mode        disabled by default
			GPIOE -> AMSEL &= ~((1 << 4) | (1 << 5)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOE -> AFSEL |= (1 << 4) | (1 << 5);
			// see about current level and/or slew rate
			// select which special peripheral
			//                U5Rx   PMC4  U5Tx   PMC5
			GPIOE -> PCTL &= ~(GPIO_PCTL_PE4_M    | GPIO_PCTL_PE5_M);
			GPIOE -> PCTL |=   GPIO_PCTL_PE4_U5RX | GPIO_PCTL_PE5_U5TX;
			// enable digital function for GPIO Port E Pin 4, 5
			GPIOE -> DEN |= (1 << 4) | (1 << 5);
		#elif UART5AHB == 1
			// enable AHB for GPIO Port E
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTE;
			// disable Analog Mode        disabled by default
			GPIOE_AHB -> AMSEL &= ~((1 << 4) | (1 << 5)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOE_AHB -> AFSEL |= (1 << 4) | (1 << 5);
			// see about current level and/or slew rate
			// select which special peripheral
			//                    U5Rx   PMC4  U5Tx   PMC5
			GPIOE_AHB -> PCTL &= ~(GPIO_PCTL_PE4_M    | GPIO_PCTL_PE5_M);
			GPIOE_AHB -> PCTL |=   GPIO_PCTL_PE4_U5RX | GPIO_PCTL_PE5_U5TX;
			// enable digital function for GPIO Port E Pin 4, 5
			GPIOE_AHB -> DEN |= (1 << 4) | (1 << 5);
		#endif  // UART5AHB
			break;
		case (uint32_t) UART6_BASE:
			// enable clock for block UART6
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R6;       
			// enable clock for block GPIOD
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R3;      
		#if UART6AHB == 0
			// disable Analog Mode        disabled by default
			GPIOD -> AMSEL &= ~((1 << 4) | (1 << 5)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOD -> AFSEL |= (1 << 4) | (1 << 5);
			// see about current level and/or slew rate
			// select which special peripheral
			//                U6Rx   PMC4  U6Tx   PMC5
			GPIOD -> PCTL &= ~(GPIO_PCTL_PD4_M    | GPIO_PCTL_PD5_M);
			GPIOD -> PCTL |=   GPIO_PCTL_PD4_U6RX | GPIO_PCTL_PD5_U6TX;
			// enable digital function for GPIO Port D Pin 4, 5
			GPIOD -> DEN |= (1 << 4) | (1 << 5);
		#elif UART6AHB == 1
			// enable AHB for GPIO Port D
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTD;
			// disable Analog Mode        disabled by default
			GPIOD_AHB -> AMSEL &= ~((1 << 4) | (1 << 5)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOD_AHB -> AFSEL |= (1 << 4) | (1 << 5);
			// see about current level and/or slew rate
			// select which special peripheral
			//                    U6Rx   PMC4  U6Tx   PMC5
			GPIOD_AHB -> PCTL &= ~(GPIO_PCTL_PD4_M    | GPIO_PCTL_PD5_M);
			GPIOD_AHB -> PCTL |=   GPIO_PCTL_PD4_U6RX | GPIO_PCTL_PD5_U6TX;
			// enable digital function for GPIO Port D Pin 4, 5
			GPIOD_AHB -> DEN |= (1 << 4) | (1 << 5);
		#endif  // UART6AHB
			break;
		case (uint32_t) UART7_BASE:
			// enable clock for block UART7
			SYSCTL -> RCGCUART |= SYSCTL_RCGCUART_R7;       
			// enable clock for block GPIOE
			SYSCTL -> RCGCGPIO |= SYSCTL_RCGCGPIO_R4;      
		#if UART7AHB == 0
			// disable Analog Mode        disabled by default
			GPIOE -> AMSEL &= ~((1 << 0) | (1 << 1)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOE -> AFSEL |= (1 << 0) | (1 << 1);
			// see about current level and/or slew rate
			// select which special peripheral
			//                  U7Rx   PMC0  U7Tx   PMC1
			GPIOE -> PCTL &= ~(GPIO_PCTL_PE0_M    | GPIO_PCTL_PE1_M);     // default
			GPIOE -> PCTL |=   GPIO_PCTL_PE0_U7RX | GPIO_PCTL_PE1_U7TX;   // default
			// enable digital function for GPIO Port E Pin 0, 1
			GPIOE -> DEN |= (1 << 0) | (1 << 1);
		#elif UART7AHB == 1
			// enable AHB for GPIO Port E
			SYSCTL -> GPIOHBCTL |= SYSCTL_GPIOHBCTL_PORTE;
			// disable Analog Mode        disabled by default
			GPIOE_AHB -> AMSEL &= ~((1 << 0) | (1 << 1)); 
			// enable assochiated peripheral functionality(alternative function)
			GPIOE_AHB -> AFSEL |= (1 << 0) | (1 << 1);
			// see about current level and/or slew rate
			// select which special peripheral
			//                    U7Rx   PMC0  U7Tx   PMC1
			GPIOE_AHB -> PCTL &= ~(GPIO_PCTL_PE0_M    | GPIO_PCTL_PE1_M);     // default
			GPIOE_AHB -> PCTL |=   GPIO_PCTL_PE0_U7RX | GPIO_PCTL_PE1_U7TX;   // default
			// enable digital function for GPIO Port E Pin 0, 1
			GPIOE_AHB -> DEN |= (1 << 0) | (1 << 1);
		#endif  // UART7AHB
			break;
		default:
			break;
	}
	
	
    psUART -> CTL &= UART_CTL_UARTEN;        // disable UART
    // setting baud rate
    psUART -> IBRD = (uint32_t)UART_BRDVAL;
    psUART -> FBRD = (uint32_t)((UART_BRDVAL - (uint32_t)(psUART -> IBRD)) * 64 + 0.5);
    psUART -> LCRH = UART_LCRH_WLEN_8 | UART_LCRH_FEN;  // 8-bit, no parity, 1 stop bit, FIFOs
    psUART -> CC = UART_CC_CS_SYSCLK;     // system clock (default)
    /*
	// enable            TX          RX           Masked Interrupts
    psUART -> IM |= (1U << 5) | (1U << 4);
    // set the interrupt number a priority      // the priority is assigned into the 3 MSBs
    //       IRQn  Priority                     // priority register number = interrupt number / 4
    NVIC -> IP[5U] = (4U << 5);
    // enable the interrupt in the suitable register according to its number in the vector table
    NVIC -> ISER[0U] |= (1U << 5);
    __enable_irq();
	*/
    psUART -> CTL |= UART_CTL_RXE | UART_CTL_TXE;
    
    psUART -> CTL |= UART_CTL_UARTEN;        // enable UART
}

/********************* TX, RX using polling **********************/
void UART_TX_Byte_P (UART0_Type *psUART, uint8_t ui8Data)
{
    while((psUART -> FR & (1 << 5)) != 0)      // wait until TX FIFO is "NOT" Full
		;
	psUART -> DR = ui8Data;
}
uint8_t UART_RX_Byte_P (UART0_Type *psUART)
{
    while((psUART -> FR & (1 << 4)) != 0)      // wait until RX FIFO is "NOT" Empty
		;
	return (uint8_t)(psUART -> DR);
}
void UART_TX_Str_P (UART0_Type *psUART, char *pui8Str) {
	uint8_t ui8StrIndex = 0;

	while ('\0' != pui8Str[ui8StrIndex]) {
		UART_TX_Byte_P (psUART, pui8Str[ui8StrIndex]);
		++ui8StrIndex;
	}
}
void UART_TX_Data_P (UART0_Type *psUART, uint8_t *pui8Data, uint32_t ui32DataLen) {
	uint8_t ui8DataIndex = 0;

	for (ui8DataIndex = 0; ui8DataIndex < ui32DataLen; ++ui8DataIndex) {
		UART_TX_Byte_P (psUART, pui8Data[ui8DataIndex]);
	}
}
/******************* TX, RX without checking *********************/
void UART_TX_Byte (UART0_Type *psUART, uint8_t ui8Data) {
	psUART -> DR = ui8Data;
}
uint8_t UART_RX_Byte (UART0_Type *psUART) {
	return (uint8_t)(psUART -> DR);
}
/************* RX with checking only (no polling) ****************/
bool UART_RX_Byte_Unblock (UART0_Type *psUART, uint8_t *pui8Data) {
	bool bStatus = false;

	if (  (psUART -> FR & (1 << 4))  ==  0  ) {     // check if RX FIFO is "NOT" Empty
		*pui8Data = psUART -> DR;
		bStatus = true;
	}
	else {
		bStatus = false;
	}
	return bStatus;
}
/****** IRQs "Setters", when called - Here - are "Getters" ******/
void UART_SetIRQ_TX (UART0_Type *psUART, void (*pf) (void)) {
	switch ((uint32_t) psUART) {
        case (uint32_t) UART0_BASE:
            gs_pfIRQ_UART_TX_0 = pf;
            break;
        case (uint32_t) UART1_BASE:
            gs_pfIRQ_UART_TX_1 = pf;
            break;
        case (uint32_t) UART2_BASE:
            gs_pfIRQ_UART_TX_2 = pf;
            break;
        case (uint32_t) UART3_BASE:
            gs_pfIRQ_UART_TX_3 = pf;
            break;
        case (uint32_t) UART4_BASE:
            gs_pfIRQ_UART_TX_4 = pf;
            break;
        case (uint32_t) UART5_BASE:
            gs_pfIRQ_UART_TX_5 = pf;
            break;
        case (uint32_t) UART6_BASE:
            gs_pfIRQ_UART_TX_6 = pf;
            break;
        case (uint32_t) UART7_BASE:
            gs_pfIRQ_UART_TX_7 = pf;
            break;
        default:
            break;
    }
}
void UART_SetIRQ_RX (UART0_Type *psUART, void (*pf) (void)) {
	switch ((uint32_t) psUART) {
        case (uint32_t) UART0_BASE:
            gs_pfIRQ_UART_RX_0 = pf;
            break;
        case (uint32_t) UART1_BASE:
            gs_pfIRQ_UART_RX_1 = pf;
            break;
        case (uint32_t) UART2_BASE:
            gs_pfIRQ_UART_RX_2 = pf;
            break;
        case (uint32_t) UART3_BASE:
            gs_pfIRQ_UART_RX_3 = pf;
            break;
        case (uint32_t) UART4_BASE:
            gs_pfIRQ_UART_RX_4 = pf;
            break;
        case (uint32_t) UART5_BASE:
            gs_pfIRQ_UART_RX_5 = pf;
            break;
        case (uint32_t) UART6_BASE:
            gs_pfIRQ_UART_RX_6 = pf;
            break;
        case (uint32_t) UART7_BASE:
            gs_pfIRQ_UART_RX_7 = pf;
            break;
        default:
            break;
    }
}


/**************************** IRQs ******************************/
void UART0_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART0 -> MIS;
    /* clear all asserted interrupts */
    UART0 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_0 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_0 ();
    }
}
void UART1_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART1 -> MIS;
    /* clear all asserted interrupts */
    UART1 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_1 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_1 ();
    }
}
void UART2_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART2 -> MIS;
    /* clear all asserted interrupts */
    UART2 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_2 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_2 ();
    }
}
void UART3_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART3 -> MIS;
    /* clear all asserted interrupts */
    UART3 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_3 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_3 ();
    }
}
void UART4_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART4 -> MIS;
    /* clear all asserted interrupts */
    UART4 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_4 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_4 ();
    }
}
void UART5_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART5 -> MIS;
    /* clear all asserted interrupts */
    UART5 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_5 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_5 ();
    }
}
void UART6_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART6 -> MIS;
    /* clear all asserted interrupts */
    UART6 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_6 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_6 ();
    }
}
void UART7_Handler (void) {
    /* get Interrupt Status (Masked: enabled to fire the interrupt) */
    uint32_t ui32Status = UART7 -> MIS;
    /* clear all asserted interrupts */
    UART7 -> ICR = ui32Status;
    
    if (ui32Status & UART_MIS_TXMIS) {
        gs_pfIRQ_UART_TX_7 ();
    }
    if (ui32Status & UART_MIS_RXMIS) {
        gs_pfIRQ_UART_RX_7 ();
    }
}
