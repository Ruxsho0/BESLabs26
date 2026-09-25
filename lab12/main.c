/*
    Lab12 - Low power mode

    Complete Port F initialization and finish SysTick init and analog_temp.c functions.
*/

#include "verify.h"
#include "analog_temp.h"
#include "inc/tm4c123gh6pm.h"

void RunMode(void);
void SleepMode(void);
void PortFInit(void);
void SysTickInit(void);
void Delay2s(void);

int main(void) {
    PortFInit();
    TempSensorInit();
    BESGrader();
    RunMode(); // Comment out if using SleepMode()
    //SleepMode(); // Comment out if using RunMode()
    return 0;
}

/*
    \brief Makes the uC work all the time (CPU usage is 100%).
    \param None
    \note None
*/
void RunMode(void) {
    UARTprintf("Starting Run mode.\n");
    while (true) {
        Delay2s();
        GetTemp();
    }
}

/*
    \brief Exits the main function and executes the WFI assembly instruction to wake up from sleep
    mode only when the interrupt is recognised.
    \param None
    \note None
*/
void SleepMode(void) {
    SysTickInit();
    UARTprintf("Starting Sleep mode.\n");
    __asm("    wfi\n"); // Wait For Interrupt (enter Sleep Mode)
}

/*
    \brief Initialisation of the PF3 and PF1 ports of the Port F.
    \param None
    \return None
*/
void PortFInit(void) {
	// 1) Activate clock for Port F
    // Allow time for clock to start
    // 2) Disable analog on PF3 and PF1
    // 3) PCTL GPIO on PF3 and PF1
    // 4) PF3 PF1 out
    // 5) Disable alt funct on PF3 and PF1
    // 6) Disable pull-up on PF3 and PF1
    // 7) Enable digital I/O on PF3 PF1
}

/* 
    \brief Subroutine initializes SysTick periodic interrupts with
    precision internal oscillator (PIOSC) divided by 4.
    \param None
    \return None
    \note Initialize SysTick interrupts to trigger every 2 s.
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;             // Disable SysTick during setup
    NVIC_ST_RELOAD_R = ? - 1; // Reload value
    NVIC_ST_CURRENT_R = 0;          // Any write to current clears it
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x1FFFFFFF) | 0x40000000; // priority 2
	NVIC_SYS_CTRL_R |= 0x0?;        // Bit 1 set SLEEPEXIT
    NVIC_ST_CTRL_R = 0x03;          // Enable SysTick with PIOSC divided by 4 and interrupts, CLK_SRC bit 2 is 0
}

/*
    \brief Delays approximately 2s.
    \param None
    \return None
    \note Assumes 16 MHz clock
*/
void Delay2s(void) {
    for (volatile uint32_t i = 3200000; i > 0; i--) {}
}

/*
    \brief Interrupt handler which is called every 2s.
    \param None
    \return None
    \note Puts uC into sleep mode after execution.
*/
void SysTickHandler(void) {
    GetTemp();
}