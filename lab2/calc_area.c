#include "calc_area.h"

uint32_t CalcArea(uint32_t l, uint32_t w) {
    // Complete this function!
    if (l < 3 || l > 20 || w < 3 || w > 20) {
        return 0;
    }
    return l * w;
}
