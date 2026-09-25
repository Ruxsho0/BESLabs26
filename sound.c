#include "sound.h"
#include "dac.h"

// RESERVED VARIABLE - DO NOT USE
extern uint32_t nvic_st_reload;

const uint32_t sinewave[] = {
    7, 8, 9, 10, 11, 12, 13, 14,
    15, 15, 15, 14, 13, 12, 11, 10,
    9, 8, 7, 6, 5, 4, 3, 2,
    1, 1, 1, 2, 3, 4, 5, 6
};
uint32_t i = 0;

void SoundInit(void) {
    // Complete functionality!
	// disable SysTick during setup
	// reload value is 0
	// any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x1FFFFFFF) | 0x20000000; // priority 1
	// enable SysTick with core clock and interrupts
}

void SoundPlay(uint32_t period) {
    // Complete functionality!
}

void SysTickHandler(void) {
    // Complete functionality!
}

uint32_t ChooseNote(uint32_t input) {
    // Complete functionality!
}