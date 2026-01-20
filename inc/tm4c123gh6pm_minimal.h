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


// Define the Base Address for Port A
#define GPIO_PORTA_BASE 0x40004000
// Cast that address to our Struct Pointer
#define GPIOA ((GPIO_Type *)GPIO_PORTA_BASE )


#define GPIO_PORTB_BASE 0x40005000
#define GPIOB ((GPIO_Type *)GPIO_PORTB_BASE)

// 1. Define the Base Address for Port F
#define GPIO_PORTF_BASE  0x40025000
// 2. Cast that address to our Struct Pointer
#define GPIOF  ((GPIO_Type *)GPIO_PORTF_BASE)

// 1. Define the Base Address for Port E
#define GPIO_PORTE_BASE 0x40024000
// 2. Cast that address to our Struct Pointer
#define GPIOE   ((GPIO_Type *)GPIO_PORTE_BASE)

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
// ADC Peripheral Definition
// ============================================================================

typedef struct {
    volatile uint32_t ACTSS;            // (Active Sample Sequencer) == 0x000
    volatile uint32_t RIS;              //(Raw Interrupt Status) == 0x004
    volatile uint32_t IM; 
    volatile uint32_t ISC;              //(Interrupt Status and Clear) == 0x00C
    volatile uint32_t OSTAT;            // 0x010: Overflow Status
    volatile uint32_t EMUX;             // 0x014: Event Multiplexer Select
    volatile uint32_t USTAT;            // 0x018: Underflow Status
    volatile uint32_t TSSEL;            // 0x01C: Trigger Source Select
    volatile uint32_t SSPRI;            // 0x020: Sample Sequencer Priority
    volatile uint32_t SPC;              // 0x024: Sample Phase Control
    volatile uint32_t PSSI;             //(Processor Sample Sequence Initiate - The "Go" Button) == 0x028

    volatile uint32_t RESERVED_2[29];   // 0x0A0 - 0x02C = 0X074(116 bytes) ==> 116 / 4 = 29 integers Gap from 0x028 to 0x0A0

    volatile uint32_t SSMUX3;           //(Sample Sequence Input Multiplexer 3) == 0x0A0
    volatile uint32_t SSCTL3;           //(Sample Sequence Control 3) == 0x0A4
    volatile uint32_t SSFIFO3;          //(Sample Sequence Result FIFO 3 - Where the data lives) == 0x0A8
}ADC_Type;

#define ADC0_BASE 0x40038000
#define ADC0 ((ADC_Type *)ADC0_BASE)


// ============================================================================
// UART Structure
// ============================================================================
typedef struct {
    volatile uint32_t DR;               // 0x000 UARTDR  UART D
    volatile uint32_t RSR;              // 0x004 UARTRSR/UARTECR  UART Receive Status/Error Clear 
    volatile uint32_t RESERVED[4];      // Gap 0x018 - 0x008 = 0x10 (16 (bytes) / 4(per integer) = 4)
    volatile uint32_t FR;               // 0x018 UARTFR  UART Flag
    volatile uint32_t RESERVED2[1];      // Gap 0x20 - 0x1C = 0x4  (4 / 4 = 1)
    volatile uint32_t ILPR;             // 0x020 UARTILPR   UART IrDA Low-Power Register 
    volatile uint32_t IBRD;             // 0x024 UARTIBRD   UART Integer Baud-Rate Divisor 
    volatile uint32_t FBRD;             // 0x028 UARTFBRD   UART Fractional Baud-Rate Divisor 
    volatile uint32_t LCRH;             // 0x02C UARTLCRH   UART Line Control 
    volatile uint32_t CTL;              // 0x030 UARTCTL   UART Control 
    volatile uint32_t IFLS;             // 0x034 UARTIFLS   UART Interrupt FIFO Level Select 
    volatile uint32_t IM;               // 0x038 UARTIM RW  UART Interrupt Mask 
    volatile uint32_t RIS;              // 0x03C UARTRIS RO  UART Raw Interrupt Status 
    volatile uint32_t MIS;              // 0x040 UARTMIS RO  UART Masked Interrupt Status 
    volatile uint32_t ICR;              // 0x044 UARTICR W1C  UART Interrupt Clear 
    volatile uint32_t DMACTL;           // 0x048 UARTDMACTL  UART DMA Control 
    volatile uint32_t RESERVED3[23];     // Gap 0x0A4 - 0x048 = 0x05C (92 / 4 = 23)
    volatile uint32_t BITADDR;          // 0x0A4 UART9BITADDR  UART 9-Bit Self Address 
    volatile uint32_t BITAMASK;         // 0x0A8 UART9BITAMASK  UART 9-Bit Self Address Mask 
}UART_Type;


#define UART0_BASE (0x4000C000)
#define UART0 ((UART_Type *)UART0_BASE)




