#include <iostream>
#include <ctime>
#include <cmath>

#include "StaticFunctions.cpp"

int* copy(const int* tab2, int size) {
    int* tab1 = new int[size];
    for(int i=0; i<size; i++) tab1[i] = tab2[i];
    return tab1;
}

// Calculate the total cost of a TSP tour based on an adjacency matrix
int calculate_cost(int** matrix, int* tour, int size) {
    int totalCost = 0;
    for (int i = 0; i < size-1; ++i) {
        totalCost += matrix[tour[i]][tour[i+1]];
    }
    totalCost += matrix[tour[size-1]][tour[0]];
    return totalCost;
}

int* swap(int* currentTour, int size) {
    int* newTour = copy(currentTour, size);
    int i = rand() % size;
    int j = rand() % size;
    std::swap(newTour[i], newTour[j]);
    return newTour;
}

int* generate_random_tour(int size) {
    int* a = new int[size];

    for(int i=0; i<size; i++) {
        a[i] = i;
    }

    for(int i=0; i<size; i++) {
        int random = rand() % size;
        std::swap(a[i], a[random]);
    }

    return a;
}

// Simulated Annealing for TSP using an adjacency matrix
int simulated_annealing_TSP(int** matrix, int size, double initialTemp, double finalTemp, double alpha, int maxStagnation, int maxIterations) {
    // Initialize random seed
    srand(time(nullptr));

    // Initial solution (random permutation of cities)
    int* currentTour = generate_random_tour(size);

    int* bestTour = copy(currentTour, size);
    double currentTemp = initialTemp;
    int bestCost = calculate_cost(matrix, bestTour, size);

    int stagnationCounter = 0;
    int iterationCounter = 0;

    while (currentTemp > finalTemp && iterationCounter < maxIterations) {
        int* neighborTour = swap(currentTour, size);
        int currentCost = calculate_cost(matrix, currentTour, size);
        int neighbourCost = calculate_cost(matrix, neighborTour, size);

        // Calculate acceptance probability
        if (neighbourCost < currentCost || exp((currentCost - neighbourCost) / currentTemp) > rand() / (double)RAND_MAX) {
            delete [] currentTour;
            currentTour = copy(neighborTour, size);
        }else {
            stagnationCounter++;
        }

        int newCost = calculate_cost(matrix, currentTour, size);
        if (newCost < bestCost) {
            delete [] bestTour;
            bestTour = copy(currentTour, size);
            bestCost = newCost;
            stagnationCounter = 0;
        }

        if (stagnationCounter == maxStagnation) {
            std::cout << "Stagnation detected, reinitializing...\n";

            // Reinitialize currentTour randomly
            delete [] currentTour;
            currentTour = generate_random_tour(size);

            // Optionally reset temperature
            currentTemp = initialTemp * 0.5; // Example: Restart at half the initial temperature

            // Reset stagnation counter
            stagnationCounter = 0;
        }

        iterationCounter++;

        currentTemp *= alpha;
        delete [] neighborTour;
    }

    show_tour(bestTour, size);

    delete [] currentTour;
    delete [] bestTour;

    return bestCost;
}

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
    int bestCost = simulated_annealing_TSP(matrix, size, initialTemp, finalTemp, alpha, 3, 100000);

    /*int* tour = new int[size];
    tour[0] = 3;
    tour[1] = 0;
    tour[2] = 4;
    tour[3] = 1;
    tour[4] = 2;
    tour[5] = 5;

    int bestCost = calculate_cost(matrix, tour, size);*/

    std::cout << "Best cost found: " << bestCost << std::endl;

    delete_matrix(matrix, size);

    // delete [] tour;

    return 0;
}
