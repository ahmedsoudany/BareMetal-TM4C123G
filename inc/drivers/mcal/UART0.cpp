#include "UART0.h"



	/* UART_REGISTERS */
#define UART0_CTL 			(*((volatile uint32_t *)0x4000C030))
#define UART0_IBRD 			(*((volatile uint32_t *)0x4000C024))
#define UART0_FBRD 			(*((volatile uint32_t *)0x4000C028))
#define UART0_LCRH 			(*((volatile uint32_t *)0x4000C02C))
#define UART0_CC 			(*((volatile uint32_t *)0x4000CFC8))
#define UART0_FR 			(*((volatile uint32_t *)0x4000C018))
#define UART0_DR 			(*((volatile uint32_t *)0x4000C000))


	/* GPIOA_REGISTERS */
#define GPIOA_AFSEL 		(*(volatile uint32_t *)0x40004420)
#define GPIOA_DEN 			(*(volatile uint32_t *)0x4000451C)
#define GPIOA_PTCL 			(*(volatile uint32_t *)0x4000452C)

/* System Control REGISTERS */
#define SYSCTL_RCGCGPIO  	(*((volatile uint32_t *)0x400FE608))

#define SYSCTL_RCGCUART 	(*(volatile uint32_t *)0x400FE618)

void UART0::init() {
	
	// 1. Enable UART0 and GPIOA.
	SYSCTL_RCGCGPIO |= (1U << 0);
	SYSCTL_RCGCUART |= (1U << 0);

	// dummy read
	volatile uint32_t delay = SYSCTL_RCGCUART;
	(void)delay;

	// 2. --- GPIO_Config --- 
	// A. Switch to "Alternate Function Mode"
	GPIOA_AFSEL |= ((1U << 1) | (1U << 0));

	// B. Set the value of PCTL
	// To enable the UART0 we must write 1 to PIN0 and PIN1
	GPIOA_PTCL &= ~0x000000FF;
	GPIOA_PTCL |= 0x11;

	// C. SET Digital ENABLE: for PA0 and PA1.
	GPIOA_DEN |= ((1U << 1) | (1U << 0));

	// 3. --- UART_Config ----
	// A. DISABLE UART0
	UART0_CTL &= ~(1U << 0);

	// B. WRITE BAUD RATE
	UART0_IBRD = 8;
	UART0_FBRD = 44;

	// C. WRITE LINE CONTROL
	//  8 Data bits, No Parity, 1 Stop bit (8N1), FIFO enabled
	UART0_LCRH |= ((1U << 4) | (3U << 5));

	// D. SET CLOCK SOURCE
	UART0_CC = 0;

	// F. ENABLE UART0
	UART0_CTL |= ((1U << 0) | (1U << 8) | (1U << 9));

}


void UART0::writeChar(char C) {
	
	// Wait until UART is not full
	while ((UART0_FR & (1U << 5)) != 0);

	UART0_DR = C;
	
}

void UART0::writeString(const char* str) {

	// until we reach the last char
	while(*str != '\0') {
		// Send the current char
		writeChar(*str);
		// move to the next char
		str++;
	}
	
}





