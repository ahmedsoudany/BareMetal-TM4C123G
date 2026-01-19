#include "tm4c123gh6pm_minimal.h"



void CAN_Transmit(uint32_t id, uint8_t data) {
    // 1. Wait for IF1 to be free (Busy bit is Bit 15)
    while (CAN0->IF1CRQ & (1U << 15));


    // 2. Setup Command Mask (We are writing everything)
    // WRNRD (Write), ARB (Arbitration), Control, DATAA, DATAB
    CAN0->IF1CMSK = 0xB3;

    // 3. Setup Arbitration (The ID)
    // ID goes in bits 12:2 of ARB2 for standard 11-bit ID
    // MsgVal (Bit 15) must be 1
    // Dir (Bit 13) = 1 (Transmit)
    CAN0->IF1ARB2 = (id << 2) | (1U << 13) | (1U << 15);
    CAN0->IF1ARB1 = 0;


    // 4. Setup Message Control
    // DLC = 1 (Sending 1 byte for test)
    // EOB (End of Buffer) = 1
    // TXRQ (Transmit Request) = 1 -> Start sending immediately
    CAN0->IF1MCTL = 1 | (1U << 7) | (1U << 8);

    // 5. Load Data (Just 1 byte for now)
    CAN0->IF1DA1 = data;

    // 6. Initiate Transfer to Message Object #1
    CAN0->IF1CRQ = 1;
}

int main() {
  // --- Initialization --- 

  // 1. Enable Run Mode Clock Gating Control for CAN0 and PORTB
  SYSCTL_RCGCCAN |= (1U << 0);
  volatile uint32_t delay = SYSCTL_RCGCCAN;
  (void)delay;

  SYSCTL_RCGCGPIO |= (1U << 1);
  delay = SYSCTL_RCGCGPIO;
  
  // --- 2. GPIO_Configuration --- 
  // A. Enable Alternate Function pins -> PB4, PB5
  GPIOB->AFSEL |= ((1U << 4) | (1U << 5));

  // B. SET PB4 and PB5 to CAN0Tx and CAN0Rx
  // by writing 8 to GPIOPCTL
  GPIOB->PCTL &= ~0x00FF0000;
  GPIOB->PCTL |=  0x00880000;

  // Digital Enable
  GPIOB->DEN |= ((1U << 4) | (1U << 5));

  // --- CAN0_Configuration
  // 3. Enter Init Mod
  CAN0->CTL |= (1U << 0);

  // 4. Enable test mode
  CAN0->CTL |= (1U << 7); 
  CAN0->TST |= (1U << 4); // Enable the loopback
  
  // 5. Configure Bit Timing
  // (TSEG2=2, TSEG1=3, BRP=0)
  CAN0->BIT = 0x00000203;

  // 6. Exit init mode
  CAN0->CTL &= ~(1U << 0);

  // Setup LED (PF1)
  SYSCTL_RCGCGPIO |= (1U << 5); // Port F
  delay = SYSCTL_RCGCGPIO;

  GPIOF->DIR |= (1U << 1);
  GPIOF->DEN |= (1U << 1);

  // Send Message
  CAN_Transmit(0x123, 0xAA);

  // Wait a tiny bit for the loopback to happen
  for(int i = 0 ; i < 1000; i++);

  // Check status
  // If Bit 3 (TXOK) is 1, turn on RED LED
  if (CAN0->STS & (1U << 3)) {
    GPIOF->DATA |= (1U << 1); // Success!
  }

  // Clear the status by reading the register (or writing to it, depending on silicon revision)
    // Usually reading STS clears the status bits.

  while(1) {}

}