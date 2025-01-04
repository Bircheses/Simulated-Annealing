#include "../Headers/SimulatedAnnealing.h"
#include "../Headers/Counter.h"
#include "../Utilities/StaticFunctions.cpp"
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

void SimulatedAnnealing::swap(int *tour, int i, int j) {
    std::swap(tour[i], tour[j]);
}

void SimulatedAnnealing::insert(int *tour, int i, int j) {
    int city = tour[i];
    if(i < j) {
        for(int k = i; k < j; ++k) {
            tour[k] = tour[k+1];
        }
    } else {
        for(int k = i; k > j; --k) {
            tour[k] = tour[k-1];
        }
    }
    tour[j] = city;
}

void SimulatedAnnealing::inverse(int *tour, int i, int j) {
    if(i < j){
        while(i < j) {
            std::swap(tour[i], tour[j]);
            ++i;
            --j;
        }
    }else{
        while(i > j) {
            std::swap(tour[i], tour[j]);
            --i;
            ++j;
        }
    }
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
        int j = rand() % size;
        int k = rand() % size;
        while (j == k) k = rand() % size;

        int current_cost = calculate_cost(matrix, current_solution, size);

        //Create new tour using swap
        swap(current_solution, j, k);
        int neighbor_cost = calculate_cost(matrix, current_solution, size);

        //Recreate original tour using swap
        swap(current_solution, j, k);

        //Increase the difference for temperature compute
        total_difference += std::abs(neighbor_cost - current_cost);
    }

    delete[] current_solution;
    return  -(total_difference / sample_size) / std::log(0.8);
}

return_values SimulatedAnnealing::simulated_annealing(double finalTemp, double alpha, double stop_time, int neighbour_strategy) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);// CLOCK_MONOTONIC gwarantuje stały wzrost czasu
    srand(ts.tv_nsec ^ ts.tv_sec);

    Counter counter;
    counter.start();

    double time_found = 0.0;
    int stagnationCounter = 0;

    // Initial tour using nearest neighbour algorithm
    NearestNeighbourAlgorithm NNA;
    NNA.load_matrix(matrix, size);
    int* currentTour = NNA.find_shortest_path();

    // Initial temperature
    double currentTemp = compute_initial_temp(matrix, size);

    // Storing best tour and best cost
    int* bestTour = copy(currentTour, size);
    int bestCost = calculate_cost(matrix, bestTour, size);

    int pom = 0;

    while (currentTemp > finalTemp && counter.getElapsedTime() < stop_time) {
        int i = rand() % size;
        int j = rand() % size;
        while (i == j) j = rand() % size;

        //int* neighborTour = swap(currentTour, size);
        int currentCost = calculate_cost(matrix, currentTour, size);

        switch (neighbour_strategy) {
            case 0:
                swap(currentTour, i, j);
                break;
            case 1:
                inverse(currentTour, i, j);
                break;
            case 2:
                insert(currentTour, i, j);
                break;
            default:
                swap(currentTour, i, j);
                break;
        }

        int neighbourCost = calculate_cost(matrix, currentTour, size);

        if (neighbourCost < currentCost || (double)exp((currentCost - neighbourCost) / currentTemp) > (rand() / (double)RAND_MAX)) {
            stagnationCounter = 0;
            if(neighbourCost < bestCost) {
                delete [] bestTour;
                bestTour = copy(currentTour, size);
                bestCost = neighbourCost;
                time_found = counter.getElapsedTime();
            }
        }else {
            switch (neighbour_strategy) {
                case 0:
                    swap(currentTour, i, j);
                break;
                case 1:
                    inverse(currentTour, i, j);
                break;
                case 2:
                    insert(currentTour, j, i);
                break;
                default:
                    swap(currentTour, i, j);
                break;
            }
            stagnationCounter++;
        }

        if (stagnationCounter >= maxStagnation) {
            delete [] currentTour;
            currentTour = NNA.find_shortest_path();
            stagnationCounter = 0;
            pom++;
        }

        currentTemp *= alpha;
    }

    return_values rv = {bestCost, bestTour, time_found, currentTemp};

    delete [] currentTour;

    counter.stop();

    return rv;
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
