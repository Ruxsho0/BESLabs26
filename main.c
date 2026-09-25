/*
    Lab3 - Port initialization, Delay, LED control

    Navigate to the led_control.c to finish the SetOrToggleLED() function.
    Finish the main function and the PortF initialization.
*/

#include "led_control.h"
#include "verify.h"

#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile uint32_t *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile uint32_t *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile uint32_t *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // Port F Clock Gating Control

void PortFInit(void);
void Delay100ms(uint32_t times);

int main(void){
    PortFInit(); // Student submitted subroutine
    BESGrader();
    uint32_t sw1;  // input from PF4
    uint32_t out = 0x04;  // output for PF2
    while (true) {
        // Complete this functionality!
        sw1 = GPIO_PORTF_DATA_R & 0x10;
        Delay100ms(1);
        out = SetOrToggleLED(sw1, out);
        GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x04) | out;
    }
}

/* 
    \brief Subroutine to initialize port F pins for input and output.
    PF4 is SW1 input.
    PF2 is output to the LED.

    \param None
    \return None
    \note Set the LED to be initially ON at the end of the initialization. Bit setting
    doesn't affect other bits.
*/
void PortFInit(void) {
    // Complete this function!
    SYSCTL_RCGC2_R |=0x20;// Turn on the clock for Port F
    (void)SYSCTL_RCGC2_R;// Allow time for clock to start
    GPIO_PORTF_AMSEL_R &= ~0x14;// Disable analog on PF4 and PF2 AMSEL
    GPIO_PORTF_PCTL_R &= ~0x000F0F00;// Clear PF4 and PF2 bit fields PCTL to configure as GPIO
    GPIO_PORTF_DIR_R = (GPIO_PORTF_DIR_R & ~0x10) | 0x04;// PF4 input, PF2 output
    GPIO_PORTF_AFSEL_R &= ~0x14;// Clear PF4 and PF2 bits AFSEL to disable alternate functions
    GPIO_PORTF_PUR_R |= 0x10;// Set PF4 PUR to activate an internal pullup resistor
    GPIO_PORTF_DEN_R |= 0x14;// Set PF4 and PF2 bits DEN to enable digital
    GPIO_PORTF_DATA_R |= 0x04;// Set PF2 DATA so LED is initially ON
}

#define DELAY_100MS 160000 // ~100ms

/*
    \brief Subroutine to delay 100 milliseconds N times
    \param times Number of times to delay 100 ms
    \return None
    \note Assumes 16 MHz clock
*/
void Delay100ms(uint32_t times) {
    for (; times > 0; times--) {
        for (volatile uint32_t i = DELAY_100MS; i > 0; i--) {}
    }
}
