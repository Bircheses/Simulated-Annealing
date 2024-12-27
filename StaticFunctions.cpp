#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>

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

/*static void read_conf_file () {
    int size;
    int** matrix;
    int* tab;
    int tabsize;

    int mode = 0;

    std::ifstream file("config.txt");
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku konfiguracyjnego" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find("//") == 0) {
            continue;
        }

        if (line.front() == '0') {
            mode = 0;
            istringstream iss(line);
            int command = -1;
            string argument = "";
            iss >> command >> argument;

            auto [fst, scd] = read_from_file(argument);
            size = fst;
            matrix = scd;

        }else if(line.front() == '1') {
            mode = 0;

            istringstream iss(line);
            int command = -1;
            int argument = -1;
            iss >> command >> argument;

            size = argument;
            matrix = generate_matrix_asymetric(argument);

        }else if(line.front() == '2') {

            mode = 1;

            istringstream iss(line);
            int command = -1;
            int argument = -1;
            iss >> command >> argument;

            size = argument;
            auto [f, s] = generate_matrix_symetric(argument);
            tabsize = f;
            tab = s;

        }else if(line.front() == '3') {

            if(mode == 0) display_matrix(matrix, size);
            else display_tab(tab, tabsize);

        }else if(line.front() == '5') {

            istringstream iss(line);
            int command = -1;
            int argument = -1;
            iss >> command >> argument;
            if(argument>=size) {
                cerr << "Zly wierzzcholek startowy" << endl;
                break;
            }
            if(mode == 0) {
                bnb.load_matrix(matrix, size);
                auto [f, s] = bnb.BestFirstAlg(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }else {
                bnb_symetric.load_tab_symetric(tab, tabsize, size);
                auto [f, s] = bnb_symetric.BestFirstAlg(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }

        }else if(line.front() == '6') {

            istringstream iss(line);
            int command = -1;
            int argument = -1;
            iss >> command >> argument;
            if(argument>=size) {
                cerr << "Zly wierzzcholek startowy" << endl;
                break;
            }
            if(mode == 0) {
                bnb.load_matrix(matrix, size);
                auto [f, s] = bnb.BreadthFirstAlg(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }else {
                bnb_symetric.load_tab_symetric(tab, tabsize, size);
                auto [f, s] = bnb_symetric.BreadthFirstAlg(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }

        }else if(line.front() == '7') {
            istringstream iss(line);
            int command = -1;
            int argument = -1;
            iss >> command >> argument;
            if(argument>=size) {
                cerr << "Zly wierzzcholek startowy" << endl;
                break;
            }
            if(mode == 0) {
                bnb.load_matrix(matrix, size);
                auto [f, s] = bnb.DepthFirstAlg(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }else {
                bnb_symetric.load_tab_symetric(tab, tabsize, size);
                auto [f, s] = bnb_symetric.DepthFirstAlg(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }
        }else if(line.front() == '8') {
            istringstream iss(line);
            int command = -1;
            int argument = -1;
            iss >> command >> argument;
            if(argument>=size) {
                cerr << "Zly wierzzcholek startowy" << endl;
                break;
            }
            if(mode == 0) {
                bnb.load_matrix(matrix, size);
                auto [f, s] = bnb.BetterBFS(argument);
                cout << f << endl;
                display_path(s, size);
                delete [] s;
            }
        }else if(line.front() == '9') {
            simulate();
        }else if(line.front() == 'A') {
            simulate_symetric();
        }
    }
    if(mode == 0) delete_matrix(matrix, size);
    else delete [] tab;
    file.close();
}*/