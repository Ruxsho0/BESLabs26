#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"

#include "i2c_helper.h"
#include "ssd1306.h"
#include "game_logic.h"

/*
    \brief Initialisation of the PF0, PF3 and PF4 of the Port F.
    \param None
    \return None
    \note Remember to unlock PortF to make changes.
*/
void PortFInit(void) {

}

/* 
    \brief Subroutine initializes SysTick periodic interrupts.
    \param None
    \return None
    \note RELOAD should be 0, priority is 0, enable with core clock and interrupts.
*/
void SysTickInit(void) {
    
}

int main(void) {
    PortFInit();   // SW1 and SW2 switch, PF3 Green LED + connected Buzzer
    SysTickInit();
    I2C3Config();
    DisplayInit();
    GameLoop();
}