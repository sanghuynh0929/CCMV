//
// Created by cuong on 7/3/24.
//

#include "../include/Data.h"

Data::Data() {

}

void Data::read_data(const char *path) {
//    std::cerr << path << std::endl;
    freopen(path, "r", stdin);
    std::cin >> N >> T;
    index.resize(T + 1);
    for (int t = 0; t <= T; t++) {
        std::cin >> index[t];
    }
    stock_prices.resize(N);
    for (int i = 0; i < N; i++) {
        stock_prices[i].resize(T + 1);
        for (int t = 0; t <= T; t++) {
            std::cin >> stock_prices[i][t];
        }
    }
    stock_returns.resize(N);

    for (int i = 0; i < N; i++) {
        stock_returns[i].resize(T);
        for (int t = 0; t < T; t++) {
            stock_returns[i][t] = (stock_prices[i][t+1] - stock_prices[i][t]) / stock_prices[i][t];
        }
    }

    mean.resize(N);
    for (int i = 0; i < N; i++) {
        double sum = 0;
        for (int t = 0; t < T; t++) {
            sum += stock_returns[i][t];
        }
        mean[i] = sum / T;
    }

    cov.resize(N);
    for (int i = 0; i < N; i++) {
        cov[i].resize(N);
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int t = 0; t < T; t++) {
                sum += (stock_returns[i][t] - mean[i]) * (stock_returns[j][t] - mean[j]);
            }
            double cv = sum / (T - 1);
            cov[i][j] = cv;
        }
    }
//    std::cerr << "Read successfully data of size " << N << std::endl;
}