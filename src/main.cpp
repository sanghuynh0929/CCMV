#include <iostream>
#include <filesystem>
#include "../include/CPLEXSolver.h"
#include "../include/GUROBISolver.h"
int main() {
    gurobi_solver::gurobi_rho_constrained(2);
//    gurobi_solver::gurobi_sig_constrained(2);

    gurobi_solver::gurobi_rho_constrained(3);
//    gurobi_solver::gurobi_sig_constrained(3);

    gurobi_solver::gurobi_rho_constrained(5);
//    gurobi_solver::gurobi_sig_constrained(5);

    gurobi_solver::gurobi_rho_constrained(4);
//    gurobi_solver::gurobi_sig_constrained(4);
    return 0;
}
