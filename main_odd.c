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

void CAN_Configure_Receiver(void) {
  // 1. Setup Command Mask (We are writing everything)
  // WRNRD (Write), MASK, ARB, CONTRIL, DATAA, DATAB
  // We need 'MASK' bit (Bit 6) because we are setting up filtering.
  CAN0->IF2CMSK = 0xF3; // 1111 0011 (WENRD, MASK, ARB, CONTRIL, DATAA, DATAB)

  // 2. Setup Mask (Filter)
  // We want to accept ONLY ID 0x123
  // ID goes in bits 12:2
  // MDIR (Bit 14) = 0 (Don't filter on direction)
  // MXTD (Bit 15) = 0 (Don't filter on extende ID)
  CAN0->IF2MSK2 = (0x123 << 2);
  CAN0->IF2MSK1 = 0;


  // 3. Setup Arbitration (The ID we want)
  // ID = 0x123 (Bits 12:2)
  // MSGVAL (Bit 15) = 1 (Valid)
  // DIR (Bit 13) = 0 (Receive)
  CAN0->IF2ARB2 = (0x123 << 2) | (1U << 15);
  CAN0->IF2ARB1 = 0;

  // 4. Setup Message Control
  // UMASK(Bit 12) = 1 (Use the mask we sent above)
  // EOB (Bit 7) = 1 (End of Buffer)
  // DLC (Bits 3:0) = 8 (Expect 8 Bytes)
  CAN0->IF2MCTL = (1U << 12) | (1U << 7) | 8;

  // 5. Initiate Transfer to Message Object #2
  // This moves all the values from IF2 registers
  // into the actual Mailbox #2 RAM.
  CAN0->IF2CRQ = 2;

  // Wait for busy bit to clear (optional but good practice)
  while (CAN0->IF2CRQ & (1U << 15));
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

  // Setup LED (PF2)
  SYSCTL_RCGCGPIO |= (1U << 5); // Port F
  delay = SYSCTL_RCGCGPIO;

  GPIOF->DIR |= (1U << 2);
  GPIOF->DEN |= (1U << 2);

  // Configure Receiver to catch the message
  CAN_Configure_Receiver();

  // Send Message
  CAN_Transmit(0x123, 0xAA);
 
  // Wait loop
  while(1) {

    // A. Check if the Message Object 2 has new data
    // We query the "New Data" register (NWDA1) which
    // has one bit per mailbox.
    // Bit 1 corresponds to Mailbox 2
     
    if(CAN0->NWDA1 & (1U << 1)) {

      // B. FETCH the data from Mailbox 2 into Interface 2
      // Mask: Read Data (DATAA/DATAB), Read Control (CONTROL), Clear NewDat (CLRINTPND)
      // 0x73 = WRNRD(0) | MASK(0) | ARB(0) | CTRL(1) | CLRINTPND(0) | NEWDAT(1) | DATAA(1) | DATAB(1)
      CAN0->IF2CMSK = 0x17;
      CAN0->IF2CRQ = 2; //Fetch Mailbox 2
     
      // Wait for busy bit
      while (CAN0->IF2CRQ & (1U << 15));

      // C. Read the data from the Interface Register
      uint32_t received_data = CAN0->IF2DA1;

      // D. Verify and Light Blue LED
      // If Bit 3 (RXOK) is 1, turn on LED
      if( (received_data & 0xFF) == 0xAA) {
      GPIOF->DATA |= (1U << 2); // Blue LED on

      }

    }

  }

}