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

bool check_if_piece_can_capture(char row, char col, BitmaskBoard& board_layout, char turn);

std::pair<std::vector<char*>, bool> check_for_possible_capture(BitmaskBoard& board_layout, char turn);

bool check_dama_has_akel(char row, char col, char color, BitmaskBoard& board_layout, char eat_direction);

bool equals(std::vector<char*>& a, std::vector<char*>& b);

bool doesnt_have(std::vector <std::vector<char*>>& a, std::vector<char*>& b);

bool passage_is_clear(BitmaskBoard& board_layout, char row, char col, char turn);

std::vector<char*> check_for_force(BitmaskBoard& board_layout, char turn, std::vector<char*> *pieces);

bool dama_is_captured(BitmaskBoard& board_layout, BitmaskBoard& move, char turn);

bool kingMoved(BitmaskBoard& board_layout, BitmaskBoard& move, char turn);