#include "tm4c123gh6pm_minimal.h"

// --- 1. Send a Single Character ---
void UART_WriteChar(char c) {

    // Step A: Wait until the Transmit FIFO is not full.
    // We check the TXFF flag (Bit 5 of UARTFR).
    // If (FR & 0x20) is not zero, the FIFO is full. We spin/wait.
    while ( (UART0->FR & (1U << 5)) != 0){

    }

    // Step B: Write the character to Data Register
    UART0->DR = c;

}

// --- 2. Send a String ---
void UART_WriteString(char *str) {
    // Loop through the string until we hit null terminator '\0'
    while (*str != '\0') {
        // Send the current character
        UART_WriteChar(*str);
        // Move the pointer to the next character
        str++;
    }

}


int main() {
    // --- Initialization ---

    // 1. Enable Clocks: UART and GPIOA
    SYSCTL_RCGCUART |= (1U << 0);
    SYSCTL_RCGCGPIO |= (1U << 0);
    volatile uint32_t delay;
    delay = SYSCTL_RCGCUART;
    (void)delay;

    // 2. Configure GPIO Alternate function (PA0=Rx, PA1=Tx)
    GPIOA->AFSEL |= ((1U << 0) | (1U << 1)); // Enable Alt Function

    // Configure PCTL for PA0 and PA1 (Encoding 1 for UART)
    // Clear first byte (pins 0 and pins 1) then set to 0x11;
    GPIOA->PCTL &= ~0x000000FF;
    GPIOA->PCTL |=  0x00000011;

    GPIOA->DEN |= ((1U << 0) | (1U << 1)); // Enable Digital

   // 3. Configure UART0
    UART0->CTL &= ~(1U << 0); // Disable UART (Bit 0 is UARTEN)

    // Configure the baud rate: 115200
    // BRD = 16,000,000 / (16 * 115200) = 8.68
    // int = 8 , fraction  0.68 * 64 + 0.5 = 44
    UART0->IBRD = 8;
    UART0->FBRD = 44;

    // Line Control: 8-bit, No Parity, 1 Stop Bit, FIFOs Enabled
    // WLEN (Bits 6:5) = 0x3 (8 bits)
    // FEN (Bit 4) = 1 (Enable FIFOs)
    // FEN (Bit 4) = 1 (Enable FIFOs)
    UART0->LCRH = 0x70;

    // Enable UART, TX, and RX
    // UARTEN (Bit 0) , TXE (Bit 8), RXE (Bit 9)
    UART0->CTL |= ((1U << 0) | (1U << 8) | (1U << 9));



    // --- Main loop ---
    while(1) {
        UART_WriteString("Hello from Tiva C!\r\n");
        

        // Delay so we don't flood the terminal
        for(int i = 0; i < 1000000; i++);
    }

}