#pragma once

#include "BitmaskBoard.hpp"

//hashing stuff
std::mt19937_64 rng(std::random_device{ }());
std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());

std::unordered_map<uint64_t, std::pair<uint64_t, int>> transpositionTable;

std::unordered_map<BitmaskBoard, std::pair<int, int>> transpositionTable2;

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