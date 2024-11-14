#include "more.h"

#include <stdlib.h>
#include <string.h>

#include "hash.h"

int MessWithABuffer(int* buffer, size_t size)
{
    int rand = HashBytes((char*)&buffer, sizeof(char*)); // hash the pointer for randomness
    int numMesses = (rand % 10) + 2;
    for (int i = 0; i < numMesses; i++)
    {
        int offsetToMessWith = rand % size;
        buffer[offsetToMessWith] += 1;
        rand = HashBytes((char*)rand, sizeof(rand));
    }
    return numMesses;
}