#ifndef INSTANCE_H
#define INSTANCE_H

#include "Globals.h"

#include <iostream>
#include <string>
#include <vector>

class Instance
{
public:
    Instance() = default;
    Instance(const std::string &filename);
    std::ostream &display(std::ostream &os) const;

    // static int get_optimal_solution(const std::string &filename);

    int J = 0;
    int M = 0;
    std::vector<int> p;              // (W+2) , p[0] = p[W+1] = 0
    std::vector<std::vector<int>> t; // (W+2) x (W+2)
    
    int U = 0;
    int max_number_of_extensions = 0;
    
    std::vector<std::vector<int>> lambda_factors; // (W+2) x (W+2)
    std::vector<int> mu_factors;
};

#endif