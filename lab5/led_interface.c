#include "led_interface.h"

uint32_t SetOrToggleLED(uint32_t sw, uint32_t led_val) {
    // Complete this function!
    uint32_t out;
    if (sw == 0) {      // 0 == pole vajutatud
        out = 0x02;     // PE1 = LED ON
    } else {            // sw != 0 -> vajutatud
        out = led_val ^ 0x02;   // toggle PE1
    }
    return out;
}
