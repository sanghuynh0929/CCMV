//
// Created by cuong on 7/3/24.
//

#include "../include/GUROBISolver.h"
#include "../include/Data.h"
#include <chrono>
#include <gurobi_c++.h>

using namespace chrono;
namespace gurobi_solver {

    stringstream ss;
    bool solve(double u, int K, const string &constraint_type, int file_num) {
        string file_name = "indtrack" + to_string(file_num) + ".txt";
        filesystem::path current_work = filesystem::current_path().parent_path().parent_path().parent_path();
        filesystem::path data_path = current_work / "data" / file_name;

        Data data;
        data.read_data(data_path.c_str());

        GRBEnv env;
        try {
            GRBModel model = GRBModel(env);

            // Variables
            vector<GRBVar> x(data.N);
            vector<GRBVar> z(data.N);
            for (int i = 0; i < data.N; ++i) {
                x[i] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, "x" + to_string(i));
                z[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z" + to_string(i));
            }

            double epsilon = 0;
            double delta = 1;

            GRBLinExpr mean = 0;
            for (int i = 0; i < data.N; ++i) {
                mean += x[i] * data.mean[i];
            }

            GRBQuadExpr variance = 0;
            for (int i = 0; i < data.N; ++i) {
                for (int j = 0; j < data.N; ++j) {
                    variance += x[i] * data.cov[i][j] * x[j];
                }
            }

            // Set the objective
            if (constraint_type == "rho") {
                model.setObjective(variance, GRB_MINIMIZE);
                model.addConstr(mean == u, "mean_constraint");
            } else {
                model.setObjective(mean, GRB_MAXIMIZE);
                model.addQConstr(variance <= u, "variance_constraint");
            }

            GRBLinExpr sumWeights = 0;
            for (int i = 0; i < data.N; ++i) {
                sumWeights += x[i];
            }
            model.addConstr(sumWeights == 1, "sumWeights_constraint");

            GRBLinExpr sumAssets = 0;
            for (int i = 0; i < data.N; ++i) {
                sumAssets += z[i];
            }
            model.addConstr(sumAssets == K, "sumAssets_constraint");

            for (int i = 0; i < data.N; ++i) {
                model.addConstr(x[i] <= delta * z[i], "x_upper" + to_string(i));
                model.addConstr(x[i] >= epsilon * z[i], "x_lower" + to_string(i));
            }

            model.set(GRB_DoubleParam_TimeLimit, 3600);
            model.set(GRB_DoubleParam_ImproveStartTime, 3000);

            model.optimize();
            auto status = model.get(GRB_IntAttr_Status);
            if (status == GRB_OPTIMAL or status == GRB_TIME_LIMIT) {
                cout << "Found a Solution" << endl;
                ss << mean.getValue() << " " << variance.getValue() << " ";
                if (model.get(GRB_IntAttr_Status) == GRB_TIME_LIMIT) {
                    ss << -1 << " ";
                }
                return true;
            } else {
                cout << "No Solution" << endl;
                return false;
            }
        } catch (GRBException& e) {
            cout << "Error code = " << e.getErrorCode() << endl;
            cout << e.getMessage() << endl;
        } catch (...) {
            cout << "Error: Unknown exception caught" << endl;
        }

        return false;
    }
    void gurobi_rho_constrained(int file_num) {
        std::filesystem::path output = std::filesystem::current_path().parent_path().parent_path().parent_path() / "output";
        cerr << output << endl;
        string file_name = "grb2_output" + to_string(file_num) + "_mean.txt";
        output = output / file_name;
        ofstream out_file(output.c_str());
        for (int assets = 2; assets <= 10; assets++) {
            bool solved_flag = false;
            for (double rho = 0.0001; rho <= 0.02; rho += 0.0005) {
                auto start = steady_clock::now();
                if (!solve(rho, assets, "rho", file_num)) {
                    if (solved_flag) {
                        ss << endl;
                        break;
                    }
                } else {
                    solved_flag = true;
                    auto end = steady_clock::now();
                    auto duration = duration_cast<milliseconds>(end - start);
                    ss << duration.count() << endl;
                }
            }
            if (out_file.is_open()) {
                out_file << "K = " << assets << endl;
                cout << "K = " << assets << endl;
                // Output the contents of the stringstream to the file
                out_file << ss.str();
                ss.str(std::string());
                // Close the file
                std::cout << "Data successfully written to output.txt" << std::endl;
            } else {
                std::cerr << "Unable to open file" << std::endl;
            }
        }
        out_file.close();
    }
    void gurobi_sig_constrained(int file_num) {
        std::filesystem::path output = std::filesystem::current_path().parent_path().parent_path().parent_path() / "output";
        cerr << output << endl;
        string file_name = "grb_output" + to_string(file_num) + "_var.txt";
        output = output / file_name;
        ofstream out_file(output.c_str());
        for (int assets = 2; assets <= 10; assets++) {
            bool solved_flag = false;
            for (double rho = 0.0001; rho <= 0.002; rho += 0.0001) {
                auto start = steady_clock::now();
                if (!solve(rho, assets, "sig", file_num)) {
                    if (solved_flag) {
                        ss << endl;
                        break;
                    }
                } else {
                    solved_flag = true;
                    auto end = steady_clock::now();
                    auto duration = duration_cast<milliseconds>(end - start);
                    ss << duration.count() << endl;
                }
            }
            if (out_file.is_open()) {
                out_file << "K = " << assets << endl;
                cout << "K = " << assets << endl;
                // Output the contents of the stringstream to the file
                out_file << ss.str();
                ss.str(std::string());
                // Close the file
                std::cout << "Data successfully written to output.txt" << std::endl;
            } else {
                std::cerr << "Unable to open file" << std::endl;
            }
        }
        out_file.close();
    }
}