#ifndef IDS_H
#define IDS_H

#include "Globals.h"
#include "State.h"

#include <assert.h>
#include <cmath>
#include <vector>

class IDs
{
public:
    IDs() : next_free_id(0), id_to_state(POOL_SIZE, nullptr) {};
    
    void assign_id(State *s)
    {
        assert(s->id == -1);
        if (released_ids.size() > 0)
        {
            s->id = released_ids.back();
            released_ids.pop_back();
        }
        else
        {
            if (next_free_id == POOL_SIZE)
            {
                std::cerr << "ERROR, " << __FILE__ << " : in function " << __func__ << " at line " << __LINE__ << std::endl
                          << "       Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
                          << "       POOL_SIZE must be increased" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            s->id = next_free_id;

            BITSET_LIMIT = ceil(s->id / 64) + 1;
            assert(BITSET_LIMIT <= BITSET_NUMBER_OF_WINDOWS);
            
            ++next_free_id;
        }
        id_to_state[s->id] = s;
    }

    void release_id(State *s)
    {
        assert(s->id != -1);
        released_ids.push_back(s->id);
        id_to_state[s->id] = nullptr;
        s->id = -1; // non so se necessario
    }

    State *get_state_from_id(int id)
    {
        assert(id >= 0 && id < POOL_SIZE);
        assert(id_to_state[id] != nullptr);
        return id_to_state[id];
    }

    int get_next_free_id() { return next_free_id; };

private:
    int next_free_id;
    std::vector<int> released_ids;
    std::vector<State *> id_to_state;
};

#endif