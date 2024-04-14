#pragma once
#include <iostream>
#include <functional>
#include <windows.h>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <time.h> 
#include <vector>
#include "BitmaskBoard.hpp"
#include "hashing.hpp"
#include "robin_hood.h"

void order_moves2(
    BitmaskBoard& board, 
    std::vector<BitmaskBoard>& moves, 
    robin_hood::unordered_map<BitmaskBoard, TTValue, BitmaskBoardHash>& transpositionTable, 
    char previousDepth, bool isMaxPlayer, int maxDepth) ;

int search2(
    char depth, bool max_player, BitmaskBoard& board_layout,
    int alpha, int beta, char akel_player, char akel_depth, bool akling,
     robin_hood::unordered_map<BitmaskBoard, TTValue, BitmaskBoardHash>& transpositionTable,
      BitmaskBoard& best_move, char maxDepth, robin_hood::unordered_map<uint64_t, int>& gameHistory);

std::pair<int, BitmaskBoard> iterativeDeepening(BitmaskBoard& initialBoard, char maxDepth, bool isMaxPlayer, robin_hood::unordered_map<BitmaskBoard, TTValue, BitmaskBoardHash>& transpositionTable, int maxTimeSeconds, robin_hood::unordered_map<uint64_t, int>& gameHistory);



