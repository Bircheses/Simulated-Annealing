#include "../Headers/SimulatedAnnealing.h"
#include "../Utilities/StaticFunctions.cpp"
#include "../Headers/Counter.h"
#include <ctime>
#include <cmath>

#include "NearestNeighbourAlgorithm.h"

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

double SimulatedAnnealing::compute_initial_temp(int **matrix, int size) {
    double total_difference = 0.0;
    int* current_solution = generate_random_tour(size);
    int sample_size = size*size;

    for (int i = 0; i < sample_size; ++i) {
        int* neighbor_solution = swap(current_solution, size);
        int current_cost = calculate_cost(matrix, current_solution, size);
        int neighbor_cost = calculate_cost(matrix, neighbor_solution, size);
        total_difference += std::abs(neighbor_cost - current_cost);
        delete[] neighbor_solution;
    }

    delete[] current_solution;
    return  -(total_difference / sample_size) / std::log(0.8);
}

int SimulatedAnnealing::simulated_annealing(double finalTemp, double alpha, double stop_time) {
    srand(time(nullptr));

    Counter counter;
    counter.start();

    double time_found = 0.0;

    // Initial solution using nearest neighbour algorithm
    NearestNeighbourAlgorithm NNA;
    NNA.load_matrix(matrix, size);
    int* currentTour = NNA.find_shortest_path();

    int* bestTour = copy(currentTour, size);
    double currentTemp = compute_initial_temp(matrix, size);
    int bestCost = calculate_cost(matrix, bestTour, size);

    show_tour(currentTour, size);
    std::cout << bestCost << std::endl;
    std::cout << currentTemp << std::endl;

    while (currentTemp > finalTemp /*&& counter.getElapsedTime() < stop_time*/) {
        int* neighborTour = swap(currentTour, size);
        int currentCost = calculate_cost(matrix, currentTour, size);
        int neighbourCost = calculate_cost(matrix, neighborTour, size);

        if (neighbourCost < currentCost || exp((currentCost - neighbourCost) / currentTemp) > (rand() / (double)RAND_MAX)) {
            delete [] currentTour;
            currentTour = copy(neighborTour, size);
        }

        int newCost = calculate_cost(matrix, currentTour, size);
        if (newCost < bestCost) {
            delete [] bestTour;
            bestTour = copy(currentTour, size);
            bestCost = newCost;
            time_found = counter.getElapsedTime();
        }

        currentTemp *= alpha;
        delete [] neighborTour;
    }

    show_tour(bestTour, size);
    cout << "Found in " << time_found << " ms." << endl;

    delete [] currentTour;
    delete [] bestTour;

    counter.stop();

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
