#include "sound.h"
#include "dac.h"

const uint32_t sinewave[] = {
    7, 8, 9, 10, 11, 12, 13, 14,
    15, 15, 15, 14, 13, 12, 11, 10,
    9, 8, 7, 6, 5, 4, 3, 2,
    1, 1, 1, 2, 3, 4, 5, 6
};
uint32_t i = 0;

void SoundInit(void) {
    // Complete functionality!
    NVIC_ST_CTRL_R = 0;               // lülita SysTick seadistamise ajaks välja
    NVIC_ST_RELOAD_R = 0;             // reload väärtus 0 (vaikimisi heli väljas)
    NVIC_ST_CURRENT_R = 0;            // mistahes kirjutus CURRENT-sse nullib selle
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x1FFFFFFF) | 0x20000000; // prioriteet 1
    NVIC_ST_CTRL_R = 0x00000007;      // luba SysTick, core clock, katkestused sees
}

void SoundPlay(uint32_t period) {
    // Complete functionality!
    NVIC_ST_RELOAD_R = period - 1;    // period=1 -> RELOAD=0 -> heli väljas
}

void SysTickHandler(void) {
    // Complete functionality!
    DACOut(sinewave[i]);              // väljasta praegune siinuslaine punkt DAC-ile
    i = (i + 1) % 32;                 // liigu järgmise punkti juurde, tsükliliselt (32 punkti)
}

uint32_t ChooseNote(uint32_t input) {
    // Complete functionality!
    switch (input) {
        case 0x01: return 956;   // PE0 = C (523.251 Hz)
        case 0x02: return 851;   // PE1 = D (587.330 Hz)
        case 0x04: return 758;   // PE2 = E (659.255 Hz)
        case 0x08: return 638;   // PE3 = G (783.991 Hz)
        default:   return 1;     // ei midagi vajutatud VÕI mitu korraga -> heli väljas
    }
}

// C (523.251 Hz): 500000 / 523.251 = 955.66 → ümardad 956
// D (587.330 Hz): 500000 / 587.330 = 851.34 → ümardad 851
// E (659.255 Hz): 500000 / 659.255 = 758.43 → ümardad 758
// G (783.991 Hz): 500000 / 783.991 = 637.76 → ümardad 638
