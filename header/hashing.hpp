#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h> 
#include <vector>
#include <cstdlib>   // for rand() and srand()
#include "BitmaskBoard.hpp"
#include "robin_hood.h"

typedef struct TTValue {
    int depth;
    int eval;
    BitmaskBoard bestMove;

    TTValue() : depth(0), eval(0), bestMove() {}

    TTValue(int depth, int eval, BitmaskBoard bestMove) : 
    depth(depth), eval(eval), bestMove(bestMove) {}
    
} TTValue;

void writeGameHistoryToFile(const robin_hood::unordered_map<uint64_t, int>& gameHistory, const std::string& filename);

void readGameHistoryFromFile(robin_hood::unordered_map<uint64_t, int>& gameHistory, const std::string& filename);

void writeTranspositionTableToFile(robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, const std::string& filename);

void readTranspositionTableFromFile(robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, const std::string& filename);

std::ostream& operator<<(std::ostream& os, const TTValue& value);

std::istream& operator>>(std::istream& is, TTValue& value);

std::ostream& operator<<(std::ostream& os, const BitmaskBoard& board);

std::istream& operator>>(std::istream& is, BitmaskBoard& board);
