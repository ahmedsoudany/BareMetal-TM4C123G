#include "tm4c123gh6pm_minimal.h"
int main() {
    // 1. Enable Clock for Port F (Bit 5)
    // 0x400FE608 is the address of the RCGCGPIO register
    SYSCTL_RCGCGPIO |= (1U << 5); 
    
    // 2. Small delay
    volatile unsigned int delay = 0;
    delay = SYSCTL_RCGCGPIO;
    (void)delay;
        
    // 3. Set direction to output (Bit 1()
    GPIOF->DIR |= (1U << 1);
    
    // 4. Enable digital function (Bit 1)
    GPIOF->DEN |= (1U << 1);

    while(1) {
        // RED ON
        GPIOF->DATA = (1U << 1);
        for (int i = 0; i < 1000000; i++);
        
        // RED OFF
        GPIOF->DATA = 0;
        for (int i = 0; i < 1000000; i++);
    }
}