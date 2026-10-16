/*
    Lab7 - Interrupts and buzzer

    Finish the SysTickHandler() function, PortA initialization and NVIC_ST_RELOAD value.
*/

#include "verify.h"

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))

#define GPIO_PORTA_AMSEL_R      (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile uint32_t *)0x4000452C))
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_DR8R_R       (*((volatile uint32_t *)0x40004508))
#define GPIO_PORTA_AFSEL_R      (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C))
#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC))
#define SYSCTL_RCGC2_GPIOA      0x00000001  // Port A Clock Gating Control
#define GPIO_PORTA_PDR_R        (*((volatile uint32_t *)0x40004514))

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_SYS_PRI3_R         (*((volatile uint32_t *)0xE000ED20))

void PortAInit(void);
void SysTickInit(void);
void SysTickHandler(void);

// Should remain untouched!
int main(void) {
    PortAInit();
    SysTickInit();
    BESGrader();
    while (true) {}
}

/* 
    \brief Subroutine to initialize port A pins for input and output.
    PA3 is external switch input.
    PA2 is output to the buzzer.

    \param None
    \return None
    \note No analog, regular function, no alt function, digital enabled, 8-mA drive select for output
*/
void PortAInit(void) {
    // Complete this function!
    volatile uint32_t delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    delay = SYSCTL_RCGC2_R;                     // oota kella käivitumist
    GPIO_PORTA_AMSEL_R &= ~0x0C;                // analoog välja PA3, PA2
    GPIO_PORTA_PCTL_R &= ~0x0000FF00;            // PCTL GPIO peale PA3, PA2
    GPIO_PORTA_DIR_R = (GPIO_PORTA_DIR_R & ~0x08) | 0x04; // PA3 sisend, PA2 väljund
    GPIO_PORTA_AFSEL_R &= ~0x0C;                 // alt-funktsioonid välja PA3, PA2
    GPIO_PORTA_DR8R_R |= 0x04;                   // 8mA draiv PA2-le
    GPIO_PORTA_PDR_R |= 0x08;
    GPIO_PORTA_DEN_R |= 0x0C;                    // digitaal sisse PA3, PA2
}

/*
    \brief Subroutine to initialize SysTick. 
    \param None
    \return None
    \note Runs at 16 MHz. 
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
    NVIC_ST_RELOAD_R = 18182 - 1;     // reload value for 1.13636ms (assuming 16MHz) (16MHZ / 880HZz = 18182)
    NVIC_ST_CURRENT_R = 0;        // any write to current clears it
    NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R & 0x1FFFFFFF; // priority 0               
    NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}

/*
    \brief Subroutine to handle the SysTick interrupt. When switch is pressed, the buzzer
    is emmiting a tone at 440 Hz. When the switch is pressed again, the buzzer turns off.
    \param None
    \return None
    \note Interrupt at 1/880Hz = 1.13636ms. 
    When the switch is held down, the buzzer tone is toggled on/off once!
*/
void SysTickHandler(void) {
    // Complete this function!
    static uint32_t lastSwitch = 0; // PA3 eelmine lugemine (positiivne loogika: 1 = vajutatud)
    static uint32_t active = 0;     // 1 = puhver peab piiksuma, 0 = vaikne
    uint32_t sw = (GPIO_PORTA_DATA_R & 0x08) ? 1 : 0; // loe PA3

    if (sw == 1 && lastSwitch == 0) {
        // tõusev serv -> lüliti just vajutati
        active ^= 1; // lülita heli sisse/välja
    }
    lastSwitch = sw;

    if (active) {
        GPIO_PORTA_DATA_R ^= 0x04; // toggle PA2 (tekitab 440Hz ruutlaine)
    } else {
        GPIO_PORTA_DATA_R &= ~0x04; // hoia PA2 madalal (vaikus)
    }

}
