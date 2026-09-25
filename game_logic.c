#include "game_logic.h"
#include "i2c_helper.h"
#include "ssd1306.h"
#include "inc/tm4c123gh6pm.h"
#include "utils/ustdlib.h"

#define GAME_SEED 213132 // You can enter your student code

#define DELAY 500

#define PF0 0x01
#define PF3 0x08
#define PF4 0x10

/*
    \brief Debounce switches with a short delay.
    \param None
    \return None
*/
void Debounce(void) {
    for (volatile uint32_t i = DELAY; i > 0; i--) {}
}

/*
    \brief Gives a random usigned number in the range.
    \param max Maximum number that can be returned (included)
    \param min Minimum number that can be returned (included)
    \return Random number in the range including both lower and upper limits
*/
uint8_t RandInRange(uint8_t max, uint8_t min) {
    int rand = urand(); 
    rand *= rand < 0 ? -1 : 1;
    return rand % (max + 1 - min) + min;
}

/*
    \brief SysTick interrupt handler that makes the buzzer pin PF3 output sound.
    \param None
    \return None
    \note To disable handler set RELOAD to 0.
*/
void SysTickHandler(void) {

}

void GameLoop(void) {
    usrand(GAME_SEED); // Sets the random seed
    // ...
    while(true) {
        DisplayClear();
        // ...
        DisplayUpdate();
    }
}
