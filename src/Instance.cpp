#include "Instance.h"

#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

Instance::Instance(const string &filename)
{
    std::ifstream in(filename);
    if (!in)
    {
        std::cerr << "ERROR, " << __FILE__ << " : in function " << __func__ << " at line " << __LINE__ << endl
                  << "       Compiled on " << __DATE__ << " at " << __TIME__ << endl
                  << "       File " << filename << " does not exists" << endl;
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
        vector<int> t_i;
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

    int tot_forMard_travel_time = 0;
    for (int i = 0; i != M + 1; ++i)
    {
        tot_forMard_travel_time += t[i][i + 1];
    }
    tot_forMard_travel_time *= J;

    int tot_backMard_travel_time = t[M + 1][0] * (J - 1);

    U = tot_processing_time + tot_forMard_travel_time + tot_backMard_travel_time;

    // initializing lambda_factors

    for (int i = 0; i != M + 2; ++i)
    {
        vector<int> lambda_factors_i(M+2, -1);
        lambda_factors_i[i] = 0;
        for (int j = i+1; j != M + 2; ++j)
        {
            lambda_factors_i[j] = lambda_factors_i[j-1] + t[j-1][j] + p[j];
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

std::ostream &Instance::display(std::ostream &os) const
{
    os << "J = " << J << endl;
    os << "M = " << M << endl;
    os << "U = " << U << endl;
    os << "max_number_of_extensions = " << max_number_of_extensions << endl;

    os << "p =\t";
    for (auto const &el : p)
    {
        os << el << " ";
    }
    os << endl;

    os << "t =" << endl;
    for (int i = 0; i != M + 2; ++i)
    {
        os << "\t";
        for (int j = 0; j != M + 2; ++j)
        {
            os << t[i][j] << " ";
        }
        os << endl;
    }

    os << "lambda_factors =" << endl;
    for (int i = 0; i != M + 2; ++i)
    {
        os << "\t";
        for (int j = 0; j != M + 2; ++j)
        {
            os << lambda_factors[i][j] << " ";
        }
        os << endl;
    }

    os << "mu_factors =\t";
    for (auto const &el : mu_factors)
    {
        os << el << " ";
    }
    os << endl;

    for (int i = 0; i != 80; ++i)
    {
        os << "-";
    }
    os << endl;

    return os;
}

// int Instance::get_optimal_solution(const std::string &filename)
// {
//     std::ifstream in(filename);
//     if (!in)
//     {
//         std::cerr << "ERROR, " << __FILE__ << " : in function " << __func__ << " at line " << __LINE__ << endl
//                   << "       Compiled on " << __DATE__ << " at " << __TIME__ << endl
//                   << "       File " << filename << " does not exists" << endl;
//         exit(EXIT_FAILURE);
//     }
//     int z;
//     in >> z;
//     in.close();
//     return z;
// }