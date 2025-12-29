#ifndef TM4C123GH6PM_MINIMAL_H
#define TM4C123GH6PM_MINIMAL_H

#include <stdint.h>

// ============================================================================
// GPIO Peripheral Definition
// ============================================================================

typedef struct {
    // Offset 0x000 to 0x3FC: Data Register
    // The Tiva C has a special feature where address bits [9:2] act as a mask.
    // To write to ALL bits, we use offset 0x3FC.
    // 0x3FC / 4 = 255. So we need an array of 256 integers.
    // The last one (index 255) corresponds to offset 0x3FC.
    volatile uint32_t DATA_BITS[255]; 
    volatile uint32_t DATA;          // Offset 0x3FC (The one we use)
    
    // Offset 0x400: Direction Register
    volatile uint32_t DIR;           
    
    // Offset 0x404: Interrupt Sense (We skip this for now)
    volatile uint32_t IS;
    
    // Offset 0x408 to 0x51C: There is a gap here!
    // Target Offset: 0x51C (DEN)
    // Current Offset: 0x408
    // Gap Size: 0x51C - 0x408 = 0x114 bytes = 276 bytes.
    // 276 bytes / 4 bytes per int = 69 integers.
    volatile uint32_t RESERVED[69];  // Padding to reach 0x51C
    
    // Offset 0x51C: Digital Enable
    volatile uint32_t DEN;           

} GPIO_Type;

// ============================================================================
// Memory Mapping (The "Pointer Cast" Trick)
// ============================================================================

// 1. Define the Base Address for Port F
#define GPIO_PORTF_BASE  0x40025000

// 2. Cast that address to our Struct Pointer
#define GPIOF  ((GPIO_Type *)GPIO_PORTF_BASE)


// ============================================================================
// System Control (Clock) Definition
// ============================================================================

// For the clock, we'll just define the specific register pointer for now
// Base: 0x400FE000, Offset: 0x608
#define SYSCTL_RCGCGPIO  (*((volatile uint32_t *)0x400FE608))

#endif // TM4C123GH6PM_MINIMAL_H