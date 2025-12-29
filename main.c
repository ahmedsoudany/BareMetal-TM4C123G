int main() {
    // 1. Enable Clock for Port F (Bit 5)
    // 0x400FE608 is the address of the RCGCGPIO register
    *((volatile unsigned int *)0x400FE608) |= (1U << 5); 
    
    // 2. Small delay
    volatile unsigned int delay = 0;
    delay = *((volatile unsigned int *)0x400FE608);
    (void)delay;
        
    // 3. Set direction to output (Bit 1 - Bit 2 -Bit 3)
    *((volatile unsigned int *)0x40025400) |= ( (1U << 1) | (1U << 2) | (1U << 3) );
    
    // 4. Enable digital function (Bit 1)
    *((volatile unsigned int *)0x4002551C) |= 0x0E;

    while(1) {
        // RED ON
        *((volatile unsigned int *)0x400253FC) = (1U << 1);
        for (int i = 0; i < 1000000; i++);
        
        // RED OFF
        *((volatile unsigned int *)0x400253FC) = 0;
        for (int i = 0; i < 1000000; i++);

         // Blue ON
        *((volatile unsigned int *)0x400253FC) = (1U << 2);
        for (int i = 0; i < 1000000; i++);
        
        // Blue OFF
        *((volatile unsigned int *)0x400253FC) = 0;
        for (int i = 0; i < 1000000; i++);

         // Green ON
        *((volatile unsigned int *)0x400253FC) = (1U << 3);
        for (int i = 0; i < 1000000; i++);
        
        // Green OFF
        *((volatile unsigned int *)0x400253FC) = 0;
        for (int i = 0; i < 1000000; i++);

         // WHITE (ALL ON)
        *((volatile unsigned int *)0x400253FC) = 0x0E;
        for (int i = 0; i < 1000000; i++);
        
        //  OFF
        *((volatile unsigned int *)0x400253FC) = 0;
        for (int i = 0; i < 1000000; i++);


    }
}
