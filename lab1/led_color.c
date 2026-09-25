#include "led_color.h"

// Color    LED(s) PortF
// none     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06

uint32_t SetLEDColor(uint32_t sw1, uint32_t sw2) {
    // Fix this function!
    if  (sw1 && sw2) {
        return 0; // dark
    } else if (sw1) {
        return 0x08; // green
    } else if (sw2) {
        return 0x02; // red
    } else {
        return 0x04;    // blue
    }
}
