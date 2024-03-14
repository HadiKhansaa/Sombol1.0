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
        // print blackPawns in hex
        // std::cout <<  (int)(__builtin_popcountll(blackPawns & 0x00000000FF000000ULL)) - (int)(__builtin_popcountll(whitePawns & 0x00FF000000000000ULL))<< std::endl;

        // Count pieces using popcount (population count - counts the number of set bits)
        int black_pawns = __builtin_popcountll(blackPawns);
        int white_pawns = __builtin_popcountll(whitePawns);
        int black_kings = __builtin_popcountll(blackKings);
        int white_kings = __builtin_popcountll(whiteKings);

        int sum = 0;

        // Base scores for pawns and kings
        sum += 100 * (black_pawns - white_pawns); // Pawn difference
        sum += 450 * (black_kings - white_kings); // King difference

        // Edge bonuses specifically for the right and left edges
        uint64_t leftEdgeMask = 0x0101010101010101; // Left edge of the board
        uint64_t rightEdgeMask = 0x8080808080808080; // Right edge of the board
        int edge_bonus_black = 10 * (__builtin_popcountll(blackPawns & leftEdgeMask) + __builtin_popcountll(blackPawns & rightEdgeMask) + __builtin_popcountll(blackKings & leftEdgeMask) + __builtin_popcountll(blackKings & rightEdgeMask));
        int edge_bonus_white = 10 * (__builtin_popcountll(whitePawns & leftEdgeMask) + __builtin_popcountll(whitePawns & rightEdgeMask) + __builtin_popcountll(whiteKings & leftEdgeMask) + __builtin_popcountll(whiteKings & rightEdgeMask));
        sum += edge_bonus_black - edge_bonus_white;

        // Corrected masks and calculations for pawn advancement
        uint64_t advancementMaskWhite = 0x000000FF00000000ULL; // 2nd row from the opponent's side for black
        uint64_t advancementMaskBlack = 0x00000000FF000000ULL; // 2nd row from the opponent's side for white
        sum += 5 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // More advancement bonuses
        advancementMaskWhite = 0x00000000FF000000ULL; // 3rd row from the opponent's side for black
        advancementMaskBlack = 0x000000FF00000000ULL; // 3rd row from the opponent's side for white
        sum += 10 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x0000000000FF0000ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x0000FF0000000000ULL; // 4th row from the opponent's side for white
        sum += 15 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x000000000000FF00ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x00FF000000000000ULL; // 4th row from the opponent's side for white
        sum +=  20 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Note: Adjust the row targeting and bonus points as needed to fit the game's strategy.


        // Balance bonus: Favor spreading pieces between the left and right halves
        uint64_t leftHalfMask = 0x0F0F0F0F0F0F0F0F; // Left 4 columns
        uint64_t rightHalfMask = 0xF0F0F0F0F0F0F0F0; // Right 4 columns
        int balance_black = abs(__builtin_popcountll(blackPawns & leftHalfMask) - __builtin_popcountll(blackPawns & rightHalfMask));
        int balance_white = abs(__builtin_popcountll(whitePawns & leftHalfMask) - __builtin_popcountll(whitePawns & rightHalfMask));
        sum -= 2 * (balance_black - balance_white); // Penalize imbalance more subtly than before, adjusting the weight as needed

        return sum;
    }

    // Function to calculate hash key for a board layout and turn
    uint64_t calculateHashKey(char turn, std::array<std::array<uint64_t, 64>, 4> zobristTable) {
        uint64_t hashKey = 0;

        // Function to XOR pieces' positions based on their bitmasks
        auto xorPiecePositions = [&](uint64_t bitmask, int pieceType) {
            for (int position = 0; position < 64; ++position) {
                if (bitmask & (1ULL << position)) {
                    hashKey ^= zobristTable[pieceType][position];
                }
            }
        };

        // Apply XOR operations for each type of piece
        xorPiecePositions(blackPawns, 0); // Black pawns
        xorPiecePositions(whitePawns, 1); // White pawns
        xorPiecePositions(blackKings, 2); // Black kings
        xorPiecePositions(whiteKings, 3); // White kings

        // Include turn information in the hash
        hashKey ^= (turn == '1' ? zobristTable[0][0] : zobristTable[1][0]); // Example: Use first values as turn indicator
        // hashKey ^= whitePawns;
        // hashKey ^= blackPawns;
        // hashKey ^= whiteKings;
        // hashKey ^= blackKings;
        // turn == '1' ? hashKey ^= zobristTable[0][0] : hashKey ^= zobristTable[1][0];


        return hashKey;
    }
};