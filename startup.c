#include <stdint.h>

// Defined in the linker script
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

// Main function prototype
extern int main(void);

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

// The Vector Table (Must be at 0x00000000)
__attribute__((section(".isr_vector")))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)0x20008000), // Stack Pointer (Top of RAM)
    Reset_Handler                           // Reset Vector
};