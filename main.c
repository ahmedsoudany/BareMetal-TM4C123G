#include "tm4c123gh6pm_minimal.h"


uint32_t read_ADC(void) {

    uint16_t result;
    // 1. Initiate Sampling
    ADC0->PSSI |= (1 << 3);

    // 2. Wait for the flag (Busy Wait until bit 3 is 1)
    while( ((ADC0->RIS) & (1 << 3)) == 0);

    // 3. Read Data
    result = ADC0->SSFIFO3;

    // 4. Clear flag
    ADC0->ISC = (1U << 3);


    return result;

}

int main() {

    // --- ADC Setup (Port E + ADC0) ---

    //1. Enable clock for ADC
    SYSCTL_RCGCADC |= (1U << 0);

    // There must be a delay of 3 system clocks after the ADC module clock is enabled before
    // any ADC module registers are accessed
    volatile unsigned int delay = SYSCTL_RCGCADC;
    (void)delay;

    // 2. GPIO Port E Run Mode Clock Gating Control
    SYSCTL_RCGCGPIO |= (1U << 4);
    delay = SYSCTL_RCGCGPIO;
    (void)delay;

    // 3. Configure PE3 --> AIN0
    // 3.1 Set PE3(ANO) as Input (DIR = 0).
    GPIOE->DIR &= ~(1U << 3);

    // 3.2 Enable Alternate Function: Set the GPIO AFSEL bits for the ADC input pins
    GPIOE->AFSEL |= (1U << 3);

    // 3.3 Disable Digital: Configure the AINx signals to be analog inputs by clearing the corresponding DEN
    GPIOE->DEN &= ~(1U << 3);

    // 3.4 Enable Analog Mode: Disable the analog isolation circuit for all ADC input pins that are to be used
    GPIOE->AMSEL |= (1U << 3);

    // 4. Configuration of the sample sequencers (SS3)
    // 4.1 Ensure that the sample sequencer is disabled by clearing the corresponding ASENn bit (Sample Sequencer 0 is disabled)
    ADC0->ACTSS &= ~(1U << 3); 

    // 4.2 Configure Trigger Source 
    // Clear bits 15:12 to select "Processor (Software) Trigger" for SS3
    ADC0->EMUX &= ~(0xF000);

    // 3.2 configure the corresponding input Select Channel 0 (AIN0)
    ADC0->SSMUX3 = 0x0;

    // 3.3 configure the sample control bits (IE0 | END0)
    // Bit 1 (END0) = 1 (End of sequence)
    // Bit 2 (IE0)  = 1 (Interrupt Enable - sets the RIS flag)
    ADC0->SSCTL3 = 0x6;

    // 3.4 Enable the sample sequencer SS3
    ADC0->ACTSS |= (1U << 3); 


    // --- 4. PWM Setup (Port F + Timer0) ---
    // 4.1 Enable clock for GPIO
    SYSCTL_RCGCGPIO |= (1U << 5); // Enable Port F
    delay = SYSCTL_RCGCGPIO;
    
     // 4.2 Enable Clock for Timer 0
    SYSCTL_RCGCTIMER |= (1U << 0); 
    delay = SYSCTL_RCGCTIMER; // Wait for clock to stabilize

    // 4.3 Disable PF1 (RED LED)
    GPIOF->DIR &= ~(1U << 1);      

    // 4.4 Enable Alternate Function
    GPIOF->AFSEL |= (1U << 1); 

    // 4.5 Select Function 7
    // Clear the Register
    GPIOF->PCTL &= ~0x000000F0;
    GPIOF->PCTL |= (0x00000070);

    GPIOF->DEN |= (1U << 1);

    // Configure Timer 0B
    // 4.6. Disable Timer0B during setup (Clear Bit 8)
    TIMER0->CTL &= ~(1U << 8);

    // 4.7. Configure for 16-bit mode (Write 0x04)
    TIMER0->CFG = 0x4;

    // 4.8 Configure for PWM Mode (Write 0x2 to TAMR)
    // Bit 1:0 = 0x2 (Periodic)
    // Bit 4 = 1 (Count Up? No, keep 0 for Count Down)
    TIMER0->TBMR =0xA;

    // We want 1kHz(1000 cycle per second)
    // 5. Set the Load Value (16,000,000 / 1000) = 16,000
    // write 1600 - 1 
    TIMER0->TBILR = 16000 - 1;

    // Start OFF (Match = Load)
    TIMER0->TBMATCHR = 16000 - 1;

    // 8. Enable Timer0B 
    TIMER0->CTL &= ~(1U << 14); // Ensure Output is NOT inverted
    TIMER0->CTL |= (1U << 8);   // Enable


    // --- 5. The loop ---
    while(1) {

    // 1. Read the Potentiometer
    uint32_t adc_value = read_ADC(); // 0 to 4096

    // 2. Calculate PWM Match
    // Cap to 4000 to prevent underflow (4000 * 4 = 16000)
    if(adc_value > 4000) adc_value = 4000;
        
    // 3. Update Match register
    // Invert logic: High ADC -> Low Match ->High Brightness
    uint32_t match_value = 16000 - (adc_value * 4);

    // 4. Update Hardware
    TIMER0->TBMATCHR = match_value;

    // 5. Delay for stability 
    for(int i=0; i<50000; i++);

    }

}