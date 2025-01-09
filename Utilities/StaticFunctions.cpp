#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>
#include <thread>

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

static void write_csv_file(double alpha, int strategy, return_values rv, int size) {
    ofstream file("results.csv", std::ios::app);
    string strat = "";

    if(strategy == 0) strat = "swap";
    else if(strategy == 1) strat = "inverse";
    else if(strategy == 2) strat = "insert";

    file << rv.cost << ";" << rv.final_temp << ";" << rv.time << ";" << strat << ";" << alpha << ";";

    for(int i=0; i<size; i++) {
        file << rv.tour[i] << " ";
    }

    file << endl;
}

static void start_thread(int** matrix, int size, int neighbour_strategy, double alpha, double stop_time, double min_temp) {
    return_values rv;

    SimulatedAnnealing SA;
    SA.load_matrix(matrix, size);

    rv = SA.simulated_annealing(min_temp, alpha, stop_time, neighbour_strategy);

    write_csv_file(alpha, neighbour_strategy, rv, size);
    delete[] rv.tour;
}

static void read_conf_file () {
    int size;
    int** matrix;
    double stop_time;
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

            if(argument == 0) {
                SimulatedAnnealing SA;
                SA.load_matrix(matrix, size);
                return_values rv = SA.simulated_annealing(std::numeric_limits<double>::denorm_min(), alpha, stop_time, 0);
                show_tour(rv.tour, size);
                std::cout << "Koszt: " << rv.cost << std::endl;
                std::cout << "Czas zakonczenia: " << rv.time <<  std::endl;
            }else if(argument == 1) {
                SimulatedAnnealing SA;
                SA.load_matrix(matrix, size);
                return_values rv = SA.simulated_annealing(std::numeric_limits<double>::denorm_min(), alpha, stop_time, 1);
                show_tour(rv.tour, size);
                std::cout << "Koszt: " << rv.cost << std::endl;
                std::cout << "Czas zakonczenia: " << rv.time <<  std::endl;
            }else if(argument == 2) {
                SimulatedAnnealing SA;
                SA.load_matrix(matrix, size);
                return_values rv = SA.simulated_annealing(std::numeric_limits<double>::denorm_min(), alpha, stop_time, 2);
                show_tour(rv.tour, size);
                std::cout << "Koszt: " << rv.cost << std::endl;
                std::cout << "Czas zakonczenia: " << rv.time <<  std::endl;
            }
        }

        if (!line.find("Simulate")) {
            int algorithm = 1;
            double stop_times[3] = {60000, 120000, 240000};
            double alphas[3] = {0.95, 0.975, 0.995};
            double min_temp = std::numeric_limits<double>::denorm_min();

            int** matrix;
            int size;

            if(algorithm == 0) {
                auto [f1, s1] = read_atsp("ftv47.atsp");
                matrix = s1;
                size = f1;
            }else if(algorithm == 1) {
                auto [f1, s1] = read_atsp("ftv170.atsp");
                matrix = s1;
                size = f1;
            }else if(algorithm == 2) {
                auto [f1, s1] = read_atsp("rbg403.atsp");
                matrix = s1;
                size = f1;
            }

            for(int k=1; k<2; k++) {
                for(int i=0; i<3; i++) {
                    for(int j=0; j<2; j++) {
                        thread t1(start_thread, matrix, size, k, alphas[i], stop_times[algorithm], min_temp);

                        this_thread::sleep_for(std::chrono::milliseconds(200));

                        thread t2(start_thread, matrix, size, k, alphas[i], stop_times[algorithm], min_temp);

                        this_thread::sleep_for(std::chrono::milliseconds(200));

                        thread t3(start_thread, matrix, size, k, alphas[i], stop_times[algorithm], min_temp);

                        this_thread::sleep_for(std::chrono::milliseconds(200));

                        thread t4(start_thread, matrix, size, k, alphas[i], stop_times[algorithm], min_temp);

                        this_thread::sleep_for(std::chrono::milliseconds(200));

                        thread t5(start_thread, matrix, size, k, alphas[i], stop_times[algorithm], min_temp);

                        t1.join();
                        t2.join();
                        t3.join();
                        t4.join();
                        t5.join();
                    }
                }
            }
        }

    }
    delete_matrix(matrix, size);
    file.close();
}