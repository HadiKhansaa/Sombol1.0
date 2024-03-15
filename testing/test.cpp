#include <iostream>
#include <tuple>
#include <fstream>
#include <cstring>
#include <random>
#include <time.h> 
#include <vector>
#include <set>
#include <memory>
#include <cstdlib>   // for rand() and srand()
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

class Coalesced2DArray {
public:
    Coalesced2DArray(int rows = 8, int cols = 8) : _rows(rows), _cols(cols), _data(new char[rows * cols]) {}

    // Copy constructor for deep copying
    Coalesced2DArray(const Coalesced2DArray& other) : _rows(other._rows), _cols(other._cols), _data(new char[other._rows * other._cols]) {
        std::memcpy(_data, other._data, _rows * _cols * sizeof(char));
    }

    // Destructor to free allocated memory
    ~Coalesced2DArray() {
        delete[] _data;
    }

    // Assignment operator for deep copying
    Coalesced2DArray& operator=(const Coalesced2DArray& other) {
        if (this != &other) { // Protect against self-assignment/
            delete[] _data; // Free existing resource
            _rows = other._rows;
            _cols = other._cols;
            _data = new char[_rows * _cols];
            std::memcpy(_data, other._data, _rows * _cols * sizeof(char));
        }
        return *this;
    }

        // Non-const version of Row proxy
    class Row {
    public:
        Row(char* row, int cols) : _row(row), _cols(cols) {}
        char& operator[](int col) {
            return _row[col];
        }
    private:
        char* _row;
        int _cols;
    };

    // Const version of Row proxy
    class ConstRow {
    public:
        ConstRow(const char* row, int cols) : _row(row), _cols(cols) {}
        const char& operator[](int col) const {
            return _row[col];
        }
    private:
        const char* _row;
        int _cols;
    };

    // Non-const version of operator[]
    Row operator[](int row) {
        return Row(_data + (row * _cols), _cols);
    }

    // Const version of operator[]
    ConstRow operator[](int row) const {
        return ConstRow(_data + (row * _cols), _cols);
    }

private:
    int _rows, _cols;
    char* _data; // The underlying storage
};

int main()
{
    // int n;
    // cin>>n;

    // // vector<Coalesced2DArray> all_moves(n);
    
    // // Coalesced2DArray arr(8, 8);

    // int arr[8][8];


    // for(int i=0; i<8; i++)
    //     for(int j=0; j<8; j++)
    //         arr[i][j] = 0;
    
    // // Coalesced2DArray arr2 = arr;

    // // arr2[0][0] = 1;

    // printf("%c ", (char)arr[0][0]);



    // clock_t begin = clock();
    
    // for(int i=0; i<n; i++)
    //     arr[0][0] = rand()%4;
    
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // std::cout<<"Time: "<<time_spent<<endl;


    std::mt19937_64 rng(42); // Fixed seed ensures repeatable results
    std::uniform_int_distribution<uint64_t> dist;

    std::cout << "std::array<std::array<uint64_t, 64>, 4> zobristTable = {{\n";
    for (int piece = 0; piece < 4; ++piece) {
        std::cout << "    {\n";
        for (int square = 0; square < 64; ++square) {
            uint64_t randomValue = dist(rng);
            std::cout << "        0x" << std::hex << randomValue << "ULL";
            if (square < 63) std::cout << ",";
            std::cout << "\n";
        }
        std::cout << "    }";
        if (piece < 3) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "}};\n";

    return 0;



}