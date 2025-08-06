#ifndef BITSET_H
#define BITSET_H

#include "Globals.h"

#include <assert.h>
#include <stdint.h>
#include <vector>

/*
 * To optimize Bitset operations, we leverage the fact that the largest ID
 * ever assigned is known at any time.
 * No bitset will ever have a bit set to 1 beyond this maximum ID.
 * Therefore, we can use BITSET_LIMIT instead of BITSET_NUMBER_OF_WINDOWS
 * to accelerate the "inplace_intersection" and "any" functions.
 */

#define WINDOW_SIZE 64
#define BITSET_INDEX_OF(x) ((x) / WINDOW_SIZE)
#define BITSET_OFFSET_OF(x) ((x) % WINDOW_SIZE)

class Bitset
{
public:
    Bitset() : bs(BITSET_NUMBER_OF_WINDOWS, 0ULL) {};

    void set_bit(int x)
    {
        assert(x < WINDOW_SIZE * BITSET_LIMIT);
        bs[BITSET_INDEX_OF(x)] |= (1ULL << BITSET_OFFSET_OF(x));
    }

    void clear_bit(int x)
    {
        assert(x < WINDOW_SIZE * BITSET_LIMIT);
        bs[BITSET_INDEX_OF(x)] &= ~(1ULL << BITSET_OFFSET_OF(x));
    }

    void inplace_intersection(const Bitset &other)
    {
        for (int i = 0; i != BITSET_LIMIT; ++i)
            bs[i] &= other.bs[i];
    }

    bool any() const
    {
        for (int i = 0; i != BITSET_LIMIT; ++i)
            if (bs[i] > 0ULL)
                return true;
        return false;
    }

    std::vector<uint64_t> bs;
};

#endif