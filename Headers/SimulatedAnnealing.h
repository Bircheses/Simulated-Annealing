#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H



class SimulatedAnnealing {
private:
    const int maxStagnation = 11;
    int** matrix = nullptr;
    int size = 0;
    int* copy(const int* tour, int size);
    int calculate_cost(int** matrix, int* tour, int size);
    int* swap(int* currentTour, int size);
    int* generate_random_tour(int size);
public:
    void load_matrix(int** matrix, int size);
    int simulated_annealing(double initialTemp, double finalTemp, double alpha, double stop_time);
    ~SimulatedAnnealing();
};



#endif
