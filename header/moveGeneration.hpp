#pragma once
#include <iostream>
#include <time.h> 
#include <vector>
#include <cstdlib>   // for rand() and srand()
#include "BitmaskBoard.hpp"
#include "move.hpp"

std::pair<std::vector<char*>, char> get_valid_moves(char row, char col, char color, char streak, BitmaskBoard& board_layout, char eat_direction);

char get_eat_direction(char row1, char col1, char row2, char col2);

char get_piece_to_eat(BitmaskBoard& board_layout, char row, char col, char r, char c, char color);

char eat_piece_if_possible(BitmaskBoard& board_layout, char row, char col, char r, char c, char color);

std::vector<std::vector<char*>> eat_max2(char row, char col, BitmaskBoard& board_layout,  std::vector<std::vector<char*>> parent_list, char color, char eat_direction);

std::vector<char*> get_all_pieces(BitmaskBoard& board_layout, char color);

void move_piece(char* piece, char* move, BitmaskBoard& board_layout, std::vector<std::vector<char*>>& parent_list, char color);

std::vector<BitmaskBoard> get_all_moves(BitmaskBoard& board_layout, char color, bool& isEmptyForceList);

std::vector<BitmaskBoard> get_all_moves2(BitmaskBoard& board_layout, char color, bool& isEmptyForceList);

// new moveGeneration functions

std::pair<std::vector<Move>, char> get_valid_moves2(Piece piece, char streak, BitmaskBoard& board_layout, char eat_direction);

std::vector<Piece> get_all_pieces2(BitmaskBoard& board_layout, char color);

std::vector<std::vector<Piece>> eat_max3(Piece piece, BitmaskBoard& board_layout,  std::vector<std::vector<Piece>> parent_list, char eat_direction, char color);

std::vector<Move> get_all_valid_moves(BitmaskBoard& board_layout, char color, bool& isEmptyForceList, std::vector<std::vector<Piece>>& main_parent_list);

std::vector<BitmaskBoard> get_all_moves3(BitmaskBoard& board_layout, char color, bool& isEmptyForceList);
