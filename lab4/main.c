/*
    Lab4 - Loop sequence, subroutines, blocking functions

    Navigate to the sequence.c to finish subroutines. Finish the main function.
    Find the DELAY_1MS value.
*/

#include "verify.h"
#include "sequence.h"

void PortFInit(void);
void Delay1ms(uint32_t msec);

int main(void) {
    PortFInit();        // Initialize Port F
    BESGrader();
    uint32_t out;       // output for PF3 and PF1 (for the debugger)
    while (true) {
    // 1) Green ON
    GPIO_PORTF_DATA_R = SetGreen(GPIO_PORTF_DATA_R);

    // 2) Wait for press
    WaitForSWLow();

    // 3) Green OFF
    GPIO_PORTF_DATA_R = ClearGreen(GPIO_PORTF_DATA_R);

    // 4) Red ON
    GPIO_PORTF_DATA_R = SetRed(GPIO_PORTF_DATA_R);

    // 5) Debounce
    Delay1ms(20);

    // 6) Wait for release
    WaitForSWHigh();

    // 7) Delay 500ms
    Delay1ms(500);

    // 8) Green ON (yellow, red juba põleb)
    GPIO_PORTF_DATA_R = SetGreen(GPIO_PORTF_DATA_R);

    // 9) Delay 500ms
    Delay1ms(500);

    // 10) Red OFF
    GPIO_PORTF_DATA_R = ClearRed(GPIO_PORTF_DATA_R);

    // Repeat

    }
}

/* 
    \brief Subroutine to initialize port F pins for input and output.
    PF4 is SW1 input.
    PF3 and PF1 is output LEDs.

    \param None
    \return None
*/
void PortFInit(void){ 
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Turn on the clock for Port F
    (void)SYSCTL_RCGC2_R;                 // Delay to allow clock to stabilize     
    GPIO_PORTF_AMSEL_R &= 0x00;             // Disable analog function
    GPIO_PORTF_PCTL_R &= 0x00000000;        // GPIO clear bit PCTL  
    GPIO_PORTF_DIR_R &= ~0x10;              // PF4 input,
    GPIO_PORTF_DIR_R |= 0x0A;               // PF3, PF1 output  
    GPIO_PORTF_AFSEL_R &= 0x00;             // No alternate function
    GPIO_PORTF_PUR_R |= 0x10;               // Enable pullup resistor on PF4       
    GPIO_PORTF_DEN_R |= 0x1A;               // Enable digital pins PF4, PF3, PF1
}

#define DELAY_1MS 1586 // Enter value that is close to ~0.9-1ms

/*
    \brief Subroutine to delay in units of milliseconds
    \param msec Number of milliseconds to delay
    \return None
    \note Uses internal 16 MHz oscillator
*/
void Delay1ms(uint32_t msec) {
    for (; msec > 0; msec--) {
        for (volatile uint32_t i = DELAY_1MS; i > 0; i--) {}
    }
}
