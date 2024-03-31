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
#include "hashing.hpp"
#include "robin_hood.h"

void order_moves2(
    BitmaskBoard& board, 
    std::vector<BitmaskBoard>& moves, 
    robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, 
    char previousDepth, bool isMaxPlayer);

int search2(
    char depth, bool max_player, BitmaskBoard& board_layout,
    int alpha, int beta, char akel_player, char akel_depth, bool akling,
     robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable,
      BitmaskBoard& best_move, char maxDepth, robin_hood::unordered_map<uint64_t, int>& gameHistory);

std::pair<int, BitmaskBoard> iterativeDeepening(BitmaskBoard& initialBoard, char maxDepth, bool isMaxPlayer, robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, int maxTimeSeconds, robin_hood::unordered_map<uint64_t, int>& gameHistory);



