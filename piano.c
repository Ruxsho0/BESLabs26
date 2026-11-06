#include "piano.h"

void PianoInit(void) {
    volatile uint32_t delay;
    // Complete functionality!
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   // lülita sisse Port E kell
    delay = SYSCTL_RCGC2_R;                  // oota kella stabiliseerumist
    GPIO_PORTE_AMSEL_R &= ~0x0F;             // lülita analoogfunktsioon PE0-3-l välja
    GPIO_PORTE_PCTL_R  &= ~0x0000FFFF;       // vali PE0-3 jaoks tavaline GPIO
    GPIO_PORTE_AFSEL_R &= ~0x0F;             // lülita alternatiivfunktsioon PE0-3-l välja
    GPIO_PORTE_DIR_R   &= ~0x0F;             // PE0-3 sisend
    GPIO_PORTE_DEN_R   |= 0x0F;              // luba digitaalne I/O PE0-3-l
}

uint32_t PianoInput(void) {
    // Complete functionality!
	return GPIO_PORTE_DATA_R & 0x0F;   // loe PE0-3 lülitite olek
}
