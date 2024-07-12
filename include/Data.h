//
// Created by cuong on 7/3/24.
//

#ifndef CCMV_CPLEX_GUROBI_DATA_H
#define CCMV_CPLEX_GUROBI_DATA_H
#include <bits/stdc++.h>

class Data {
public:
    int N;
    int T;
    std::vector<double> index;
    std::vector<std::vector<double>> stock_prices;
    std::vector<std::vector<double>> stock_returns;
    std::vector<double> mean;
    std::vector<std::vector<double>> cov;

    Data();
    void read_data(const char*);
};


#endif //CCMV_CPLEX_GUROBI_DATA_H
