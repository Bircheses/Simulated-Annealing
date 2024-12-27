#include <iostream>

#include "Headers/SimulatedAnnealing.h"
#include "Utilities/StaticFunctions.cpp"

int main() {
    int size;
    int** matrix;
    auto [f, d] = read_from_file("matrix11x11.txt");
    size = f;
    matrix = d;

    // Parameters for simulated annealing
    double initialTemp = 1000.0;
    double finalTemp = 0.01;
    double alpha = 0.95; // Cooling rate

    // Perform simulated annealing for TSP
    SimulatedAnnealing SA;
    SA.load_matrix(matrix, size);
    int bestCost = SA.simulated_annealing(initialTemp, finalTemp, alpha, 100000);

    std::cout << "Best cost found: " << bestCost << std::endl;

    delete_matrix(matrix, size);

    return 0;
}
