#pragma once

#include <iostream>
#include <tuple>
#include <fstream>
#include <cstring>
#include <random>
#include <time.h> 
#include <vector>
#include <set>
#include <memory>
#include <windows.h>
#include <cstdlib>   // for rand() and srand()
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class BitmaskBoard {
private:
    uint64_t whitePawns, blackPawns, whiteKings, blackKings, emptySquares;

    uint64_t ONE = 1ULL, ZERO = 0ULL, INITIAL_POSITION = 0xffff0000ffff00ULL;

    void clearPosition(int i, int j) {
        uint64_t mask = ~(1ULL << (i * 8 + j));
        whitePawns &= mask;
        blackPawns &= mask;
        whiteKings &= mask;
        blackKings &= mask;
        // emptySquares |= mask;
    }

public:
    // Default constructor initializes all masks to 0
    BitmaskBoard() : whitePawns(0), blackPawns(0), whiteKings(0), blackKings(0), emptySquares() {}

    // Copy constructor
    BitmaskBoard(const BitmaskBoard& other)
        : whitePawns(other.whitePawns), blackPawns(other.blackPawns),
          whiteKings(other.whiteKings), blackKings(other.blackKings) {}

    char get(int i, int j) const {
        uint64_t pos = 1ULL << (i * 8 + j);
        if (whitePawns & pos) return 2;
        if (blackPawns & pos) return 1;
        if (whiteKings & pos) return 4;
        if (blackKings & pos) return 3;
        return 0; // Empty
    }

    void set(int i, int j, char piece) {
        clearPosition(i, j); // Clear the piece at (i, j) if any
        uint64_t pos = 1ULL << (i * 8 + j);
        switch (piece) {
            case 1: blackPawns |= pos; break;
            case 2: whitePawns |= pos; break;
            case 3: blackKings |= pos; break;
            case 4: whiteKings |= pos; break;
            default: break; // Do nothing for invalid piece
        }
        emptySquares &= ~(pos);
    }

    void set_whitePawn(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        whitePawns |= pos;
        emptySquares &= ~(pos);
    }

    void set_blackPawn(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        blackPawns |= pos;
        emptySquares &= ~(pos);
    }

    void set_whiteKing(int i, int j) {
        uint64_t mask = ~(1ULL << (i * 8 + j));
        whitePawns &= mask;

        uint64_t pos = 1ULL << (i * 8 + j);
        whiteKings |= pos;
        emptySquares &= ~(pos);
    }

    void set_blackKing(int i, int j) {
        uint64_t mask = ~(1ULL << (i * 8 + j));
        blackPawns &= mask;

        uint64_t pos = 1ULL << (i * 8 + j);
        blackKings |= pos;
        emptySquares &= ~(pos);
    }

    bool check_empty_index(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        if(emptySquares & pos)
            return true;
        return false;
    }

    bool check_index_has_whitePawn(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        if(whitePawns & pos)
            return true;
        return false;
    }

    bool check_index_has_blackPawn(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        if(blackPawns & pos)
            return true;
        return false;
    }

    bool check_index_has_whiteKing(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        if(whiteKings & pos)
            return true;
        return false;
    }

    bool check_index_has_blackKing(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        if(blackKings & pos)
            return true;
        return false;
    }

    uint64_t get_color_mask(char color) {
        if(color == 1)
            return blackPawns | blackKings;
        else
            return whitePawns | whiteKings;
    }

    // Define the < operator
    bool operator<(const BitmaskBoard& other) const {
        // Implement comparison logic here
        // This is a simple example comparing a hypothetical member variable
        // You should adjust this to fit your class design and comparison criteria
        return blackKings + blackPawns < whiteKings + whitePawns;
    }

    // int evaluate_board() {
    //     // Count pieces using popcount (population count - counts the number of set bits)
    //     int black_pawns = __builtin_popcountll(blackPawns);
    //     int white_pawns = __builtin_popcountll(whitePawns);
    //     int black_kings = __builtin_popcountll(blackKings);
    //     int white_kings = __builtin_popcountll(whiteKings);

    //     int sum = 0;

    //     // Basic evaluation based on piece counts
    //     sum += 100 * (black_pawns - white_pawns); // Pawn difference
    //     sum += 350 * (black_kings - white_kings); // King difference

    //     // Additional evaluations could go here

    //     return sum;
    // }

    int evaluate_board() {
        // Count pieces using popcount (population count - counts the number of set bits)
        int black_pawns = __builtin_popcountll(blackPawns);
        int white_pawns = __builtin_popcountll(whitePawns);
        int black_kings = __builtin_popcountll(blackKings);
        int white_kings = __builtin_popcountll(whiteKings);

        int sum = 0;

        // Base score for pawns and kings
        sum += 100 * (black_pawns - white_pawns); // Pawn difference
        sum += 450 * (black_kings - white_kings); // King difference

        // Bonus for pawn advancement:
        // Give more value to pawns further down the board for black, and up the board for white.
        // This is a simplistic approach. You could refine it by adjusting the row weights or using different scoring for different rows.
        
        uint64_t mask;
        int bonus = 0;

        // Bonus for black pawns based on their row
        for (int row = 1; row <= 6; ++row) {
            mask = 0xFFLL << (row * 8); // Select the row
            bonus += (row + 1) * __builtin_popcountll(blackPawns & mask); // row+1 to give higher bonus as pawns advance
        }
        sum += bonus;

        // Reset bonus for white pawns
        bonus = 0;

        // Bonus for white pawns based on their row (in reverse)
        for (int row = 6; row >= 1; --row) {
            mask = 0xFFLL << (row * 8); // Select the row
            bonus += (8 - row) * __builtin_popcountll(whitePawns & mask); // 8-row to give higher bonus as pawns advance
        }
        sum -= bonus;

        // Additional evaluations could go here

        return sum;
    }
};