#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>

#include "SimulatedAnnealing.h"

using namespace std;

static int** generate_matrix_asymetric(int size){
    srand(time(NULL));
    //inicjalizacja macierzy
    int** matrix = new int*[size];
    for(int i=0; i<size; i++){
        matrix[i] = new int[size];
    }
    //uzupełnienie macierzy losowymi wartościami krawędzi
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++) {
            if(i==j) matrix[i][j] = -1;
            else matrix[i][j] = rand()%100+1;
        }
    }
    return matrix;
}

static std::pair<int, int*> generate_matrix_symetric(int size){
    srand(time(NULL));

    //inicjalizacja tablicy
    int size_temp = size*(size-1)/2;
    int* tab = new int[size_temp];

    for(int i=0; i<size_temp; i++) {
        tab[i] = rand()%100+1;
    }

    return std::make_pair(size_temp, tab);
}

static void show_matrix(int** matrix, int size){
    cout << "Macierz: " << endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            cout << setw(3) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

static void show_tab(int* tab, int tabsize){
    cout << "Tablica: " << endl;
    for (int i=0; i<tabsize; i++) {
        cout << tab[i] << " ";
    }
    cout << endl;
}

static void show_tour (int* tour, int size) {
    for(int i=0; i<size; i++) {
        cout << tour[i] << "->";
    }
    cout << tour[0] << endl;
}

static void delete_matrix(int** matrix, int size){
    if(matrix == nullptr) return;
    for(int i=0; i<size; i++){
        delete [] matrix[i];
    }
    delete [] matrix;
}

// Function to read ATSP (Asymmetric TSP) file and create a distance matrix
static pair<int, int**> read_atsp(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return std::make_pair(-1, nullptr);
    }

    string line, key;
    int** matrix = nullptr;
    bool is_edge_weight_section = false;
    int size = 0;

    int i=0, j=0;

    while (getline(file, line)) {
        stringstream ss(line);

        ss >> key;

        if (key == "DIMENSION:") {
            ss >> size;
            matrix = new int*[size];
            for (int i = 0; i < size; i++) {
                matrix[i] = new int[size];
            }
        } else if (key == "EDGE_WEIGHT_SECTION") {
            is_edge_weight_section = true;
            continue;
        } else if (key == "EOF") {
            break;
        }

        if (is_edge_weight_section) {
            istringstream stream(line);
            int value;
            while(stream >> value) {
                if(i==size) {
                    i=0;
                    j++;
                }
                matrix[j][i] = value;
                i++;
            }
        }
    }

    file.close();
    return std::make_pair(size, matrix);
}

static int silnia(int size){
    if(size == 2) return 2;
    return size*silnia(size-1);
}

/**
 * Reading data (matrix) from file
 * @param path - path to destination file
 * @return address of matrix
 */
static pair<int, int**> read_from_file (string path) {
    int** matrix = nullptr;
    int temp, i=0, j=0, k=0, amount=0;
    ifstream file(path);
    if(!file.is_open()) return std::make_pair(-1,nullptr);;
    while (file >> temp) {
        if(i == 0) {
            amount = temp;
            matrix = new int*[amount];
            for(int i=0; i<amount; i++){
                matrix[i] = new int[amount];
            }
        }else if(k<amount){
            matrix[j][k] = temp;
            k++;
        }
        if(k==amount) {
            k=0;
            j++;
        }
        i=1;
    }
    return std::make_pair(amount, matrix);
}

static void read_conf_file () {
    int size;
    int** matrix;
    double stop_time;
    double final_temp = 0.000001;
    double alpha;

    std::ifstream file("config.txt");
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku konfiguracyjnego" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find('#') == 0) {
            continue;
        }

        if (!line.find("nazwa_pliku:")) {
            istringstream iss(line);
            string command = "";
            string argument = "";
            iss >> command >> argument;

            auto [fst, scd] = read_atsp(argument);
            size = fst;
            matrix = scd;
        }

        if (!line.find("czas_stopu:")) {
            istringstream iss(line);
            string command = "";
            double argument;
            iss >> command >> argument;

            stop_time = argument * 1000; // Converting to milliseconds
        }

        if (!line.find("wsp_zmiany:")) {
            istringstream iss(line);
            string command = "";
            double argument;
            iss >> command >> argument;

            alpha = argument; // Change factor
        }

        if (!line.find("SA:")) {
            istringstream iss(line);
            string command = "";
            int argument;
            iss >> command >> argument;

            if(argument == 1) {
                SimulatedAnnealing SA;
                SA.load_matrix(matrix, size);
                int bestCost = SA.simulated_annealing(final_temp, alpha, stop_time);
                std::cout << bestCost << std::endl;
            }
        }

    }
    delete_matrix(matrix, size);
    file.close();
}