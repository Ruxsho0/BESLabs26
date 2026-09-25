/*
    Lab1 - Simple I/O
    
    Navigate to the led_color.c file to finish the SetLEDColor() function.
*/

#include "led_color.h"
#include "verify.h"

#define DELAY_20MS 32000

void PortFInit(void);
void Delay20ms(void);

int main(void) {
    PortFInit();        // Call initialization of port PF4, PF3, PF2, PF1, PF0
    BESGrader();
    uint32_t sw1, sw2;  // Input from PF4, PF0
    uint32_t out;       // Output for PF1, PF2, PF3
    while (true) {
        sw1 = GPIO_PORTF_DATA_R & 0x10;      // Read PF4 into SW1
        sw2 = GPIO_PORTF_DATA_R & 0x01;      // Read PF0 into SW2
        Delay20ms();                         // Debounce switches
        out = SetLEDColor(sw1, sw2);
        GPIO_PORTF_DATA_R = out;
    }
}

/* 
    \brief Subroutine to initialize port F pins for input and output.
    
    PF4 and PF0 are input SW1 and SW2 respectively.
    PF3, PF2, PF1 are outputs to the LED.

    \param None
    \return None
    \note These five pins are connected to hardware on the LaunchPad
*/
void PortFInit(void) { 
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // Turn on the clock for Port F
    delay = SYSCTL_RCGC2_R;                 // Delay (dummy read)  
    GPIO_PORTF_LOCK_R = 0x4C4F434B;         // Unlock PF0  
    GPIO_PORTF_CR_R = 0x1F;                 // Allow changes to PF4-0       
    GPIO_PORTF_AMSEL_R = 0x00;              // Disable analog function
    GPIO_PORTF_PCTL_R = 0x00000000;         // GPIO clear bit PCTL  
    GPIO_PORTF_DIR_R = 0x0E;                // PF4, PF0 input, PF3, PF2, PF1 output   
    GPIO_PORTF_AFSEL_R = 0x00;              // No alternate function
    GPIO_PORTF_PUR_R = 0x11;                // Enable pullup resistors on PF4,PF0       
    GPIO_PORTF_DEN_R = 0x1F;                // Enable digital pins PF4-PF0        
}

/*
    \brief Subroutine to delay 20 milliseconds
    \param None
    \return None
    \note Assumes 16 MHz clock
*/
void Delay20ms(void) {
    for (volatile uint32_t i = DELAY_20MS; i > 0; i--) {}
}
