#include "sequence.h"

void WaitForSWLow(void) {
    while ((GPIO_PORTF_DATA_R & 0x10) != 0) {}   // oota, kuni PF4 läheb madalaks (vajutatud)
}

void WaitForSWHigh(void) {
    while ((GPIO_PORTF_DATA_R & 0x10) == 0) {}   // oota, kuni PF4 läheb kõrgeks (vabastatud)
}

uint32_t SetRed(uint32_t portf_data_reg) {
    return portf_data_reg | 0x02;      // PF1 = 1
}

uint32_t ClearRed(uint32_t portf_data_reg) {
    return portf_data_reg & ~0x02;     // PF1 = 0
}

uint32_t SetGreen(uint32_t portf_data_reg) {
    return portf_data_reg | 0x08;      // PF3 = 1
}

uint32_t ClearGreen(uint32_t portf_data_reg) {
    return portf_data_reg & ~0x08;     // PF3 = 0
}