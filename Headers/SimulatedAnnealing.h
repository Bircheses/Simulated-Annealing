#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H



class SimulatedAnnealing {
private:
    const int maxStagnation = 1000;
    int** matrix = nullptr;
    int size = 0;
    int* copy(const int* tour, int size);
    int calculate_cost(int** matrix, int* tour, int size);
    static void swap(int *tour, int i, int j);
    static void insert(int *tour, int i, int j);
    static void inverse(int *tour, int i, int j);
    int* generate_random_tour(int size);
    double compute_initial_temp(int** matrix, int size);
public:
    void load_matrix(int** matrix, int size);
    /**
     * Function simulated_annealing implements simulated annealing algorithm
     * @param finalTemp - final temperature of algorithm
     * @param alpha - value between (0-1), which determines how fast the temperature should be cooled
     * @param stop_time - time in seconds, which determines when the algorithm will stop
     * @param neighbour_strategy - 0 for swap, 1 for inverse, 2 for insert, which determines the strategy of picking neighbour tour
     * @return cost of best tour that algorithm has found
     */
    int simulated_annealing(double finalTemp, double alpha, double stop_time, int neighbour_strategy);
    ~SimulatedAnnealing();
};



#endif
