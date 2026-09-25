#ifndef CALC_AREA_H
#define CALC_AREA_H

#include <stdint.h>

/* 
    Computes the area of a rectangular room whose sides are given.
    \param l length of a rectangle
    \param w width of a rectangle
    \return area of the rectangular room 
    \note Return a result of ``0`` if 
        the length is less than 3,  
        the width is less than 3,  
        the length is greater than 20 or  
        the width is greater than 20.
*/
uint32_t CalcArea(uint32_t l, uint32_t w);

#endif /* CALC_AREA_H */
