#pragma once
#include <iostream>

const uint64_t MST_64 = 0b1000000000000000000000000000000000000000000000000000000000000000;  // 64 bits set to 1

const bool AI_IS_WHITE = true;

const int NUM_PIECES = 5;  // Number of piece types
const int NUM_SQUARES = 64;  // Number of squares on the board
const int NUM_TURNS = 2;  // Number of possible turns

const int MAX_DEPTH = 100;  // Depth of the search tree

const int MAX_TIME_SECONDS = 12;  // Maximum time allowed for the AI to make a move

const int MAX_EVAL = 1000000;
const int MIN_EVAL = -1000000;