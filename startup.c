#include <stdint.h>

// Defined in the linker script
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

// Main function prototype
extern int main(void);

// 1. Define the Default Handler (Infinite Loop for unhandled interrupts)
void Default_Handler(void) {
    while (1);
}

// 2. Declare the prototype for our specific ISR
// "Weak" alias means: if we don't define this function in main.c, use Default_Handler.
void GPIOF_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Timer0A_Handler(void) __attribute__((weak, alias("Default_Handler")));

// Reset Handler: The first function to run
void Reset_Handler(void) {
    // 1. Copy .data section from FLASH to RAM
    uint32_t *pSrc = &_etext;
    uint32_t *pDest = &_data;
    while (pDest < &_edata) {
        *pDest++ = *pSrc++;
    }

    // 2. Zero out .bss section in RAM
    pDest = &_bss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }

    // 3. Jump to main
    main();
}


// 3. The Vector Table
// We must place GPIOF_Handler in the correct physical slot (Index 46)
__attribute__((section(".isr_vector")))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)0x20008000), // Stack Pointer
    Reset_Handler,                          // Reset Vector
    Default_Handler, // NMI
    Default_Handler, // Hard Fault
    Default_Handler, // MPU Fault
    Default_Handler, // Bus Fault
    Default_Handler, // Usage Fault
    0, 0, 0, 0,      // Reserved
    Default_Handler, // SVCall
    Default_Handler, // Debug Monitor
    0,               // Reserved
    Default_Handler, // PendSV
    Default_Handler, // SysTick
    // --- External Interrupts start here (IRQ0) ---
    Default_Handler, // GPIO Port A
    Default_Handler, // GPIO Port B
    Default_Handler, // GPIO Port C
    Default_Handler, // GPIO Port D
    Default_Handler, // GPIO Port E
    Default_Handler, // UART0
    Default_Handler, // UART1
    Default_Handler, // SSI0
    Default_Handler, // I2C0
    Default_Handler, // PWM0 Fault
    Default_Handler, // PWM0 Generator 0
    Default_Handler, // PWM0 Generator 1
    Default_Handler, // PWM0 Generator 2
    Default_Handler, // QEI0
    Default_Handler, // ADC0 Sequence 0
    Default_Handler, // ADC0 Sequence 1
    Default_Handler, // ADC0 Sequence 2
    Default_Handler, // ADC0 Sequence 3
    Default_Handler, // Watchdog timer
    Timer0A_Handler, // Timer 0 subtimer A
    Default_Handler, // Timer 0 subtimer B
    Default_Handler, // Timer 1 subtimer A
    Default_Handler, // Timer 1 subtimer B
    Default_Handler, // Timer 2 subtimer A
    Default_Handler, // Timer 2 subtimer B
    Default_Handler, // Analog Comparator 0
    Default_Handler, // Analog Comparator 1
    Default_Handler, // Analog Comparator 2
    Default_Handler, // System Control (PLL, OSC, BO)
    Default_Handler, // Flash Control
    GPIOF_Handler    // GPIO Port F (IRQ 30) <--- WE ARE HERE
};