#pragma once

#include "BitmaskBoard.hpp"


typedef struct TTValue {
    int depth;
    int eval;
} TTValue;

class TranspositionTableValue {
    int eval;
    int depth;
    BitmaskBoard bestMove;

public:
     // Default constructor
    TranspositionTableValue() : eval(0), depth(0), bestMove() {}

    // Constructor with initializer list
    TranspositionTableValue(int e, int d, const BitmaskBoard& b) : eval(e), depth(d), bestMove(b) {}

    //copy constructor
    TranspositionTableValue(const TranspositionTableValue& other) : eval(other.eval), depth(other.depth), bestMove(other.bestMove) {}

    // Getters
    int getEval() const { return eval; }
    int getDepth() const { return depth; }
    BitmaskBoard& getBestMove() { return bestMove; }

    // If setters are needed
    void setEval(int e) { eval = e; }
    void setDepth(int d) { depth = d; }
    void setBestMove(const BitmaskBoard& b) { bestMove = b; }
};

//hashing stuff
std::mt19937_64 rng(std::random_device{ }());
std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

std::unordered_map<uint64_t, std::pair<uint64_t, int>> transpositionTable;

std::unordered_map<BitmaskBoard, TTValue> transpositionTable3;

std::unordered_map<BitmaskBoard, TranspositionTableValue> transpositionTable2;

void initializeLookupTable() {
    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (int square = 0; square < NUM_SQUARES; ++square) {
            for (int turn = 0; turn < NUM_TURNS; ++turn) {
                lookupTable[piece][square][turn] = distribution(rng);
            }
        }
    }
}

// Function to initialize Zobrist table with random values
void initializeZobristTable() {
    std::mt19937_64 rng(std::random_device{ }());
    for (int pieceType = 0; pieceType < 4; ++pieceType) {
        for (int position = 0; position < 64; ++position) {
            zobristTable[pieceType][position] = rng();
        }
    }
}

uint64_t calculateHashKey(BitmaskBoard& boardLayout, char turn) {
    return boardLayout.calculateHashKey(turn, zobristTable);
}

void writeTranspositionTableToFile(const std::unordered_map<uint64_t, std::pair<uint64_t, int>>& transpositionTable, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& entry : transpositionTable) {
        outFile << entry.first << " " << entry.second.first << " " << entry.second.second << "\n";
    }

    outFile.close();
}

// Function to load the transposition table from a file
void loadTranspositionTableFromFile(std::unordered_map<uint64_t, std::pair<uint64_t, int>>& transpositionTable, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    uint64_t key;
    uint64_t valueFirst;
    int valueSecond;

    while (inFile >> key >> valueFirst >> valueSecond) {
        transpositionTable[key] = {valueFirst, valueSecond};
    }

    inFile.close();
}

void writeGameHistoryToFile(const std::unordered_map<uint64_t, int>& gameHistory, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& [key, value] : gameHistory) {
        if(value != 0)
            outFile << key << " " << value << "\n";
    }

    outFile.close();
}

void readGameHistoryFromFile(std::unordered_map<uint64_t, int>& gameHistory, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    uint64_t key;
    int value;
    while (inFile >> key >> value) {
        gameHistory[key] = value;
    }

    inFile.close();
}
