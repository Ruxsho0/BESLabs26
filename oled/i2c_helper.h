#ifndef I2C_HELPER_H
#define I2C_HELPER_H

#include <stdint.h>

void I2C3Config(void);

/*
    \brief Sends a single byte through I2C.

    \param slave_addr Address of the OLED 128x64
    \param command SSD1306 command
    \param data Data to be sent
    \return None
*/
void SendSingleByte(uint8_t slave_addr, uint8_t command, uint8_t data);

/*
    \brief Sends multiple bytes through I2C.

    \param slave_addr Address of the OLED 128x64
    \param command SSD1306 command
    \param data Data array to be sent
    \param n Size of the data array
    \return None
*/
void SendMultipleBytes(uint8_t slave_addr, uint8_t command, uint8_t* data, uint32_t n);

#endif /* I2C_HELPER */