#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>

#define SSD1306_LCDWIDTH                128
#define SSD1306_LCDHEIGHT               64
#define SSD1306_DEACTIVATE_SCROLL       0x2E
#define SSD1306_SETCONTRAST             0x81
#define SSD1306_DISPLAYALLON_RESUME     0xA4
#define SSD1306_DISPLAYALLON            0xA5
#define SSD1306_NORMALDISPLAY           0xA6
#define SSD1306_INVERTDISPLAY           0xA7
#define SSD1306_DISPLAYOFF              0xAE
#define SSD1306_DISPLAYON               0xAF
#define SSD1306_SETDISPLAYOFFSET        0xD3
#define SSD1306_SETCOMPINS              0xDA
#define SSD1306_SETVCOMDETECT           0xDB
#define SSD1306_SETDISPLAYCLOCKDIV      0xD5
#define SSD1306_SETPRECHARGE            0xD9
#define SSD1306_SETMULTIPLEX            0xA8
#define SSD1306_SETLOWCOLUMN            0x00
#define SSD1306_SETHIGHCOLUMN           0x10
#define SSD1306_SETSTARTLINE            0x40
#define SSD1306_MEMORYMODE              0x20
#define SSD1306_COLUMNADDR              0x21
#define SSD1306_PAGEADDR                0x22
#define SSD1306_COMSCANINC              0xC0
#define SSD1306_COMSCANDEC              0xC8
#define SSD1306_SEGREMAP                0xA0
#define SSD1306_CHARGEPUMP              0x8D
#define SSD1306_EXTERNALVCC             0x01
#define SSD1306_SWITCHCAPVCC            0x02

#define PAGE_SIZE 8 // how many pixels in a single page in 1 column
#define PAGES (SSD1306_LCDHEIGHT / 8) // how many pages in the screen

/* 
    \brief Initialize the screen using 128x64 OLED sequence.

    \param None
    \return None
    \note Sets the cursor at the beginning (0, 0).
*/
void DisplayInit(void);

/* 
    \brief Turns the display on.

    \param None
    \return None
*/
void DisplayOn(void);

/* 
    \brief Turns the display off.

    \param None
    \return None
*/
void DisplayOff(void);

/* 
    \brief Clears the display cache.

    \param None
    \return None
    \note You must call DisplayUpdate() after this to see any change.
*/
void DisplayClear(void);

/* 
    \brief Applies the cache onto the display, updating any changes.

    \param None
    \return None
*/
void DisplayUpdate(void);


/* 
    \brief Sets the cursor on the screen.

    \param col Column of the screen (from 0 to 127 inclusively)
    \param page Page of the screen (from 0 to 7 inclusively)
    \return None
    \note The cursors sets the index in the cache.
*/
void SetCursor(uint8_t col, uint8_t page);


/* 
    \brief Draws a string on the screen.

    \param str String consisting of ASCII characters
    \return None
    \note Draws at the current cursor position. Wraps around if cannot fit.
*/
void DrawStr(const char* str);

/* 
    \brief Draws a ASCII character on the screen.

    \param ch Character that would be written (from ASCII)
    \return None
    \note Draws at the current cursor position.
*/
void DrawChar(uint8_t ch);


/* 
    \brief Draws a pixel at the specified position.

    \param x Position in range (from 0 to 127 inclusively)
    \param y Position in range (from 0 to 63 inclusively)
    \param clear_pixel Specifies either you draw (`false`) or clear (`true`) 
    \return None
    \note Doesn't depend on the current cursor position OR changes the current cursor position.
*/
void DrawPixel(uint8_t x, uint8_t y, bool clear_pixel);

/* 
    \brief Draws a vertical line at the specified x position. The length of the line is dictated
    by the y1 and y2 coordinates.

    \param x Position in range (from 0 to 127 inclusively)
    \param y1 Position in range (from 0 to 63 inclusively)
    \param y2 Position in range (from 0 to 63 inclusively)
    \return None
    \note Doesn't depend on the current cursor position OR changes the current cursor position.
*/
void DrawVerticalLine(uint8_t x, uint8_t y1, uint8_t y2);

/* 
    \brief Draws a horizontal line at the specified y position. The length of the line is dictated
    by the x1 and x2 coordinates.

    \param y Position in range (from 0 to 63 inclusively)
    \param x1 Position in range (from 0 to 127 inclusively)
    \param x2 Position in range (from 0 to 127 inclusively)
    \return None
    \note Doesn't depend on the current cursor position OR changes the current cursor position.
*/
void DrawHorizontalLine(uint8_t y, uint8_t x1, uint8_t x2);

#endif /* SSD1306_H */