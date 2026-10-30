#include "convert.h"

void ConvertDec(uint32_t num, char* str) {
	// Complete this function!
	uint8_t digits[4];
	uint8_t i;
	uint8_t leading;

	if (num > 9999) {
		str[0] = '*';
        str[1] = '*';
        str[2] = '*';
        str[3] = '*';
        str[4] = ' ';
        str[5] = '\0';
		return;
	}

	digits[0] = (num / 1000) % 10; 	// thousand
	digits[1] = (num / 100) % 10; 	// hundred
	digits[2] = (num / 10) % 10; 	// tens
	digits[3] = num % 10;			// ones

	leading = 1; // 1 = still in leading-zero region -> print space instead of digit
    for (i = 0; i < 4; i++) {
        if (digits[i] != 0 || i == 3) {
            leading = 0;             // ones digit is always printed, even if 0
        }
        str[i] = leading ? ' ' : ('0' + digits[i]);
    }
    str[4] = ' ';
    str[5] = '\0';
}

void ConvertDistance(uint32_t num, char* str) {
	// Complete this function!
	uint32_t whole;
    uint32_t frac;

    if (num > 9999) {
        str[0] = '*';
        str[1] = '.';
        str[2] = '*';
        str[3] = '*';
        str[4] = '*';
        str[5] = ' ';
        str[6] = 'c';
        str[7] = 'm';
        str[8] = '\0';
        return;
    }

    whole = num / 1000;
    frac  = num % 1000;

    str[0] = '0' + whole;
    str[1] = '.';
    str[2] = '0' + (frac / 100) % 10;
    str[3] = '0' + (frac / 10)  % 10;
    str[4] = '0' + frac % 10;
    str[5] = ' ';
    str[6] = 'c';
    str[7] = 'm';
    str[8] = '\0';
}
