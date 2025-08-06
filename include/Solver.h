#ifndef SOLVER_H
#define SOLVER_H

#include "Globals.h"
#include "Instance.h"
#include "State.h"

#include <tuple>
#include <unordered_map>

class Solver
{
public:
    Solver() = default;
    std::tuple<int, double, int, int> run_exact(Instance &ins, bool verbose = false);
    std::tuple<int, double> run_heuristic(Instance &ins, int rho, bool verbose = false);

private:
    static void print_optimal_solution(State *s, std::unordered_map<State *, State *> pred);
};

#endif