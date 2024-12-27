#include "../Headers/SimulatedAnnealing.h"
#include "../Utilities/StaticFunctions.cpp"
#include <ctime>
#include <cmath>

int * SimulatedAnnealing::copy(const int *tour, int size) {
    int* temp = new int[size];
    for(int i=0; i<size; i++) temp[i] = tour[i];
    return temp;
}

int SimulatedAnnealing::calculate_cost(int **matrix, int *tour, int size) {
    int totalCost = 0;
    for (int i = 0; i < size-1; ++i) {
        totalCost += matrix[tour[i]][tour[i+1]];
    }
    totalCost += matrix[tour[size-1]][tour[0]];
    return totalCost;
}

int * SimulatedAnnealing::swap(int *currentTour, int size) {
    int* newTour = copy(currentTour, size);
    int i = rand() % size;
    int j = rand() % size;
    std::swap(newTour[i], newTour[j]);
    return newTour;
}

int * SimulatedAnnealing::generate_random_tour(int size) {
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

int SimulatedAnnealing::simulated_annealing(double initialTemp, double finalTemp, double alpha,
    int maxIterations) {
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
            delete [] currentTour;
            // Making new random tour to unstack algorithm
            currentTour = generate_random_tour(size);
            // Making more room for algorithm to traverse
            currentTemp = initialTemp * 0.5;
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

void SimulatedAnnealing::load_matrix(int **matrix, int size) {
    if(this->matrix!=nullptr) {
        for(int i=0; i<this->size; i++) delete [] this->matrix[i];
        delete [] this->matrix;
    }
    this->size = size;

    //initialize matrix
    this->matrix = new int*[size];
    for(int i=0; i<size; i++) {
        this->matrix[i] = new int[size];
    }

    //copy values from old address to new address
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            this->matrix[i][j] = matrix[i][j];
        }
    }
}

SimulatedAnnealing::~SimulatedAnnealing() {
    if(this->matrix == nullptr) return;
    for(int i=0; i<size; i++) delete [] this->matrix[i];
    delete [] this->matrix;
}
