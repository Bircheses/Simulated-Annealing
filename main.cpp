#include <iostream>
#include <ctime>
#include <cmath>

#include "StaticFunctions.cpp"

// Calculate the total cost of a TSP tour based on an adjacency matrix
int calculate_cost(int** matrix, int* tour, int size) {
    int totalCost = 0;
    for (int i = 0; i < size-1; ++i) {
        totalCost += matrix[tour[i]][tour[i+1]];
    }
    totalCost += matrix[tour[size]][tour[0]];
    return totalCost;
}

// Generate a random neighbor by swapping two cities in the tour
int* random_neighbor(int* currentTour) {

    //Poprawić to
    int currentTourSize = sizeof(currentTour)/sizeof(int);
    int* newTour = copy(currentTour, currentTourSize);
    int i = rand() % currentTourSize;
    int j = rand() % currentTourSize;
    std::swap(newTour[i], newTour[j]);
    return newTour;
}

int* generate_random_tour(int size) {
    int* a = new int[size+1];

    for(int i=0; i<size; i++) {
        a[i] = i;
    }

    for(int i=0; i<size; i++) {
        int random = rand() % size;
        std::swap(a[i], a[random]);
    }

    a[size] = a[0];

    return a;
}

// Simulated Annealing for TSP using an adjacency matrix
int simulated_annealing_TSP(int** matrix, int size, double initialTemp, double finalTemp, double alpha) {
    // Initialize random seed
    srand(time(nullptr));

    // Initial solution (random permutation of cities)
    int* currentTour = generate_random_tour(size);

    int* bestTour = copy(currentTour, size);
    double currentTemp = initialTemp;
    int bestCost = calculate_cost(matrix, bestTour, size);

    while (currentTemp > finalTemp) {
        int* neighborTour = random_neighbor(currentTour);
        int currentCost = calculate_cost(matrix, currentTour, size);
        int neighbourCost = calculate_cost(matrix, neighborTour, size);

        // Calculate acceptance probability
        if (neighbourCost < currentCost || exp((currentCost - neighbourCost) / currentTemp) > rand() / (double)RAND_MAX) {
            delete [] currentTour;
            currentTour = copy(neighborTour, size);
        }

        // Update the best solution found so far
        if (const int newCost = calculate_cost(matrix, currentTour, size) < bestCost) {
            delete [] bestTour;
            bestTour = copy(currentTour, size);
            bestCost = newCost;
        }

        // Cool down the temperature
        currentTemp *= alpha;
        delete [] neighborTour;
    }

    // Print the best tour
    std::cout << "Best tour found: ";
    for (int i=0; i<size+1; i++) {
        std::cout << bestTour[i] << "->";
    }
    std::cout << std::endl;

    delete [] currentTour;
    delete [] bestTour;

    return bestCost;
}

int main() {
    int size;
    // Define the adjacency matrix representing the graph
    int** matrix;
    auto [f, d] = read_from_file("matrix6x6.txt");
    size = f;
    matrix = d;

    // Parameters for simulated annealing
    double initialTemp = 1000.0;
    double finalTemp = 0.01;
    double alpha = 0.95; // Cooling rate

    // Perform simulated annealing for TSP
    int bestCost = simulated_annealing_TSP(matrix, size, initialTemp, finalTemp, alpha);

    std::cout << "Best cost found: " << bestCost << std::endl;

    delete_matrix(matrix, 4);
    return 0;
}
