#include "clib.h"

void clib_swap(void** a, void** b)
{
    void* t = *a;
    *a = *b;
    *b = t;
}

void clib_swapi(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void clib_swapf(float* a, float* b)
{
    float t = *a;
    *a = *b;
    *b = t;
}

void clib_swapstr(char** a, char** b)
{
    char* t = *a;
    *a = *b;
    *b = t;
}
