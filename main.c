/*
    Lab8 - FSM

    Complete port initializations and main function.
    Complete the enum states in the fsm.h.
    Fill in the 'fsm' array variable.
    Choose the correct argument for the SysTickWait.
*/

#include "verify.h"
#include "fsm.h"

#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))

#define GPIO_PORTF_DATA_R       (*((volatile uint32_t *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile uint32_t *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile uint32_t *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile uint32_t *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile uint32_t *)0x4002552C))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // Port F Clock Gating Control
	
#define GPIO_PORTB_DATA_R       (*((volatile uint32_t *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile uint32_t *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile uint32_t *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile uint32_t *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile uint32_t *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile uint32_t *)0x4000552C))
#define SYSCTL_RCGC2_GPIOB      0x00000002  // Port B Clock Gating Control
	
#define GPIO_PORTE_DATA_R       (*((volatile uint32_t *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile uint32_t *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // Port E Clock Gating Control
	
#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count Flag

void SysTickWait100ms(uint32_t times);
void SysTickInit(void);
void PortBInit(void);
void PortEInit(void);
void PortFInit(void);

const FSMStateData_t fsm[Count] = {
	// Complete this!
    {0x04, 0x02, 20, {GoCar,     WtCar,     WtCar,     WtCar}},      // GoCar
    {0x02, 0x02, 30, {StpCar,    StpCar,    StpCar,    StpCar}},     // WtCar
    {0x01, 0x02, 10, {GoPed,     GoPed,     GoPed,     GoPed}},      // StpCar
    {0x01, 0x08, 50, {BlinkOff1, BlinkOff1, BlinkOff1, BlinkOff1}},  // GoPed
    {0x01, 0x00,  5, {BlinkOn1,  BlinkOn1,  BlinkOn1,  BlinkOn1}},   // BlinkOff1
    {0x01, 0x08,  5, {BlinkOff2, BlinkOff2, BlinkOff2, BlinkOff2}},  // BlinkOn1
    {0x01, 0x00,  5, {BlinkOn2,  BlinkOn2,  BlinkOn2,  BlinkOn2}},   // BlinkOff2
    {0x01, 0x08,  5, {BlinkOff3, BlinkOff3, BlinkOff3, BlinkOff3}},  // BlinkOn2
    {0x01, 0x00,  5, {StpPed,    StpPed,    StpPed,    StpPed}},     // BlinkOff3
    {0x01, 0x02, 20, {GoCar,     GoCar,     GoCar,     GoCar}},      // StpPed
};

int main(void) {
	PortBInit();
	PortEInit();
	PortFInit();
    SysTickInit();
	BESGrader();
    uint32_t input;           // input combination from sensors on PE0-1
    FSMState_t state = GoCar; // current state index
    while (true) {
        // Complete this function!
        GPIO_PORTB_DATA_R = fsm[state].portb_out;   // hetkeoleku auto-tulede muster (Port B)
        GPIO_PORTF_DATA_R = fsm[state].portf_out;   // hetkeoleku jalakäija-tulede muster (Port F)
        SysTickWait100ms(fsm[state].times);         // oota selles olekus ettenähtud aeg (×100ms)
        input = GPIO_PORTE_DATA_R & 0x03;           // loe sensorid: PE0=auto, PE1=jalakäija
        state = fsm[state].next[input];             // mine järgmisesse olekusse vastavalt loetud sisendile
    }
}

/* 
    \brief Subroutine to delay for a specifiad amount of time.

    \param delay  
    \return The delay parameter is in units of the 16 MHz core clock (62.5 ns).
*/
void SysTickWait(uint32_t delay) {
    NVIC_ST_RELOAD_R = delay - 1;                           // number of counts to wait
    NVIC_ST_CURRENT_R = 0;                                  // any value written to CURRENT clears it
    while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0) {}   // wait for count flag
}

/* 
    \brief Subroutine to delay exactly 100ms X times.

    \param times How many times to delay 100ms
    \return None
	\note Use this function for delays in the main program.
*/
void SysTickWait100ms(uint32_t times) {
    for (uint32_t i = 0; i < times; i++) {
        SysTickWait(1600000); // 16 MHz - 1s, ? - 100ms
    }
}

/*
    \brief Subroutine to initialize SysTick. 
    \param None
    \return None
    \note Runs at 16 MHz. 
*/
void SysTickInit(void) {
    NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
    NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}

/* 
    \brief Subroutine to initialize port B pins for output.
    PB0 is car red external LED.
    PB1 is car yellow external LED.
	PB2 is car green external LED.

    \param None
    \return None
*/
void PortBInit(void) {
    // Complete this function!
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   // lülitab sisse Port B kella signaali (clock gating)
    volatile uint32_t delay = SYSCTL_RCGC2_R; // loe register tagasi, annab kellale aega stabiliseeruda
    GPIO_PORTB_AMSEL_R &= ~0x07;            // lülita PB0-PB2 analoogfunktsioon välja (digitaalseks)
    GPIO_PORTB_PCTL_R  &= ~0x00000FFF;      // vali PB0-PB2 jaoks tavaline GPIO (mitte alternatiivfunktsioon)
    GPIO_PORTB_DIR_R   |= 0x07;             // PB0-PB2 väljundiks (auto LEDid)
    GPIO_PORTB_AFSEL_R &= ~0x07;            // PB0-PB2 alternatiivfunktsioon välja (tavaline GPIO)
    GPIO_PORTB_DEN_R   |= 0x07;             // luba PB0-PB2 digitaalsisend/väljund
}

/* 
    \brief Subroutine to initialize port E pins for input.
    PE0 is car sensor (external switch).
    PE1 is pedestrian sensor (external switch).

    \param None
    \return None
*/
void PortEInit(void) {
    // Complete this function!
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   // lülita sisse Port E kella signaal
    volatile uint32_t delay = SYSCTL_RCGC2_R; // loe register tagasi, annab kellale aega stabiliseeruda
    GPIO_PORTE_AMSEL_R &= ~0x03;            // lülita PE0-PE1 analoogfunktsioon välja
    GPIO_PORTE_PCTL_R  &= ~0x000000FF;      // vali PE0-PE1 jaoks tavaline GPIO
    GPIO_PORTE_DIR_R   &= ~0x03;            // PE0-PE1 sisendiks (sensorid)
    GPIO_PORTE_AFSEL_R &= ~0x03;            // PE0-PE1 alternatiivfunktsioon välja
    GPIO_PORTE_DEN_R   |= 0x03;             // luba PE0-PE1 digitaalsisend/väljund
}

/* 
    \brief Subroutine to initialize port F pins for input.
    PF1 is pedestrian internal red LED.
    PF3 is pedestrian internal green LED.

    \param None
    \return None
*/
void PortFInit(void) { 
    // Complete this function!
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // lülita sisse Port F kella signaal
    volatile uint32_t delay = SYSCTL_RCGC2_R; // loe register tagasi, annab kellale aega stabiliseeruda
    GPIO_PORTF_AMSEL_R &= ~0x0A;            // lülita PF1 ja PF3 analoogfunktsioon välja
    GPIO_PORTF_PCTL_R  &= ~0x0000F0F0;      // vali PF1 ja PF3 jaoks tavaline GPIO
    GPIO_PORTF_DIR_R   |= 0x0A;             // PF1 ja PF3 väljundiks (jalakäija LEDid)
    GPIO_PORTF_AFSEL_R &= ~0x0A;            // PF1 ja PF3 alternatiivfunktsioon välja
    GPIO_PORTF_DEN_R   |= 0x0A;             // luba PF1 ja PF3 digitaalsisend/väljund
}
