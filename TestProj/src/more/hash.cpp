#include "hash.h"

uint64_t HashBytesL(char* data, int size)
{
    // FNV-1 hash
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    constexpr uint64_t FNV_offset_basis = 0xcbf29ce484222325;
    constexpr uint64_t FNV_prime = 0x100000001b3;
    uint64_t hash = FNV_offset_basis;
    for (int i = 0; i < size; i++)
    {
        char byte_of_data = data[i];
        hash = hash * FNV_prime;
        hash = hash ^ byte_of_data;
    }
    return hash;
}
int HashBytes(char* data, int size)
{
    // FNV-1 hash -> doing the *PRIME, and THEN the XOR.
    // FNV-1a hash -> doing the XOR, then the *PRIME
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    constexpr int FNV_offset_basis = 0x811c9dc5;
    constexpr int FNV_prime = 0x01000193;
    int hash = FNV_offset_basis;
    for (int i = 0; i < size; i++)
    {
        char byte_of_data = data[i];
        hash = hash ^ byte_of_data;
        hash = hash * FNV_prime;
    }
    return hash;
}