#pragma once
#include <iostream>
#include <tuple>
#include <fstream>
#include <sstream>
#include <cstring>
#include <random>
#include <time.h> 
#include <vector>
#include <memory>
#include <windows.h>
#include <cstdlib>   // for rand() and srand()
#include <ctime>
#include <fstream>
#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>   
// #include <unordered_map>
#include <functional>
#include "BitmaskBoard.hpp"

typedef struct TTValue {
    int depth;
    int eval;
} TTValue;

void writeGameHistoryToFile(const std::unordered_map<uint64_t, int>& gameHistory, const std::string& filename);

void readGameHistoryFromFile(std::unordered_map<uint64_t, int>& gameHistory, const std::string& filename);

void writeTranspositionTableToFile(std::unordered_map<BitmaskBoard, TTValue>& transpositionTable, const std::string& filename);

void readTranspositionTableFromFile(std::unordered_map<BitmaskBoard, TTValue>& transpositionTable, const std::string& filename);

std::ostream& operator<<(std::ostream& os, const TTValue& value);

std::istream& operator>>(std::istream& is, TTValue& value);

std::ostream& operator<<(std::ostream& os, const BitmaskBoard& board);

std::istream& operator>>(std::istream& is, BitmaskBoard& board);
