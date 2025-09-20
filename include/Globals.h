#ifndef GLOBALS_H
#define GLOBALS_H

#include <limits>
#include <vector>

#define INF std::numeric_limits<int>::max()

#define POOL_SIZE 1000000
#define BITSET_NUMBER_OF_WINDOWS (POOL_SIZE + 63) / 64

extern int J;
extern int M;
extern std::vector<std::vector<int>> G_p;
extern std::vector<std::vector<int>> G_t;
extern std::vector<std::vector<std::vector<int>>> G_lambda_factors; // G_lambda_factors[j][m][k]
extern std::vector<int> G_mu_factors_1;                             // G_mu_factors_1[m]
extern std::vector<std::vector<std::vector<int>>> G_mu_factors_2;   // G_mu_factors_2[m][j][i]

inline int BITSET_LIMIT = 1;

#endif