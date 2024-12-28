#include "NearestNeighbourAlgorithm.h"

#include <iostream>

/**
 * Deallocates memory before destroying the object
 */
NearestNeighbourAlgorithm::~NearestNeighbourAlgorithm() {
    if(matrix_copy == nullptr) return;
    for(int i=0; i<size; i++) delete [] matrix_copy[i];
    delete [] matrix_copy;
    delete [] visited;
}

/**
 * Creates copy of matrix inside NearestNeighbourAlgorithm object
 * @param matrix - matrix that we want to copy
 * @param size - size of that matrix
 */
void NearestNeighbourAlgorithm::load_matrix(int **matrix, int size) {
    if(matrix_copy!=nullptr) {
        for(int i=0; i<this->size; i++) delete [] matrix_copy[i];
        delete [] matrix_copy;
    }
    //initialize variable
    this->size = size;
    if(visited!=nullptr) delete [] visited;
    matrix_copy = new int*[size];
    for(int i=0; i<size; i++) {
        matrix_copy[i] = new int[size];
    }
    //copy values from matrix address to new address
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            matrix_copy[i][j] = matrix[i][j];
        }
    }
    visited = new bool[size];
    for(int i=0; i<size; i++) {
        visited[i] = false;
    }
}

/**
 * Nearest neighbour algorithm, that looks for minimal path cost.
 * @return pair of int and int*, first one is minimal path cost value, the latter is a array holding this path from vertex to vertex
 */
int* NearestNeighbourAlgorithm::find_shortest_path() {
    srand(time(NULL));

    int* tour = new int[size];
    int starting_vertex = rand() % size;

    for(int i=0; i<size; i++) {
        visited[i] = false;
    }

    int point = starting_vertex, temp = 0;

    for(int i=0; i<size; i++) {
        visited[point] = true;
        int min = INT_MAX;
        for(int j=0; j<size; j++) {
            if(matrix_copy[point][j] == -1 || visited[j] == true) continue;
            if(matrix_copy[point][j]<min) {
                min = matrix_copy[point][j];
                temp = j;
                tour[i] = point;
            }
        }
        point = temp;
    }

    tour[size-1] = point;
    return tour;
}
