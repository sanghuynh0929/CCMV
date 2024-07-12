//
// Created by cuong on 7/3/24.
//

#ifndef CCMV_CPLEX_GUROBI_GUROBISOLVER_H
#define CCMV_CPLEX_GUROBI_GUROBISOLVER_H

#include <bits/stdc++.h>

using namespace std;
namespace gurobi_solver {
    bool solve(double u = 0.001, int K = 5, const string &constraint_type = "rho", int file_num = 1);
    void gurobi_rho_constrained(int file_num = 1);
    void gurobi_sig_constrained(int file_num = 1);
}

#endif //CCMV_CPLEX_GUROBI_GUROBISOLVER_H
