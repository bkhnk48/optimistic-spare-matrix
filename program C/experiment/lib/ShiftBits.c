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

unsigned long increase(unsigned long id, int portENB){
    if(id == -1){
        id = 0;
        id |= 1;
        id |= (portENB << 16);
        return id;
    }
    else{
        int count = getCount(id);
        count++;
        int min = getMin(id);
        int max = getMax(id);
        if(max == 0)
            max = min;
        if(max < portENB)
        {
            max = portENB;
        }
        else{
            if(min > portENB)
                min = portENB;
        }
        id = 0;
        id |= (unsigned short)count;
        id |= (min << 16);
        id |= ((unsigned long)max << 32);
        return id;
    }

    return id;
}


#endif