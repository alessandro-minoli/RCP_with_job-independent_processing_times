#include "State.h"

#include <assert.h>

std::ostream &operator<<(std::ostream &os, const State &s)
{
    os << "t " << s.t << " y " << s.y;
    // os << " idx_in_time_slot " << s.idx_in_time_slot << std::endl;

    os << " x ";
    for (const auto &el : s.x)
        os << el << " ";

    os << " C ";
    for (const auto &el : s.C)
        if (el == INF)
            os << "inf ";
        else
            os << el << " ";
    os << std::endl;

    /*
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
    */

    return os;
}

bool State::dominates(const State *other) const
{

    // we are assuming this has been generated before than other
    if (t == other->t && y == other->y && x == other->x && C == other->C)
        return true;

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