#include <stdio.h>
#include <math.h>

int main(void)
{

int num = 0b101 + 0b1000;
printf("num = %d\n", num);

//101 + 1000 = 1101;


float f = 5.3  + 4.83;
printf("Floatval is %4.20f\n", f);

printf("Floats are %lu size\n", sizeof(float));
}