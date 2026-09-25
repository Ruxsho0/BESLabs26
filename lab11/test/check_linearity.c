#include <stdio.h>
#include <stdint.h>

// Enter values collected from the potentiometer,
// they must be [adc_value:calibrated_distance]
long x[5] = {100, 200, 300, 400, 500};
long y[5] = {1, 2, 3, 4, 5};

/*
    \brief Checks the linearity of the linear regression model.
    \param None
    \return r^2 correlation coefficient of the linear regression (0-100)
*/
uint32_t CheckLinearity(void) {
    long sumx  = x[0] + x[1] + x[2] + x[3] + x[4];
    long sumy  = y[0] + y[1] + y[2] + y[3] + y[4];
    long sumxy = x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3] + x[4] * y[4]; // sum of x*y
    long sumx2 = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3] + x[4] * x[4]; // sum of x^2
    long sumy2 = y[0] * y[0] + y[1] * y[1] + y[2] * y[2] + y[3] * y[3] + y[4] * y[4]; // sum of y^2
    long numerator25 = 5 * sumxy - sumx * sumy;
    for (uint32_t n = 0; (numerator25 > 46340 || numerator25 < -46340) && n < 8; n++) { // divide by 2^n until numerator is less than sqrt(2^31-1)=46,340
        x[0] >>= 1; x[1] >>= 1; x[2] >>= 1; x[3] >>= 1; x[4] >>= 1;
        y[0] >>= 1; y[1] >>= 1; y[2] >>= 1; y[3] >>= 1; y[4] >>= 1;
        sumx  = x[0] + x[1] + x[2] + x[3] + x[4];
        sumy  = y[0] + y[1] + y[2] + y[3] + y[4];
        sumxy = x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3] + x[4] * y[4]; // sum of x*y
        sumx2 = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3] + x[4] * x[4]; // sum of x^2
        sumy2 = y[0] * y[0] + y[1] * y[1] + y[2] * y[2] + y[3] * y[3] + y[4] * y[4]; // sum of y^2
        numerator25 = 5 * sumxy - sumx * sumy;
    }
    long denominator0_25 = (5 * sumx2 - sumx * sumx) * (5 * sumy2 - sumy * sumy) / 100;
    return numerator25 * numerator25 / denominator0_25;
}

int main(void) {
    printf("Correlation coefficient: %d\n", CheckLinearity());
    return 0;
}