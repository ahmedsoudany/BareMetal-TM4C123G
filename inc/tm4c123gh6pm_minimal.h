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
    
    volatile uint32_t IS;            // Offset 0x404 (Interrupt Sense)
    volatile uint32_t IBE;           // Offset 0x408 (Interrupt Both Edges)
    volatile uint32_t IEV;           // Offset 0x40C (Interrupt Event)
    volatile uint32_t IM;            // Offset 0x410 (Interrupt Mask)
    volatile uint32_t RIS;           // Offset 0x414 (Raw Interrupt Status)
    volatile uint32_t MIS;           // Offset 0x418 (Masked Interrupt Status)
    volatile uint32_t ICR;           // Offset 0x41C (Interrupt Clear)
    volatile uint32_t AFSEL;         // Offset 0x420 (Alt Function)
    
    // Gap from 0x424 to 0x510
    // 0x510 - 0x424 = 0xEC = 236 bytes = 59 ints
    volatile uint32_t RESERVED_2[59];

    volatile uint32_t PUR;           // Offset 0x510 (Pull-Up Select)
    
    // Gap from 0x514 to 0x51C
    // 0x51C - 0x514 = 8 bytes = 2 ints
    volatile uint32_t RESERVED_3[2];

    volatile uint32_t DEN;           // Offset 0x51C
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

// NVIC Enable Register 0 (Controls IRQ 0 to 31)
// Base: 0xE000E000, Offset: 0x100
#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))

#endif // TM4C123GH6PM_MINIMAL_H