#include "tm4c123gh6pm_minimal.h"

// --- The Timer ISR ---
// This runs automatically every 0.5 seconds
void Timer0A_Handler(void) {

    // 1. Check: Did a Time-Out occur? (Bit 0 of Masked Interrupt Status)
    if(TIMER0->MIS & (1U << 0)) {
        // 2. Clear the Interrupt Flag (TATOCINT - Bit 0)
        // If we don't do this, it will loop forever!
        TIMER0->ICR |= (1U << 0);

        // 3. Toggle Red LED (PF1)
        GPIOF->DATA ^= (1U << 1);

    }
}

int main() {
    // --- GPIO Setup (Standard Blinky Stuff) ---
    SYSCTL_RCGCGPIO |= (1U << 5); // Enable Port F
    volatile unsigned int delay = SYSCTL_RCGCGPIO;
    (void)delay;

    GPIOF->DIR |= (1U << 1);      // PF1 Output
    GPIOF->DEN |= (1U << 1);      // PF1 Digital Enable

    // --- Timer Setup (The New Stuff) ---
    
    // 1. Enable Clock for Timer 0
    SYSCTL_RCGCTIMER |= (1U << 0); 
    delay = SYSCTL_RCGCTIMER; // Wait for clock to stabilize
    (void)delay;

    // 2. Disable Timer 0A during setup (Clear Bit 0)
    TIMER0->CTL &= ~(1U << 0);

    // 3. Configure for 32-bit mode (Write 0 to bits 0-2)
    TIMER0->CFG = 0x00000000;

    // 4. Configure for Periodic Mode (Write 0x2 to TAMR)
    // Bit 1:0 = 0x2 (Periodic)
    // Bit 4 = 1 (Count Up? No, keep 0 for Count Down)
    TIMER0->TAMR = 0x00000002;

    // 5. Set the Load Value (16,000,000 cycles = 1 second)
    // We want 0.5 seconds -> 8,000,000
    TIMER0->TAILR = 8000000;

    // 6. Enable Timer 0A Time-Out Interrupt (Bit 0)
    TIMER0->IMR |= (1U << 0);

    // 7. Enable IRQ 19 in NVIC
    NVIC_EN0 |= (1U << 19);

    // 8. Enable Timer 0A (Set Bit 0)
    TIMER0->CTL |= (1U << 0);

    // --- Main Loop ---
    while(1) {
        // The CPU is free! 
        // The LED is blinking automatically in the background.
    }
}