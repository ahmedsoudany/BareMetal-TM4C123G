#include "tm4c123gh6pm_minimal.h"

// --- 1. The Interrupt Service Routine (ISR) ---
// This function is called AUTOMATICALLY by the hardware when the button is pressed.
// The name MUST match the one in startup.c exactly.
void GPIOF_Handler(void) {
    // A. Check if the interrupt was caused by Pin 4 (Switch 1)
    // We check the Masked Interrupt Status (MIS) register.
    if (GPIOF->MIS & (1U << 4)) {
        
        // B. Clear the Interrupt Flag (CRITICAL!)
        // If we don't do this, the ISR will run again immediately forever.
        GPIOF->ICR |= (1U << 4);

        // C. Toggle the Red LED (Pin 1)
        // We use XOR (^) to flip the bit.
        // Note: We write to the "All Bits" data address (0x3FC)
        GPIOF->DATA ^= (1U << 1);
    }
}

int main() {
    // --- Hardware Setup ---

    // 1. Enable Clock for Port F
    SYSCTL_RCGCGPIO |= (1U << 5); 
    
    // 2. Small delay
    volatile unsigned int delay = SYSCTL_RCGCGPIO;
    (void)delay;

    // 3. Set PF1 (LED) as Output
    GPIOF->DIR |= (1U << 1);

    // 4. Set PF4 (Switch) as Input (Clear bit 4)
    GPIOF->DIR &= ~(1U << 4);

    // 5. Enable Pull-Up Resistor for PF4 (Switch needs this!)
    GPIOF->PUR |= (1U << 4);
    
    // 6. Enable Digital Function for PF1 and PF4
    GPIOF->DEN |= ((1U << 1) | (1U << 4));

    // --- Interrupt Configuration ---

    // 7. Disable interrupts on PF4 locally while we configure
    GPIOF->IM &= ~(1U << 4);

    // 8. Configure Trigger: Edge Sensitive, Not Both Edges, Falling Edge
    GPIOF->IS  &= ~(1U << 4); // 0 = Edge sensitive
    GPIOF->IBE &= ~(1U << 4); // 0 = Controlled by IEV
    GPIOF->IEV &= ~(1U << 4); // 0 = Falling Edge (Press)

    // 9. Clear any prior fake interrupts
    GPIOF->ICR |= (1U << 4);

    // 10. Enable (Unmask) interrupt for PF4 locally
    GPIOF->IM |= (1U << 4);

    // 11. Enable IRQ 30 in the NVIC (The Master Switch)
    // IRQ 30 corresponds to GPIO Port F
    NVIC_EN0 |= (1U << 30);

    // --- The Main Loop ---
    while(1) {
        // Do NOTHING.
        // The CPU can sleep here, or do complex math.
        // The LED toggling happens "in the background" via the ISR.
    }
}