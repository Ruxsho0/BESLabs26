#include "dac.h"

void DACInit(void) {
    volatile uint32_t delay;
    // Complete functionality!
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   // lülita sisse Port B kell
    delay = SYSCTL_RCGC2_R;                  // oota kella stabiliseerumist
    GPIO_PORTB_DIR_R  |= 0x0F;               // PB0-3 väljund
    GPIO_PORTB_DR8R_R |= 0x0F;               // 8mA ajamitugevus PB0-3-l
    GPIO_PORTB_DEN_R  |= 0x0F;               // luba digitaalne I/O PB0-3-l
    GPIO_PORTB_DATA_R &= ~0x0F;              // algväärtus 0
}

void DACOut(uint32_t data) {
    // Complete functionality!
	GPIO_PORTB_DATA_R = data & 0x0F;   // väljasta ainult 4 madalamat bitti (0-15)
}
