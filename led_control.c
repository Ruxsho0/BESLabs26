#include "led_control.h"

uint32_t SetOrToggleLED(uint32_t sw, uint32_t led_val) {
    uint32_t out = 0x04;
    if (sw == 0) {                  // negatiivne loogika: 0 = lüliti vajutatud
        out = led_val ^ 0x04;       // toggle PF2
    } else {                        // lüliti pole vajutatud
        out = 0x04;                 // PF2 = 1 (LED ON)
    }
    return out;
}

// Color    LED(s) PortF
// none     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06