// ============================================================================
// CAN Structure
// ============================================================================
typedef struct {
    volatile uint32_t CTL;          // 0x000 CAN Control
    volatile uint32_t STS;          // 0x004 CAN Status
    volatile uint32_t ERR;          // 0x008 CAN Error Counter
    volatile uint32_t BIT;          // 0x00C CAN Bit Timing
    volatile uint32_t INT;          // 0x0010 CAN Interrupt
    volatile uint32_t TST;          // 0x0014 CAN Test
    volatile uint32_t BRPE;         // 0x0018 CAN Baud Rate Prescaler Extension

    volatile uint32_t RESERVED[1];  // GAP: 0x20 - 0x1C = 4 bytes 

    volatile uint32_t IF1CRQ;       // 0x020 CAN IF1 Command Request
    volatile uint32_t IF1CMSK;      // 0x024 CAN IF1 Command Mask
    volatile uint32_t IF1MSK1;      // 0x028 CAN IF1 Mask 1
    volatile uint32_t IF1MSK2;      // 0x02C CAN IF1 Mask 2
    volatile uint32_t IF1ARB1;      // 0x030 CAN IF1 Arbitration 1
    volatile uint32_t IF1ARB2;      // 0x034 CAN IF1 Arbitration 2
    volatile uint32_t IF1MCTL;      // 0x038 CAN IF1 Message Control
    volatile uint32_t IF1DA1;       // 0x03C CAN IF1 Data A1
    volatile uint32_t IF1DA2;       // 0x040 CAN IF1 Data A2
    volatile uint32_t IF1DB1;       // 0x044 CAN IF1 Data B1
    volatile uint32_t IF1DB2;       // 0x048 CAN IF1 Data B2

    volatile uint32_t RESERVED_1[13]; // GAP: 0x80 - 0x4C = 52 bytes / 4 per int ==> 13 

    volatile uint32_t IF2CRQ;    // 0x080 CAN IF2 Command Request 
    volatile uint32_t IF2CMSK;   // 0x084 CAN IF2 Command Mask 
    volatile uint32_t IF2MSK1;   // 0x088 CAN IF2 Mask 1 
    volatile uint32_t IF2MSK2;   // 0x08C CAN IF2 Mask 2 
    volatile uint32_t IF2ARB1;   // 0x090 CAN IF2 Arbitration 1 
    volatile uint32_t IF2ARB2;   // 0x094 CAN IF2 Arbitration 2 
    volatile uint32_t IF2MCTL;   // 0x098 CAN IF2 Message Control 
    volatile uint32_t IF2DA1;    // 0x09C CAN IF2 Data A1 
    volatile uint32_t IF2DA2;    // 0x0A0 CAN IF2 Data A2 
    volatile uint32_t IF2DB1;    // 0x0A4 CAN IF2 Data B1 
    volatile uint32_t IF2DB2;    // 0x0A8 CAN IF2 Data B2 

    volatile uint32_t RESERVED_2[21]; // GAP: 0x100 - 0xAC = 84 bytes / 4 per int ==> 21

    volatile uint32_t TXRQ1;    // 0x100 CAN Transmission Request 1 
    volatile uint32_t TXRQ2;    // 0x104 CAN Transmission Request 2 

    volatile uint32_t RESERVED_3[6]; // GAP: 0x120 - 0x108 = 24 bytes / 4 per int ==> 6
           
    volatile uint32_t NWDA1;     // 0x120 CAN New Data 1 
    volatile uint32_t NWDA2;     // 0x124 CAN New Data 2 

    volatile uint32_t RESERVED_4[6]; // GAP: 0x140 - 0x128 = 24 bytes / 4 per int ==> 6

               
    volatile uint32_t MSG1INT;    // 0x140 CAN Message 1 Interrupt Pending 
    volatile uint32_t MSG2INT;    // 0x144 CAN Message 2 Interrupt Pending 

    volatile uint32_t RESERVED_5[6]; // GAP: 0x160 - 0x148 = 24 bytes / 4 per int ==> 6

    volatile uint32_t MSG1VAL;    // 0x160 CAN Message 1 Valid 
    volatile uint32_t MSG2VAL;    // 0x164 CAN Message 2 Valid

}CAN_Type;

#define CAN0_BASE 0x40040000
#define CAN0 ((CAN_Type *)CAN0_BASE)

// ============================================================================
// System Control (Clock) Definition
// ============================================================================

// For the clock, we'll just define the specific register pointer for now
// Base: 0x400FE000, Offset: 0x608
#define SYSCTL_RCGCGPIO  (*((volatile uint32_t *)0x400FE608))

// System Control for Timers (Base 0x400FE000 + Offset 0x604)
#define SYSCTL_RCGCTIMER (*((volatile uint32_t *)0x400FE604))

// System Control for ADC 
// Base 0x400F.E000, Offset 0x638 
#define SYSCTL_RCGCADC (*((volatile uint32_t *)0x400FE638))


// System Control for UART 
// Base 0x400F.E000, Offset 0x618
#define SYSCTL_RCGCUART (*(volatile uint32_t *)0x400FE618)


// System Control for CAN 
// Base 0x400F.E000, Offset 0x634
#define SYSCTL_RCGCCAN (*(volatile uint32_t *)0x400FE634)


// NVIC Enable Register 0 (Controls IRQ 0 to 31)
// Base: 0xE000E000, Offset: 0x100
#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))




#endif // TM4C123GH6PM_MINIMAL_H