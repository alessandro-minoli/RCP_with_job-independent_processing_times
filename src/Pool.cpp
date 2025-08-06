#include "Pool.h"

#include <assert.h>
#include <bit>

using std::vector;

bool Pool::try_push(State *s)
{
    assert(s != nullptr);
    // assert(s->count_not_started < J);

    /*************************************************************************
     *** checking if some state in the pool dominates s **********************
     *************************************************************************/

    /*
     *  we set the bitset candidate_dominators to contain
     *  the ids of the states s' in the pool
     *  such that s'->xk[k] >= s->xk[k] for all k in [0...J)
     */

    // candidate_dominators.clear_all(); // non dovrebbe servire

    // it is sufficient to intersecate the bitsets relative to products k s.t. start <= k < end
    candidate_dominators = geq[0][s->n[0]];

    // for (int i = 1; i != M+1 && candidate_dominators.any(); ++i)
    for (int i = 1; i != M+1 && candidate_dominators.any(); ++i)
    {
        candidate_dominators.inplace_intersection(geq[i][s->n[i]]);
    }

    /*
     *  the loop iterates on the indexes of bits set to 1 in candidate_dominators,
     *  that represent the ids of the states to check for dominance.
     *  if some state in candidate_dominators dominates s,
     *  then s is not added to the pool and we return false.
     */

    if (candidate_dominators.any())
    {
        int offset = 0;
        for (int i = 0; i != BITSET_LIMIT; ++i)
        {
            uint64_t word = candidate_dominators.bs[i];
            while (word != 0)
            {
                uint64_t t = word & -word;
                int id = offset + std::countr_zero(word);
                State *dominator = ids.get_state_from_id(id);
                if (dominator->dominates(s))
                {
                    return false;
                }
                word ^= t;
            }
            offset += 64;
        }
    }
    // std::cout << "try_push punto1" << std::endl;

    /*************************************************************************
     *** checking if s dominates some state in the pool **********************
     *************************************************************************/

    /*
     *  we set the bitset candidate_dominated to contain
     *  the ids of the states s' in the pool
     *  such that s'->xk[k] <= s->xk[k] for all k in [0...J)
     */

    // bitset_clear_all(pool->candidate_dominated_non_aligned);

    // it is sufficient to intersecate the bitsets relative to products k s.t. start <= k <= end
    candidate_dominated = leq[0][s->n[0]];

    // for (int i = 1; i != M+1 && candidate_dominators.any(); ++i)
    for (int i = 1; i != M+1 && candidate_dominated.any(); ++i)
    {
        candidate_dominated.inplace_intersection(leq[i][s->n[i]]);
    }

    /*
     *  the loop iterates on the indexes of bits set to 1 in candidate_dominated,
     *  that represent the ids of the states to check for dominance.
     *  if s dominates some state s' in candidate_dominated,
     *  then s' must be removed from the pool.
     */

    if (candidate_dominated.any())
    {
        int offset = 0;
        for (int i = 0; i != BITSET_LIMIT; ++i)
        {
            uint64_t word = candidate_dominated.bs[i];
            while (word != 0)
            {
                uint64_t t = word & -word;
                int id = offset + std::countr_zero(word);
                State *dominated = ids.get_state_from_id(id);
                if (s->dominates(dominated))
                {   
                    remove(dominated);
                }
                word ^= t;
            }
            offset += 64;
        }
    }

    // std::cout << "try_push punto2" << std::endl;
    /*************************************************************************/

    push(s);

    return true;
}

/*
bool Pool::try_push(State *s, const vector<int> &P, const vector<vector<int>> &T)
{
    assert(s != nullptr);

    // checking if some state in the pool dominates s

    auto slot = 0;
    auto slot_idx = 0;

    while (slot < time_slots.size())
    {
        while (slot_idx < time_slots[slot].size())
        {
            if (time_slots[slot][slot_idx] != nullptr)
            {
                State *dominator = time_slots[slot][slot_idx];
                if (dominator->dominates(s, P, T))
                {
                    return false;
                }
            }
            ++slot_idx;
        }
        ++slot;
        slot_idx = 0;
    }

    // checking if s dominates some state in the pool

    slot = 0;
    slot_idx = 0;

    while (slot < time_slots.size())
    {
        while (slot_idx < time_slots[slot].size())
        {
            if (time_slots[slot][slot_idx] != nullptr)
            {
                State *dominated = time_slots[slot][slot_idx];
                if (s->dominates(dominated, P, T))
                {
                    remove(dominated);
                }
            }
            ++slot_idx;
        }
        ++slot;
        slot_idx = 0;
    }

    push(s);

    return true;
}
*/

void Pool::push(State *s)
{
    ids.assign_id(s);

    time_slots[s->t].push_back(s);
    s->idx_in_time_slot = time_slots[s->t].size() - 1;

    for (int i = 0; i != M+1; ++i)
    {
        int jobs_count = s->n[i];
        for (int j = 0; j != jobs_count; ++j)
        {
            geq[i][j].set_bit(s->id);
        }
        geq[i][jobs_count].set_bit(s->id);
        leq[i][jobs_count].set_bit(s->id);
        for (int j = jobs_count + 1; j != J + 1; ++j)
        {
            leq[i][j].set_bit(s->id);
        }
    }
}

bool Pool::is_empty()
{
    while (cur_slot < time_slots.size())
    {
        while (cur_slot_idx < time_slots[cur_slot].size())
        {
            if (time_slots[cur_slot][cur_slot_idx] != nullptr)
            {
                return false;
            }
            ++cur_slot_idx;
        }
        // time_slots[cur_slot].clear();
        ++cur_slot;
        cur_slot_idx = 0;
    }
    return true;
}

State *Pool::pop()
{
    assert(!is_empty());
    assert(
        cur_slot_idx >= 0 &&
        cur_slot_idx < time_slots[cur_slot].size());

    State *s = time_slots[cur_slot][cur_slot_idx];
    assert(s != nullptr);
    ++cur_slot_idx;

    remove(s);

    return s;
}

void Pool::remove(State *s)
{
    time_slots[s->t][s->idx_in_time_slot] = nullptr;

    for (int i = 0; i != M+1; ++i)
    {
        int jobs_count = s->n[i];
        for (int j = 0; j != jobs_count; ++j)
        {
            geq[i][j].clear_bit(s->id);
        }
        geq[i][jobs_count].clear_bit(s->id);
        leq[i][jobs_count].clear_bit(s->id);
        for (int j = jobs_count + 1; j != J + 1; ++j)
        {
            leq[i][j].clear_bit(s->id);
        }
    }
    
    ids.release_id(s);
}