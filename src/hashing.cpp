#include "BitmaskBoard.hpp"
#include "hashing.hpp"

std::ostream& operator<<(std::ostream& os, const TTValue& value) {
    os << value.depth << " " << value.eval;
    return os;
}

std::istream& operator>>(std::istream& is, TTValue& value) {
    is >> value.depth >> value.eval;
    return is;
}

void writeGameHistoryToFile(const robin_hood::unordered_map<uint64_t, int>& gameHistory, const std::string& filename) {
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

void readGameHistoryFromFile(robin_hood::unordered_map<uint64_t, int>& gameHistory, const std::string& filename) {
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

std::ostream& operator<<(std::ostream& os, const BitmaskBoard& board) {
    // Serialize BitmaskBoard to stream
    os << board.whitePawns << " "
       << board.blackPawns << " "
       << board.whiteKings << " "
       << board.blackKings << " "
       << board.isWhiteTurn << " ";
    return os;
}

std::istream& operator>>(std::istream& is, BitmaskBoard& board) {
    // Deserialize BitmaskBoard from stream
    is >> board.whitePawns
       >> board.blackPawns
       >> board.whiteKings
       >> board.blackKings
       >> board.isWhiteTurn;
    return is;
}

// Function to read transposition table from file
void readTranspositionTableFromFile(robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file for reading: " << filename << std::endl;
        return;
    }

    BitmaskBoard board;
    TTValue value;
    while (inFile >> board >> value) {
        transpositionTable[board] = value;
    }
    inFile.close();
}

// Function to write transposition table to file
void writeTranspositionTableToFile(robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& [board, value] : transpositionTable) {
        outFile << board << value << '\n';
    }
    outFile.close();
}
