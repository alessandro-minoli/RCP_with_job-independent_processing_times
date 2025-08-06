#include "State.h"

#include <assert.h>

using std::vector;

void State::set_e()
{
    for (int m = M; m != -1; --m)
    {
        if (machine_is_blocked(m))
        {
            e[m] = std::max(C[m], e[m + 1] + G_t[m + 1][m + 2] + G_t[m + 2][m]);
        }
        else
        {
            e[m] = std::max(C[m], t + G_t[y][m]);
        }
    }
}

std::ostream &State::display(std::ostream &os) const
{
    os << "t " << t
       << " y " << y
       << " idx_in_time_slot " << idx_in_time_slot << std::endl;

    os << "x ";
    for (const auto &el : x)
        os << el << " ";
    os << std::endl;

    os << "C ";
    for (const auto &el : C)
        if (el == INF)
            os << "inf ";
        else
            os << el << " ";
    os << std::endl;

    os << "n ";
    for (const auto &el : n)
        os << el << " ";
    os << std::endl;

    os << "e ";
    for (const auto &el : e)
        if (el == INF)
            os << "inf ";
        else
            os << el << " ";
    os << std::endl;

    for (int i = 0; i != 80; ++i)
    {
        os << "-";
    }
    os << std::endl;

    return os;
}

bool State::dominates(const State *other) const
{
    
    // we are assuming this has been generated before than other
    if (t == other->t && y == other->y && x == other->x && C == other->C)
        return true;

    // for (int m = 0; m != M + 1; ++m)
    // {
    //     if (n[m] < other->n[m])
    //     {
    //         return false;
    //     }
    // }

    for (int m = 0; m != M + 1; ++m)
    {
        if (x[m] != 0)
        {
            int lambda = other->get_lambda(m, x[m]);
            int mu = other->get_mu(m, x[m]);
            if (e[m] > std::max(lambda, mu))
            {
                return false;
            }
        }
    }
    
    return true;
}