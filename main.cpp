#include <iostream>

#include "Headers/SimulatedAnnealing.h"
#include "Utilities/StaticFunctions.cpp"

// Function to calculate the initial temperature
double calculate_initial_temperature(int** matrix, int size, double lambda) {
    int min = INT_MAX;
    int max = INT_MIN;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i != j) {
                if(matrix[i][j] < min) min = matrix[i][j];
                if(matrix[i][j] > max) max = matrix[i][j];
            }
        }
    }

    double temperature = lambda * (max - min);
    return temperature;
}

int main() {
    int size;
    int** matrix;
    auto [f, d] = read_from_file("matrix11x11.txt");
    size = f;
    matrix = d;

    double lambda = 1.0;
    double initialTemp = calculate_initial_temperature(matrix, size, lambda);

    SimulatedAnnealing SA;
    SA.load_matrix(matrix, size);
    int BestCost = SA.simulated_annealing(initialTemp, 0.1, 0.95, 10000);

    std::cout << BestCost << std::endl;

    delete_matrix(matrix, size);

    return 0;
}
