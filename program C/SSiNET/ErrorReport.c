#include <stdio.h>

void nullPointerException(int line, char *fileName, const char *funcName);
void outOfBoundException(int line, char *fileName, const char *funcName);

void nullPointerException(int line, char *fileName, const char *funcName)
{
    printf("\n NULL Pointer at here: %d in file: %s in function: %s", line, fileName, funcName);
}

void outOfBoundException(int line, char *fileName, const char *funcName)
{
    printf("\n OUT of Boundary Exception at here: %d in file: %s in function: %s", line, fileName, funcName);
}