#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef _LIB_OF_SHIFT_BITS_
#define _LIB_OF_SHIFT_BITS_

int getCount(unsigned long id){
    return (id & USHRT_MAX);
}

int getMin(unsigned long id){
    return ((id >> 16) & USHRT_MAX);
}

int getMax(unsigned long id){
    return ((id >> 32) & USHRT_MAX);
}
#endif