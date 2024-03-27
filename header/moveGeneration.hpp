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

std::pair<std::vector<char*>, char> get_valid_moves(char row, char col, char color, char streak, BitmaskBoard& board_layout, char eat_direction);

char get_eat_direction(char row1, char col1, char row2, char col2);

char get_piece_to_eat(BitmaskBoard& board_layout, char row, char col, char r, char c, char color);

std::pair<BitmaskBoard, char> eat_piece_if_possible(BitmaskBoard& board_layout, char row, char col, char r, char c, char color);

std::vector<std::vector<char*>> eat_max2(char row, char col, BitmaskBoard& board_layout,  std::vector<std::vector<char*>> parent_list, char color, char eat_direction);

std::vector<char*> get_all_pieces(BitmaskBoard& board_layout, char color);

BitmaskBoard move_piece(char* piece, char* move, BitmaskBoard& board_layout, std::vector<std::vector<char*>>& parent_list, char color);

std::pair<std::vector<BitmaskBoard>, bool> get_all_moves(BitmaskBoard& board_layout, char color);