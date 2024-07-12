//
// Created by cuong on 6/28/24.
//

#include "../include/CPLEXSolver.h"
#include <ilcplex/ilocplex.h>
#include <chrono>
#include "../include/Data.h"

using namespace std;
using namespace chrono;


namespace cplex_solver {
    stringstream ss;
    Data data = Data();
    bool solve(double u, int K, const string &constraint_type, int file_num) {
        string file_name = "indtrack" + to_string(file_num) + ".txt";
        std::filesystem::path current_work = std::filesystem::current_path().parent_path().parent_path().parent_path();
        std::filesystem::path data_path = current_work / "data" / file_name;
        data.read_data(data_path.c_str());
        IloEnv env;
        try {
            IloModel model(env);
            IloCplex cplex(model);

            // Variables
            IloNumVarArray x(env, data.N, 0, 1, ILOFLOAT);
            IloIntVarArray z(env, data.N, 0, 1);

            double epsilon = 0;
            double delta = 1;

            IloExpr obj(env);
            IloExpr variance(env);
            IloExpr mean(env);

            for (int i = 0; i < data.N; i++) {
                mean += x[i] * data.mean[i];
            }

            for (int i = 0; i < data.N; i++) {
                for (int j = 0; j < data.N; j++) {
                    variance += x[i] * data.cov[i][j] * x[j];
                } cerr << endl;
            }

            // Set the objective

            if (constraint_type == "rho") {
                obj = variance;
                model.add(IloMinimize(env, obj));
                model.add(mean == u);
            } else {
                obj = mean;
                model.add(IloMaximize(env, obj));
                model.add(variance <= u);
            }
            IloExpr sumWeights(env);
            for (int i = 0; i < data.N; i++) {
                sumWeights += x[i];
            }
            model.add(sumWeights == 1);

            IloExpr sumAssets(env);
            for (int i = 0; i < data.N; i++) {
                sumAssets += z[i];
            }
            model.add(sumAssets == K);

            for (int i = 0; i < data.N; i++) {
                model.add(x[i] <= delta * z[i]);
                model.add(x[i] >= epsilon * z[i]);
            }

            cplex.extract(model);
            cplex.setParam(IloCplex::Param::OptimalityTarget, 3);
            cplex.setParam(IloCplex::Param::TimeLimit, 3600);
            cplex.setParam(IloCplex::Param::MIP::PolishAfter::Time, 3000);

            if (cplex.solve()) {
                cout << "Found a Solution" << endl;
                ss << cplex.getValue(mean) << " " << cplex.getValue(variance) << " ";
                if (cplex.getStatus() != IloAlgorithm::Optimal) {
                    ss << -1 << " ";
                }
                return true;
            } else {
                cout << "No Solution" << endl;
                return false;
            }
        } catch (IloException& ex) {
            cout << "Error: " << ex << std::endl;
        } catch (...) {
            cout << "Error: Unknown exception caught" << std::endl;
        }
        env.end();
        return false;
    }

    void cplex_rho_constrained(int file_num) {
        std::filesystem::path output = std::filesystem::current_path().parent_path().parent_path().parent_path() / "output";
        cerr << output << endl;
        string file_name = "output" + to_string(file_num) + "_mean.txt";
        output = output / file_name;
        ofstream out_file(output.c_str());
        for (int assets = 2; assets <= 10; assets++) {
            bool solved_flag = false;
            for (double rho = 0.0001; rho <= 0.02; rho += 0.0001) {
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

    void cplex_sig_constrained(int file_num) {
        std::filesystem::path output = std::filesystem::current_path().parent_path() / "output";
        string file_name = "output" + to_string(file_num) + "_var.txt";
        output = output / file_name;

        ofstream out_file(output.c_str(), std::ios_base::app);


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
