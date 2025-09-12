#include <assert.h>
#include <iostream>
#include <string>
#include <tuple>

#include "Instance.h"
#include "Solver.h"

int main(int argc, char *argv[])
{
    std::string filename = std::string(argv[1]);
    std::cout << filename << std::endl << std::endl;

    Instance ins(filename);
    // std::cout << ins << std::endl;
    
    Solver solver;

    auto result_exact = solver.run_exact(ins);
    int z_optimal_exact = std::get<0>(result_exact);

    std::cout << "exact" << std::endl;
    std::cout << "z* " << z_optimal_exact << std::endl;
    assert (z_optimal_exact > ins.U);
    std::cout << "time[s] " << std::get<1>(result_exact) << std::endl;
    // std::cout << "extended_states_cnt " << std::get<2>(result_exact) << std::endl;
    // std::cout << "used_IDs_cnt " << std::get<3>(result_exact) << std::endl;
    
    for (int rho = 1; rho != ins.max_number_of_extensions; ++rho)
    {
        auto result_heuristic = solver.run_heuristic(ins, rho);
        int z_optimal_heuristic = std::get<0>(result_heuristic);

        std::cout << std::endl;
        std::cout << "heuristic " << rho << std::endl;
        std::cout << "z* " << z_optimal_heuristic << std::endl;
        std::cout << "time[s] " << std::get<1>(result_heuristic) << std::endl;

        assert(z_optimal_heuristic >= z_optimal_exact);
        if (z_optimal_heuristic == z_optimal_exact)
            break;
    }

    return 0;
}
