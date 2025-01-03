#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H



class SimulatedAnnealing {
private:
    const int maxStagnation = 500;
    int** matrix = nullptr;
    int size = 0;
    int* copy(const int* tour, int size);
    int calculate_cost(int** matrix, int* tour, int size);
    void swap(int *tour, int i, int j);
    int* generate_random_tour(int size);
    double compute_initial_temp(int** matrix, int size);
public:
    void load_matrix(int** matrix, int size);
    int simulated_annealing(double finalTemp, double alpha, double stop_time);
    ~SimulatedAnnealing();
};



#endif
