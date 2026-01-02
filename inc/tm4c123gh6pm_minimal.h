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
    volatile uint32_t LOCK;          // Offset 0x520 (Lock)
    volatile uint32_t CR;            // Offset 0x524 (Commit)
    volatile uint32_t AMSEL;         // Offset 0x528 (Analog Mode)
    volatile uint32_t PCTL;          // Offset 0x52C (Port Control) - TARGET ACQUIRED
    volatile uint32_t ADCCTL;        // Offset 0x530 (ADC Control)
    volatile uint32_t DMACTL;        // Offset 0x534 (DMA Control)
} GPIO_Type;

// ============================================================================
// Memory Mapping (The "Pointer Cast" Trick)
// ============================================================================

// 1. Define the Base Address for Port F
#define GPIO_PORTF_BASE  0x40025000

// 2. Cast that address to our Struct Pointer
#define GPIOF  ((GPIO_Type *)GPIO_PORTF_BASE)

// ============================================================================
// Timer Peripheral Definition
// ============================================================================

typedef struct {
    volatile uint32_t CFG;           // 0x000: Configuration
    volatile uint32_t TAMR;          // 0x004: Timer A Mode
    volatile uint32_t TBMR;          // 0x008: Timer B Mode (We won't use this yet, but it fills the gap)
    volatile uint32_t CTL;           // 0x00C: Control
    volatile uint32_t SYNC;          // 0x010: Synchronize (Fills gap)
    volatile uint32_t RESERVED[1];   // 0x014: Reserved space (4 bytes)
    volatile uint32_t IMR;           // 0x018: Interrupt Mask
    volatile uint32_t RIS;           // 0x01C: Raw Interrupt Status
    volatile uint32_t MIS;           // 0x020: Masked Interrupt Status
    volatile uint32_t ICR;           // 0x024: Interrupt Clear
    volatile uint32_t TAILR;         // 0x028: Timer A Interval Load
    volatile uint32_t TBILR;         // 0x02C (Timer B Load)
    volatile uint32_t TAMATCHR;      // 0x030
    volatile uint32_t TBMATCHR;      // 0x034 (Timer B Match)
} Timer_Type;

// ============================================================================
// Timer Memory Mapping
// ============================================================================

#define TIMER0_BASE      0x40030000
#define TIMER0           ((Timer_Type *)TIMER0_BASE)



// ============================================================================
// System Control (Clock) Definition
// ============================================================================

// For the clock, we'll just define the specific register pointer for now
// Base: 0x400FE000, Offset: 0x608
#define SYSCTL_RCGCGPIO  (*((volatile uint32_t *)0x400FE608))

// System Control for Timers (Base 0x400FE000 + Offset 0x604)
#define SYSCTL_RCGCTIMER (*((volatile uint32_t *)0x400FE604))

// NVIC Enable Register 0 (Controls IRQ 0 to 31)
// Base: 0xE000E000, Offset: 0x100
#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))

#endif // TM4C123GH6PM_MINIMAL_H