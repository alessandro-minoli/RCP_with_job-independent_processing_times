#ifndef STATE_H
#define STATE_H

#include "Globals.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>

class State
{
public:
    State() : t(0), y(0), x(M + 1), C(M + 1), n(M + 1), e(M + 1), idx_in_time_slot(-1), id(-1) {};
    State(const State &other) : t(0), y(0), e(M + 1), idx_in_time_slot(-1), id(-1)
    {
        x = other.x;
        C = other.C;
        n = other.n;
    };

    std::ostream &display(std::ostream &os) const;

    void set_e();

    bool is_final() const
    {
        return y == M + 1 && std::all_of(x.begin(), x.end(), [](int x_){ return x_ == 0; });
        // return y == M + 1 &&
        //        std::all_of(x.begin(), x.end(), [](int x_)
        //                    { return x_ == 0; }) &&
        //        std::all_of(C.begin(), C.end(), [](int x_)
        //                    { return x_ == INF; });
    };

    bool machine_is_blocked(int m) const
    {
        assert(m >= 0 && m < M + 1);
        return m < M && x[m + 1] != 0;
    };

    bool machine_is_non_idle_and_non_blocked(int m) const
    {
        assert(m >= 0 && m < M + 1);
        return C[m] != INF && !machine_is_blocked(m);
    };

    int get_lambda(int k, int j) const
    {
        assert (j >= 1 && j <= J);
        // m is the machine on which job j is
        int m = 0;
        if (x[0] > j)
        {
            for (int i = 1; i != M + 1; ++i)
            {
                if (x[i] == j)
                {
                    m = i;
                    break;
                }
            }
            assert(m > 0);
        }
        assert(k >= m);
        int lambda = e[m] + G_lambda_factors[m][k];
        // for (int h = m + 1; h <= k; ++h)
        // {
        //     lambda += G_t[h - 1][h] + G_p[h];
        // }
        return lambda;
    };

    int get_mu(int m, int i) const
    {
        assert(m < M + 1);
        int j = x[m]; // j is the job on machine m
        assert(i >= j);
        int mu = e[m] + (i - j) * G_mu_factors[m];
        return mu;
    };

    bool dominates(const State *other) const;
                   
    // tenere aggiornato il copy constructor in base ai campi

    int t;
    int y;

    std::vector<int> x; // |M+1|
    // x[m] = 0 if m is idle
    // x[0] indicates the first unprocessed job in the initial buffer

    std::vector<int> C; // |M+1|
    // C[m] = INF if m is idle
    // C[0] = INF iff all jobs have left the initial buffer

    std::vector<int> n; // |M+1|
    std::vector<int> e; // |M+1|

    int idx_in_time_slot;
    int id;
};

#endif