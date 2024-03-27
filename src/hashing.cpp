#include "BitmaskBoard.hpp"

typedef struct TTValue {
    int depth;
    int eval;
} TTValue;

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
