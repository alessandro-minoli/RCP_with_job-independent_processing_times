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

    int J = 0;
    int M = 0;
    std::vector<std::vector<int>> p; // (M+1) x (J+1) , M[0][...] = 0 and M[i][0] = 0
    std::vector<std::vector<int>> t; // (M+2) x (M+2)

    int U = 0;
    int max_number_of_extensions = 0;

    std::vector<std::vector<std::vector<int>>> lambda_factors;
    std::vector<int> mu_factors_1;
    std::vector<std::vector<std::vector<int>>> mu_factors_2;
};

std::ostream &operator<<(std::ostream &os, const Instance &ins);

#endif