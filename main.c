/*
    Lab10 - DAC

    Navigate to the dac.c/piano.c/sound.c to finish functions.
    Finish the main function.
*/

#include "verify.h"
#include "sound.h"
#include "piano.h"
#include "dac.h"

#define DELAY_20MS 32000

void Delay20ms(void);

int main(void) {
    SoundInit();
    PianoInit();
    DACInit();
    BESGrader();
    while (true) {
        // Complete functionality!
        uint32_t input = PianoInput();
        uint32_t period = ChooseNote(input);
        SoundPlay(period);
    }
}

/*
    \brief Subroutine to delay 20 milliseconds
    \param None
    \return None
    \note Assumes 16 MHz clock
*/
void Delay20ms(void) {
    for (volatile uint32_t i = DELAY_20MS; i > 0; i--) {}
}
