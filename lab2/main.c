/*
    Lab2 - Functions in C

    Navigate to the calc_area.c to finish the CalcArea() function.
*/

#include "calc_area.h"
#include "verify.h"

#define BUF_SIZE 10

int main(void) {
    uint32_t length, width, area;
    char input_str[BUF_SIZE];
    ConfigureUART();
    UARTprintf("\nThis program calculates areas of rectangular rooms.\n");
    while (true) {
        UARTprintf("\nGive length: "); 
        UARTgets(input_str, BUF_SIZE);
        length = atoi(input_str);
        UARTprintf("\nGive width: ");  
        UARTgets(input_str, BUF_SIZE);
        width = atoi(input_str);
        area = CalcArea(length, width); // student submitted function
        UARTprintf("\nArea of the room = %d\n", area);
    }
}
