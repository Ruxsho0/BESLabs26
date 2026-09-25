#ifndef LED_COLOR_H
#define LED_COLOR_H

#include <stdint.h>

/*
    Returns a LED color combination value depending on the switch press combination.
    \param sw1 Switch value connected to PF4
    \param sw2 Switch value connected to PF0
    \return The value for PortF. Both SW1 and SW2 are pressed -> blue.\n 
    Just SW1 pressed -> red.
    Just SW2 pressed -> green.
    Neither SW1 or SW2 is pressed -> off.
    \note SW1 and SW2 is negative logic on the Launchpad.
    Red LED connected to PF1.
    Blue LED connected to PF2.
    Green LED connected to PF3.
*/
uint32_t SetLEDColor(uint32_t sw1, uint32_t sw2);

#endif /* LED_COLOR_H */
