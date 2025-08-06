#ifndef POOL_H
#define POOL_H

#include "Globals.h"
#include "State.h"
#include "IDs.h"
#include "Bitset.h"

#include <vector>

class Pool
{
public:
    Pool() = default;

    // Pool(int U) : time_slots(U + 1), cur_slot(0), cur_slot_idx(0) {};

    // Pool(int U, const std::vector<std::vector<int>> &M)
    //     : M(M), time_slots(U + 1), cur_slot(0), cur_slot_idx(0)

    Pool(int U) : time_slots(U + 1), cur_slot(0), cur_slot_idx(0)
    {
        for (int i = 0; i != M+1; ++i)
        {
            std::vector<Bitset> geq_i;
            std::vector<Bitset> leq_i;
            for (int j = 0; j != J+1; ++j)
            {
                geq_i.push_back(Bitset());
                leq_i.push_back(Bitset());
            }
            geq.push_back(geq_i);
            leq.push_back(leq_i);
        }
    };

    bool try_push(State *s);
    bool is_empty();
    State *pop();

    int get_next_free_id() { return ids.get_next_free_id(); };

private:
    // const std::vector<std::vector<int>> &M;

    std::vector<std::vector<State *>> time_slots;
    int cur_slot;
    int cur_slot_idx;

    IDs ids;
    std::vector<std::vector<Bitset>> geq;
    std::vector<std::vector<Bitset>> leq;
    Bitset candidate_dominators;
    Bitset candidate_dominated;

    void push(State *s);
    void remove(State *s);
};

#endif