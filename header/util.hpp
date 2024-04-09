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

void printBoard(BitmaskBoard& board_layout);

void addMoveToHistory(robin_hood::unordered_map<uint64_t, int>& gameHistory, BitmaskBoard& move, bool isMaxPlayer);

void removeMoveFromHistory(robin_hood::unordered_map<uint64_t, int>& gameHistory, BitmaskBoard& move, bool isMaxPlayer);