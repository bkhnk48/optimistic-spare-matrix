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

int increase(unsigned long id, int portENB){
    if(id == -1){
        id = 0;
        id |= 1;
        id |= (portENB << 16);
        return id;
    }

    return id;
}
#endif