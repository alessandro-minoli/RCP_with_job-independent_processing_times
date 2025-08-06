#include "Instance.h"

#include <fstream>

Instance::Instance(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        std::cerr << "ERROR, " << __FILE__ << " : in function " << __func__ << " at line " << __LINE__ << std::endl
                  << "       Compiled on " << __DATE__ << " at " << __TIME__ << std::endl
                  << "       File " << filename << " does not exists" << std::endl;
        exit(EXIT_FAILURE);
    }

    in >> M;
    in >> J;

    p.push_back(0);
    for (int i = 1, x = 0; i != M + 1; ++i)
    {
        in >> x;
        p.push_back(x);
    }
    p.push_back(0);

    for (int i = 0; i != M + 2; ++i)
    {
        std::vector<int> t_i;
        int x;
        for (int j = 0; j != M + 2; ++j)
        {
            in >> x;
            t_i.push_back(x);
        }
        t.push_back(t_i);
    }

    in.close();

    max_number_of_extensions = M / 2 + 1;

    // initializing U

    int tot_processing_time = 0;
    for (int i = 1; i != M + 1; ++i)
    {
        tot_processing_time += p[i];
    }
    tot_processing_time *= J;

    int tot_forward_travel_time = 0;
    for (int i = 0; i != M + 1; ++i)
    {
        tot_forward_travel_time += t[i][i + 1];
    }
    tot_forward_travel_time *= J;

    int tot_backward_travel_time = t[M + 1][0] * (J - 1);

    U = tot_processing_time + tot_forward_travel_time + tot_backward_travel_time;

    // initializing lambda_factors

    for (int i = 0; i != M + 2; ++i)
    {
        std::vector<int> lambda_factors_i(M + 2, -1);
        lambda_factors_i[i] = 0;
        for (int j = i + 1; j != M + 2; ++j)
        {
            lambda_factors_i[j] = lambda_factors_i[j - 1] + t[j - 1][j] + p[j];
        }
        lambda_factors.push_back(lambda_factors_i);
    }

    // initializing mu_factors

    mu_factors.push_back(t[0][1] + t[1][0]);
    for (int m = 1; m != M + 1; ++m)
    {
        mu_factors.push_back(t[m][m + 1] + t[m + 1][m - 1] + t[m - 1][m] + p[m]);
    }
}

std::ostream &operator<<(std::ostream &os, const Instance &ins)
{
    os << "J = " << ins.J << std::endl;
    os << "M = " << ins.M << std::endl;
    os << "p =\t";
    for (auto const &el : ins.p)
    {
        os << el << " ";
    }
    os << std::endl;

    os << "t =" << std::endl;
    for (int i = 0; i != ins.M + 2; ++i)
    {
        os << "\t";
        for (int j = 0; j != ins.M + 2; ++j)
        {
            os << ins.t[i][j] << " ";
        }
        os << std::endl;
    }
    os << "U = " << ins.U << std::endl;

    /*
    os << "max_number_of_extensions = " << ins.max_number_of_extensions << std::endl;
    os << "lambda_factors =" << std::endl;
    for (int i = 0; i != ins.M + 2; ++i)
    {
        os << "\t";
        for (int j = 0; j != ins.M + 2; ++j)
        {
            os << ins.lambda_factors[i][j] << " ";
        }
        os << std::endl;
    }

    os << "mu_factors =\t";
    for (auto const &el : ins.mu_factors)
    {
        os << el << " ";
    }
    os << std::endl;
    */

    return os;
}