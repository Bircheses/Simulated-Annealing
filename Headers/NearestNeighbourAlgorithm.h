#ifndef NEARESTNEIGHBOURALGORITHM_H
#define NEARESTNEIGHBOURALGORITHM_H

#include <algorithm>

class NearestNeighbourAlgorithm {
private:
    int** matrix_copy = nullptr;
    int size = 0;
    bool* visited = nullptr;
public:
    ~NearestNeighbourAlgorithm();
    int* find_shortest_path();
    void load_matrix(int** matrix, int size);
};



#endif
