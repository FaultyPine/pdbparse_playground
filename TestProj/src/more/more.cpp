#include "more.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"

struct MoreStaticGlobalData
{
    const char* static_str = "HELLOTHERE\n";
    int static_member1 = 40;
    float static_member2 = 0.0f;
};
static MoreStaticGlobalData g_moreglobaldata;

void MoreRandomOperations()
{
    g_moreglobaldata.static_member1++;
    printf("%i %s\n", g_moreglobaldata.static_member1, g_moreglobaldata.static_str);
}

int MessWithABuffer(int* buffer, size_t size)
{
    int rand = HashBytes((char*)&buffer + uint64_t(&g_moreglobaldata), sizeof(char*)); // hash the pointer for randomness
    int numMesses = (rand % 10) + 2;
    for (int i = 0; i < numMesses; i++)
    {
        int offsetToMessWith = rand % size;
        buffer[offsetToMessWith] += 1;
        rand = HashBytes((char*)rand, sizeof(rand));
    }
    return numMesses;
}