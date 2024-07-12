//
// Created by cuong on 6/28/24.
//

#ifndef CCMV_CPLEX_GUROBI_CPLEXSOLVER_H
#define CCMV_CPLEX_GUROBI_CPLEXSOLVER_H

#include <bits/stdc++.h>

using namespace std;
namespace cplex_solver {
    bool solve(double u = 0.001, int K = 5, const string &constraint_type = "rho", int file_num = 1);
    void cplex_rho_constrained(int file_num = 1);
    void cplex_sig_constrained(int file_num = 1);
}

#endif //CCMV_CPLEX_GUROBI_CPLEXSOLVER_H
