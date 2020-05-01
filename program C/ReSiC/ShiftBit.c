#include <stdio.h>

/* This function will return 0 if a != b
 * in case a == b the result is 1
 */

int checkEqual(int a, int b)
{
    int x = a - b;
    int y = b - a;
    //x*y = (-1) * ((a-b)^2 <= 0
    x = x*y;
    y = x >> 31;
    y = 1 + y;
    return y;
}