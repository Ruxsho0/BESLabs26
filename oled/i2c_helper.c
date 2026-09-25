#include "i2c_helper.h"
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"
#include "utils/uartstdio.h"

#define I2C_INFO 0

// I2C
#define GPIO_PD0_I2C3SCL 0x00030003
#define GPIO_PD1_I2C3SDA 0x00030403

void I2C3Config(void) { // Configure I2C3 for pins PD0 and PD1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD0_I2C3SCL);
    GPIOPinConfigure(GPIO_PD1_I2C3SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
    I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), false);

    // Pull up I2C3 by PB6 and PB7 pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
}

void SendSingleByte(uint8_t slave_addr, uint8_t command, uint8_t data) {
    I2CMasterSlaveAddrSet(I2C3_BASE, slave_addr, false);
    I2CMasterDataPut(I2C3_BASE, command);
    I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(I2C3_BASE)) {}
    I2CMasterDataPut(I2C3_BASE, data);
    I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(I2C3_BASE)) {}
}

void SendMultipleBytes(uint8_t slave_addr, uint8_t command, uint8_t* data, uint32_t n) {
    I2CMasterSlaveAddrSet(I2C3_BASE, slave_addr, false);
    I2CMasterDataPut(I2C3_BASE, command);
    I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    if (I2C_INFO) {
        UARTprintf("[SEND_START] Master error code: %x\n", I2CMasterErr(I2C3_BASE));
    }
    while (I2CMasterBusy(I2C3_BASE)) {}
    for (uint32_t i = 0; i < n - 1; i++) {
        I2CMasterDataPut(I2C3_BASE, data[i]);
        I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        if (I2C_INFO) {
            UARTprintf("[SEND_CONT] Master error code: %x\n", I2CMasterErr(I2C3_BASE));
        }
        while (I2CMasterBusy(I2C3_BASE)) {}
    }
    I2CMasterDataPut(I2C3_BASE, data[n - 1]);
    I2CMasterControl(I2C3_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    if (I2C_INFO) {
        UARTprintf("[SEND_FINISH] Master error code: %x\n", I2CMasterErr(I2C3_BASE));
    }
    while (I2CMasterBusy(I2C3_BASE)) {}
}