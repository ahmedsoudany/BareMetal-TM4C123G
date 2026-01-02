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
    // 1. Enable clock for GPIO
    SYSCTL_RCGCGPIO |= (1U << 5); // Enable Port F
    volatile unsigned int delay = SYSCTL_RCGCGPIO;
    (void)delay;

     // 2. Enable Clock for Timer 0
    SYSCTL_RCGCTIMER |= (1U << 0); 
    delay = SYSCTL_RCGCTIMER; // Wait for clock to stabilize
    (void)delay;

    // 3. Disable PF1
    GPIOF->DIR &= ~(1U << 1);      

    // 4. Enable Alternate Function
    GPIOF->AFSEL |= (1U << 1); 

    // 5. Select Function 7
    // Clear the Register
    GPIOF->PCTL &= ~0x000000F0;
    GPIOF->PCTL |= (0x00000070);

    GPIOF->DEN |= (1U << 1);

    // 2. Disable Timer0B during setup (Clear Bit 8)
    TIMER0->CTL &= ~(1U << 8);

    // 3. Configure for 16-bit mode (Write 0x04)
    TIMER0->CFG = 0x4;

    // 4. Configure for PW< Mode (Write 0x2 to TAMR)
    // Bit 1:0 = 0x2 (Periodic)
    // Bit 4 = 1 (Count Up? No, keep 0 for Count Down)
    TIMER0->TBMR =0x000000A;

    // We want 1kHz(1000 cycle per second)
    // 5. Set the Load Value (16,000,000 / 1000) = 16,000
    // write 1600 - 1 
    TIMER0->TBILR = 16000 - 1;

    // 6.We want 50% brightness initially.
    // Match Value = Load_Value * (1 - Duty_Cycle).
    // For 50%: 16000 * 0.5 = 8000
    TIMER0->TBMATCHR = 8000;

    // 8. Enable Timer0B (Set Bit 0)
    TIMER0->CTL |= (1U << 8);

   int brightness = 0;
    int step = 100;

    while(1) {
        // 1. Update the Match Register
        // Remember: Match = Load * (1 - Duty). 
        // Higher Match = Lower Brightness (LED is OFF longer).
        // Lower Match = Higher Brightness (LED is ON longer).
        TIMER0->TBMATCHR = brightness;

        // 2. Change brightness
        brightness += step;

        // 3. Reverse direction at limits
        if (brightness >= 15000) step = -100; // Go down
        if (brightness <= 100)   step = 100;  // Go up

        // 4. Small delay so our eyes can see the fade
        for(int i=0; i<10000; i++);
    }
}