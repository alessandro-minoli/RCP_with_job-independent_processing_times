#ifndef BITSET_H
#define BITSET_H

#define WINDOW_SIZE 64
#define BITSET_INDEX_OF(x) ((x) / WINDOW_SIZE)
#define BITSET_OFFSET_OF(x) ((x) % WINDOW_SIZE)

#include "Globals.h"

#include <assert.h>
#include <stdint.h>
#include <vector>

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

/*
 * J = W+2 = 16
 *
 * POOL_SIZE = 100_000
 * we need 3 * 16 * 16 = 768 bitsets,
 * each bitset is composed of 1563 int64 = 12504 bytes -> 9,603072 MB in total
 *
 * ----------------------------------------------------------------------------
 *
 * POOL_SIZE = 1_000_000
 * each bitset is composed of 15625 int64 = 125000 bytes -> 96 MB in total
 *
 * we need 3 * 17 * 17 =  867 bitsets -> 108.375 MB
 * we need 3 * 18 * 18 =  972 bitsets -> 121.5   MB
 * we need 3 * 19 * 19 = 1083 bitsets -> 135.375 MB
 * we need 3 * 20 * 20 = 1200 bitsets -> 150.000 MB
 *
 * bottleneck is not memory space.
 * we can have fixed size bitsets even if we would set POOL_SIZE = 1_000_000
 */

/*
 * to speed up operations on bitset we exploit the fact that we know
 * the largest ID ever given to a state at any moment
 * -> no bitset will ever have set to 1 a bit that is > the largest ID
 * -> we can use LIMIT instead of BITSET_ARR_LEN in the functions
 *      bitset_copy
 *      bitset_inplace_intersection
 *      bitset_inplace_difference
 *      bitset_any
 */