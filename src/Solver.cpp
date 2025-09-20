#include "Solver.h"
#include "State.h"
#include "Pool.h"

#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <vector>

using std::tuple;
using std::vector;

int J;
int M;
vector<vector<int>> G_p;
vector<vector<int>> G_t;
vector<vector<vector<int>>> G_lambda_factors;
vector<int> G_mu_factors_1;
vector<vector<vector<int>>> G_mu_factors_2;

tuple<int, double, int, int> Solver::run_exact(Instance &ins, bool verbose)
{
    J = ins.J;
    M = ins.M;
    G_p = ins.p;
    G_t = ins.t;
    G_lambda_factors = ins.lambda_factors;
    G_mu_factors_1 = ins.mu_factors_1;
    G_mu_factors_2 = ins.mu_factors_2;

    auto start = std::chrono::high_resolution_clock::now();

    int best_obj_val = ins.U;
    State *best_state = nullptr;

    // creating the initial state

    State *initial = new State();

    std::fill((initial->x).begin(), (initial->x).end(), 0);
    initial->x[0] = 1;

    std::fill((initial->C).begin(), (initial->C).end(), INF);
    initial->C[0] = 0;

    std::fill((initial->n).begin(), (initial->n).end(), 0);

    initial->set_e();

    std::unordered_map<State *, State *> pred;
    pred.emplace(initial, nullptr);

    Pool pool(ins.U);
    pool.try_push(initial);

    while (!pool.is_empty())
    {

        State *s = pool.pop();

        // the first final state we reach is the optimal solution,
        // because states with lower t are popped from the pool first
        if (s->is_final())
        {
            assert(s->t <= best_obj_val);
            best_obj_val = s->t;
            best_state = s;
            break;
        }

        for (int m = 0; m != M + 1; ++m)
        {
            if (s->machine_is_non_idle_and_non_blocked(m))
            {

                int j = s->x[m];

                State *s_next = new State(*s);

                assert(s->C[m] != INF);
                s_next->t = std::max(s->t + G_t[s->y][m], s->C[m]) + G_t[m][m + 1];

                s_next->y = m + 1;

                if (m > 0 || j == J)
                {
                    s_next->x[m] = 0;
                    s_next->C[m] = INF;
                }
                else
                {
                    assert(m == 0 && j < J);
                    s_next->x[m] = j + 1;
                    s_next->C[m] = s_next->t;
                }

                if (m + 1 < M + 1)
                {
                    s_next->x[m + 1] = j;
                    s_next->C[m + 1] = s_next->t + G_p[m + 1][j];
                }

                ++(s_next->n[m]);
                s_next->set_e();

                bool pushed = pool.try_push(s_next);

                if (pushed)
                {
                    assert(pred.count(s_next) == 0);
                    pred.emplace(s_next, s);
                }
                else
                {
                    delete s_next;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    assert(best_state != nullptr);
    if (verbose)
    {
        std::cout << std::endl;
        print_optimal_solution(best_state, pred);
        std::cout << std::endl;
    }

    int extended_states_cnt = pred.size();
    int used_IDs_cnt = pool.get_next_free_id();

    for (const auto &key : pred)
    {
        delete key.first;
    }
    pred.clear();

    int z_optimal = best_obj_val;
    double execution_time = elapsed.count();
    return std::make_tuple(z_optimal, execution_time, extended_states_cnt, used_IDs_cnt);
}

tuple<int, double> Solver::run_heuristic(Instance &ins, int rho, bool verbose)
{
    J = ins.J;
    M = ins.M;
    G_p = ins.p;
    G_t = ins.t;
    G_lambda_factors = ins.lambda_factors;
    G_mu_factors_1 = ins.mu_factors_1;
    G_mu_factors_2 = ins.mu_factors_2;

    auto start = std::chrono::high_resolution_clock::now();

    int best_obj_val = ins.U;
    State *best_state = nullptr;

    vector<tuple<int, int>> heuristic_extension_candidates;
    auto cmp_function = [](const std::tuple<int, int> &a, const std::tuple<int, int> &b)
    {
        return std::get<1>(a) < std::get<1>(b);
    };

    State *initial = new State();

    std::fill((initial->x).begin(), (initial->x).end(), 0);
    initial->x[0] = 1;

    std::fill((initial->C).begin(), (initial->C).end(), INF);
    initial->C[0] = 0;

    std::fill((initial->n).begin(), (initial->n).end(), 0);

    initial->set_e();

    std::unordered_map<State *, State *> pred;
    pred.emplace(initial, nullptr);

    Pool pool(ins.U);
    pool.try_push(initial);

    while (!pool.is_empty())
    {

        State *s = pool.pop();

        if (s->is_final())
        {
            assert(s->t <= best_obj_val);
            best_obj_val = s->t;
            best_state = s;
            break;
        }

        heuristic_extension_candidates.clear();

        for (int m = 0; m != M + 1; ++m)
        {
            if (s->machine_is_non_idle_and_non_blocked(m))
            {
                assert(s->C[m] != INF);
                heuristic_extension_candidates.emplace_back(m, s->e[m]);
            }
        }

        if (rho >= heuristic_extension_candidates.size())
        {
            std::sort(
                heuristic_extension_candidates.begin(),
                heuristic_extension_candidates.end(),
                cmp_function);
        }
        else
        {
            std::partial_sort(
                heuristic_extension_candidates.begin(),
                heuristic_extension_candidates.begin() + rho,
                heuristic_extension_candidates.end(),
                cmp_function);
        }

        for (int i = 0; i != std::min(rho, static_cast<int>(heuristic_extension_candidates.size())); ++i)
        {
            int m = std::get<0>(heuristic_extension_candidates[i]);

            int j = s->x[m];

            State *s_next = new State(*s);

            assert(s->C[m] != INF);
            s_next->t = std::max(s->t + G_t[s->y][m], s->C[m]) + G_t[m][m + 1];

            // can only be true in heuristic case
            if (s_next->t > best_obj_val)
            {
                delete s_next;
                continue;
            }

            s_next->y = m + 1;

            if (m > 0 || j == J)
            {
                s_next->x[m] = 0;
                s_next->C[m] = INF;
            }
            else
            {
                assert(m == 0 && j < J);
                s_next->x[m] = j + 1;
                s_next->C[m] = s_next->t;
            }

            if (m + 1 < M + 1)
            {
                s_next->x[m + 1] = j;
                s_next->C[m + 1] = s_next->t + G_p[m + 1][j];
            }

            ++(s_next->n[m]);
            s_next->set_e();

            bool pushed = pool.try_push(s_next);

            if (pushed)
            {
                assert(pred.count(s_next) == 0);
                pred.emplace(s_next, s);
            }
            else
            {
                delete s_next;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    if (verbose)
    {
        if (best_state != nullptr)
        {
            std::cout << std::endl;
            print_optimal_solution(best_state, pred);
            std::cout << std::endl;
        }
        else
        {
            std::cout << std::endl
                      << "trivial solution is the optimal one" << std::endl
                      << std::endl;
        }
    }

    for (const auto &key : pred)
    {
        delete key.first;
    }
    pred.clear();

    int z_optimal = best_obj_val;
    double execution_time = elapsed.count();
    return std::make_tuple(z_optimal, execution_time);
}

void Solver::print_optimal_solution(State *s, std::unordered_map<State *, State *> pred)
{
    if (pred[s] != nullptr)
        print_optimal_solution(pred[s], pred);
    std::cout << *s;
}