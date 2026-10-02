#include "bits.h"

uint32_t ToggledBits(uint32_t data_reg, uint32_t before_toggle) {
    // Complete this function!
    return (data_reg ^ before_toggle) & 0x13; // PF0, PF1, PF4
}
