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
#include "robin_hood.h"
#include "constant.hpp"

class BitmaskBoard {
private:
    uint64_t whitePawns, blackPawns, whiteKings, blackKings;
    bool isWhiteTurn;

    // uint64_t ONE = 1ULL, ZERO = 0ULL, INITIAL_POSITION = 0xffff0000ffff00ULL;

public:
    friend std::ostream& operator<<(std::ostream& os, const BitmaskBoard& board);
    friend std::istream& operator>>(std::istream& is, BitmaskBoard& board);

    // Default constructor initializes all masks to 0
    BitmaskBoard() : whitePawns(0x0ULL), blackPawns(0x0ULL), whiteKings(0x0ULL), blackKings(0x0ULL), isWhiteTurn(true) {}

    // Copy constructor
    BitmaskBoard(const BitmaskBoard& other)
        : whitePawns(other.whitePawns), blackPawns(other.blackPawns),
          whiteKings(other.whiteKings), blackKings(other.blackKings),
          isWhiteTurn(other.isWhiteTurn) {}
    
    // copy assignmet operator
    BitmaskBoard& operator=(const BitmaskBoard& other) {
        whitePawns = other.whitePawns;
        blackPawns = other.blackPawns;
        whiteKings = other.whiteKings;
        blackKings = other.blackKings;
        isWhiteTurn = other.isWhiteTurn;
        return *this;
    }

    // empty board
    static BitmaskBoard emptyBoard() {
        return BitmaskBoard();
    }

    char get(int i, int j) const {

        uint64_t pos = 1ULL << (i * 8 + j);
        // I want to right shift from MST to right
        // uint64_t pos = MST_64 >> (i * 8 + j);
        if (whitePawns & pos) return 2;
        if (blackPawns & pos) return 1;
        if (whiteKings & pos) return 4;
        if (blackKings & pos) return 3;
        return 0; // Empty
    }

    uint64_t getWhiteKings()  {
        return whiteKings;
    }

    uint64_t getBlackKings()  {
        return blackKings;
    }

    uint64_t getWhitePawns()  {
        return whitePawns;
    }

    uint64_t getBlackPawns()  {
        return blackPawns;
    }

    void clearPosition(int i, int j) {
        uint64_t pos = ~(1ULL << (i * 8 + j));
        whitePawns &= pos;
        blackPawns &= pos;
        whiteKings &= pos;
        blackKings &= pos;
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
    }

    void set_whitePawn(int i, int j) {
        // clearPosition(i, j);
        uint64_t pos = 1ULL << (i * 8 + j);
        whitePawns |= pos;
    }

    void set_blackPawn(int i, int j) {
        // clearPosition(i, j);
        uint64_t pos = 1ULL << (i * 8 + j);
        blackPawns |= pos;
    }

    void set_whiteKing(int i, int j) {
        // clearPosition(i, j);

        uint64_t mask = ~(1ULL << (i * 8 + j));
        whitePawns &= mask;

        uint64_t pos = 1ULL << (i * 8 + j);
        whiteKings |= pos;
    }

    void set_blackKing(int i, int j) {
        // clearPosition(i, j);

        uint64_t mask = ~(1ULL << (i * 8 + j));
        blackPawns &= mask;

        uint64_t pos = 1ULL << (i * 8 + j);
        blackKings |= pos;
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

    // 0 for black, 1 for white
    void setTurn(bool turn) {
        isWhiteTurn = turn;
    }

    bool getTurn() const {
        return isWhiteTurn;
    }

    // Define the < operator
    // bool operator<(const BitmaskBoard& other) const {
    //     // Implement comparison logic here
    //     return blackKings + blackPawns < whiteKings + whitePawns;
    // }

    bool operator==(const BitmaskBoard& other) const {
        return whitePawns == other.whitePawns && blackPawns == other.blackPawns &&
               whiteKings == other.whiteKings && blackKings == other.blackKings
               && isWhiteTurn == other.isWhiteTurn;
    }

    // more advanced methods

    bool capture_available() const {
        return capture_available(isWhiteTurn);
    }

    bool capture_available(bool isWhiteTurn) const {

        // Masks to prevent wrap-around horizontally
        uint64_t notAFile = 0b0111111101111111011111110111111101111111011111110111111101111111;
        uint64_t notHFile = 0b1111111011111110111111101111111011111110111111101111111011111110; 
        uint64_t notBFile = 0b1011111110111111101111111011111110111111101111111011111110111111;
        uint64_t notGFile = 0b1111110111111101111111011111110111111101111111011111110111111101;

        uint64_t notRank8 = 0b1111111111111111111111111111111111111111111111111111111100000000;
        uint64_t notRank7 = 0b1111111111111111111111111111111111111111111111110000000011111111;
        uint64_t notRank1 = 0b1111111100000000111111111111111111111111111111111111111111111111;
        uint64_t notRank0 = 0b0000000011111111111111111111111111111111111111111111111111111111;

        // Combine all pieces into one bitboard for checking empty squares
        uint64_t allPieces = whitePawns | blackPawns | whiteKings | blackKings;
        uint64_t emptySquares = ~allPieces;

        uint64_t opponentPieces = isWhiteTurn ? blackPawns | blackKings : whitePawns | whiteKings;
        uint64_t playerPieces = isWhiteTurn ? whitePawns | whiteKings : blackPawns | blackKings;
        
        auto playerPiecesForLeft = playerPieces & notAFile & notBFile;
        auto opponentPiecesForLeft = opponentPieces & notAFile;
        auto playerPiecesForRight = playerPieces & notHFile & notGFile;
        auto opponentPiecesForRight = opponentPieces & notHFile;
        // Check horizontal captures
        // Right captures: piece, opponent piece, empty square
        bool leftCapture = ( (playerPiecesForLeft << 1) & opponentPiecesForLeft) && 
                            (((playerPiecesForLeft << 2) & (opponentPiecesForLeft << 1) & emptySquares));
        // Left captures: piece, opponent piece, empty square
        bool rightCapture = ((playerPiecesForRight >> 1) & opponentPiecesForRight) && 
                           (((playerPiecesForRight >> 2) & (opponentPiecesForRight >> 1) & emptySquares));

        // Check vertical captures for pawns
        bool upCapture = false, downCapture = false;

        auto playerPiecesForUp = playerPieces & notRank1 & notRank0;
        auto opponentPiecesForUp = opponentPieces & notRank0;
        auto playerPiecesForDowm = playerPieces & notRank7 & notRank8;
        auto opponentPiecesForDown = opponentPieces & notRank8;
        if (isWhiteTurn) { // White's turn
            // Up captures: piece, opponent piece, empty square
            upCapture = ((playerPiecesForUp << 8) & opponentPiecesForUp) && 
                        (((playerPiecesForUp << 16) & (opponentPiecesForUp << 8) & emptySquares)); 
        } else { // Black's turn
            // Down captures: piece, opponent piece, empty square
            downCapture = ((playerPiecesForDowm >> 8) & opponentPiecesForDown) && 
                          (((playerPiecesForDowm >> 16) & (opponentPiecesForDown >> 8) & emptySquares));
        }

        return rightCapture || leftCapture || upCapture || downCapture;
    }

    bool hasWhiteKing() {
        return whiteKings != 0;
    }

    bool hasBlackKing() {
        return blackKings != 0;
    }

    int calculate_advancment_score() {
        int sum = 0;

        // Corrected masks and calculations for pawn advancement
        uint64_t advancementMaskWhite = 0x000000FF00000000ULL; // 2nd row from the opponent's side for black
        uint64_t advancementMaskBlack = 0x00000000FF000000ULL; // 2nd row from the opponent's side for white
        sum += 5 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // More advancement bonuses
        advancementMaskWhite = 0x00000000FF000000ULL; // 3rd row from the opponent's side for black
        advancementMaskBlack = 0x000000FF00000000ULL; // 3rd row from the opponent's side for white
        
        // in early game only advance at the edges
        sum += 10 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x0000000000FF0000ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x0000FF0000000000ULL; // 4th row from the opponent's side for white
        sum += 15 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x000000000000FF00ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x00FF000000000000ULL; // 4th row from the opponent's side for white
        sum +=  50 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));
        return sum;
    }

    bool passage_is_clear(BitmaskBoard& board_layout, char row, char col, char turn) {
        if (turn == 1) {
            if (col == 7) {
                if (!board_layout.check_index_has_whitePawn(6, 7) && !board_layout.check_index_has_whitePawn(6, 6) && !board_layout.check_index_has_whiteKing(6, 7) && !board_layout.check_index_has_whiteKing(6, 6)) {
                    return true;
                }
                return false;
            }
            if (col == 0) {
                if (!board_layout.check_index_has_whitePawn(6, 0) && !board_layout.check_index_has_whitePawn(6, 1) && !board_layout.check_index_has_whiteKing(6, 0) && !board_layout.check_index_has_whiteKing(6, 1)) {
                    return true;
                }
                return false;
            }
            if ((board_layout.check_index_has_whitePawn(row + 1, col + 1) || board_layout.check_index_has_whiteKing(row + 1, col + 1)) ^
                (board_layout.check_index_has_whitePawn(row + 1, col - 1) || board_layout.check_index_has_whiteKing(row + 1, col - 1))) {
                return false;
            }
            for (char j = -2; j <= 2; j++) {
                if (col + j < 0) {
                    continue;
                }
                if (col + j > 7) {
                    break;
                }
                if (board_layout.check_index_has_whitePawn(row + 2, col + j) || board_layout.check_index_has_whiteKing(row + 2, col + j)) {
                    return false;
                }
            }
            return true;
        }
        if (turn == 2) {
            if (col == 7) {
                if (!board_layout.check_index_has_blackPawn(1, 7) && !board_layout.check_index_has_blackPawn(1, 6) && !board_layout.check_index_has_blackKing(1, 7) && !board_layout.check_index_has_blackKing(1, 6)) {
                    return true;
                }
                return false;
            }
            if (col == 0) {
                if (!board_layout.check_index_has_blackPawn(1, 0) && !board_layout.check_index_has_blackPawn(1, 1) && !board_layout.check_index_has_blackKing(1, 0) && !board_layout.check_index_has_blackKing(1, 1)) {
                    return true;
                }
                return false;
            }
            if ((board_layout.check_index_has_blackPawn(row - 1, col + 1) || board_layout.check_index_has_blackKing(row - 1, col + 1)) ^
                (board_layout.check_index_has_blackPawn(row - 1, col - 1) || board_layout.check_index_has_blackKing(row - 1, col - 1))) {
                return false;
            }
            for (char j = -2; j <= 2; j++) {
                if (col + j < 0) {
                    continue;
                }
                if (col + j > 7) {
                    break;
                }
                if (board_layout.check_index_has_blackPawn(row - 2, col + j) || board_layout.check_index_has_blackKing(row - 2, col + j)) {
                    return false;
                }
            }
            return true;
        }
        return false; // Return false for any other turn value
    }

    int evaluate_board(robin_hood::unordered_map<uint64_t, int> &gameHistory) {

        // bool swapped = false;
        // // if there is a capture do it
        // if(capture_available(!isWhiteTurn)) {
        //     bool isEmptyForceList = false;
        //     auto b2 = *this;
        //     auto moves = get_all_moves(b2, isWhiteTurn ? 2 : 1, isEmptyForceList);
        //     *this = moves[0]; // play that single move
        //     isWhiteTurn = !isWhiteTurn; // now its the other player's turn
        //     swapped = true;
        // }


        

        // Count pieces using popcount (population count - counts the number of set bits)
        int nb_black_pawns = __builtin_popcountll(blackPawns);
        int nb_white_pawns = __builtin_popcountll(whitePawns);
        int nb_black_kings = __builtin_popcountll(blackKings);
        int nb_white_kings = __builtin_popcountll(whiteKings);

        int total_pieces = nb_black_pawns + nb_white_pawns + nb_black_kings + nb_white_kings;
        int nb_black_pieces = nb_black_pawns + nb_black_kings;
        int nb_white_pieces = nb_white_pawns + nb_white_kings;

        // wins
        if(nb_black_pieces == 0)
            return MIN_EVAL;
        if(nb_white_pieces == 0)
            return MAX_EVAL;
        // draw
        if(draw(gameHistory))
            return 0;

        int sum = 0;
        double scaling_factor = 1;
        if(total_pieces >= 16)
            scaling_factor = 1.0 + (32.0 - total_pieces)/128.0; 
        else if (total_pieces >= 12)
            scaling_factor = 1.0 + (32.0 - total_pieces)/100.0; // in endgames, give more weight to the pawns and piece difference
        else if (total_pieces >= 8)
            scaling_factor = 1.0 + (32.0 - total_pieces)/64.0; // in endgames, give more weight to the pawns and piece difference
        else
            scaling_factor = 1.0 + (32.0 - total_pieces)/32.0; // in endgames, give more weight to the pawns and piece difference

        // Base scores for pawns and kings
        if(blackKings == 0 && whiteKings == 0) {
            // add scaling factor
            sum += scaling_factor * (100 * (nb_black_pawns - nb_white_pawns)); // Pawn difference
            sum += scaling_factor * (430 * (nb_black_kings - nb_white_kings)); // King difference
        }
        else {
            sum +=  (100 * (nb_black_pawns - nb_white_pawns)); // Pawn difference
            sum +=  (430 * (nb_black_kings - nb_white_kings)); // King difference
        }
       

        constexpr uint64_t leftEdgeMask = 0x0101010101010101; // Left edge of the board
        constexpr uint64_t rightEdgeMask = 0x8080808080808080; // Right edge of the board
        
        // Edge bonuses specifically for the right and left edges
        // if in endgame dont give edge bonus
        // if(total_pieces > 16)
        // {
            int edge_bonus_black = 3 * (__builtin_popcountll(blackPawns & leftEdgeMask) + __builtin_popcountll(blackPawns & rightEdgeMask) + __builtin_popcountll(blackKings & leftEdgeMask) + __builtin_popcountll(blackKings & rightEdgeMask));
            int edge_bonus_white = 3 * (__builtin_popcountll(whitePawns & leftEdgeMask) + __builtin_popcountll(whitePawns & rightEdgeMask) + __builtin_popcountll(whiteKings & leftEdgeMask) + __builtin_popcountll(whiteKings & rightEdgeMask));
            sum += edge_bonus_black - edge_bonus_white;
        // }

        // Corrected masks and calculations for pawn advancement
        uint64_t advancementMaskWhite = 0x000000FF00000000ULL; // 2nd row from the opponent's side for black
        uint64_t advancementMaskBlack = 0x00000000FF000000ULL; // 2nd row from the opponent's side for white
        sum += 5 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // More advancement bonuses
        advancementMaskWhite = 0x00000000FF000000ULL; // 3rd row from the opponent's side for black
        advancementMaskBlack = 0x000000FF00000000ULL; // 3rd row from the opponent's side for white
        
        // in early game only advance at the edges
        if(total_pieces > 26) {

            if(AI_IS_WHITE)
            {
                sum += 10 * (int)(__builtin_popcountll(blackPawns & (advancementMaskBlack & leftEdgeMask)));
                sum += 10 * (int)(__builtin_popcountll(blackPawns & (advancementMaskBlack & rightEdgeMask)));
                
                // even here don't let whit push
                sum += 10 * (int)( - __builtin_popcountll(whitePawns & advancementMaskWhite));
            }
            else 
            {
                sum += - 10 * (int)(__builtin_popcountll(whitePawns & (advancementMaskWhite & leftEdgeMask)));
                sum += - 10 * (int)(__builtin_popcountll(whitePawns & (advancementMaskWhite & rightEdgeMask)));
                
                // even here don't let whit push
                sum += 10 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack));
            }
        }
        else // in middle ad endgame reward any advancement
            sum += 10 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x0000000000FF0000ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x0000FF0000000000ULL; // 4th row from the opponent's side for white
        sum += 15 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x000000000000FF00ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x00FF000000000000ULL; // 4th row from the opponent's side for white
        sum +=  50 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // reward black kings that are on the first rank
        sum += 10 * (int)(__builtin_popcountll(blackKings & 0x00000000000000FFULL));

        // reward white kings that are on the last rank
        sum += 10 * (int)(__builtin_popcountll(whiteKings & 0xFF00000000000000ULL));


        // Balance bonus: Favor spreading pieces between the left and right halves
        constexpr uint64_t leftHalfMask = 0x0F0F0F0F0F0F0F0F; // Left 4 columns
        constexpr uint64_t rightHalfMask = 0xF0F0F0F0F0F0F0F0; // Right 4 columns
        int balance_black = abs(__builtin_popcountll(blackPawns & leftHalfMask) - __builtin_popcountll(blackPawns & rightHalfMask));
        int balance_white = abs(__builtin_popcountll(whitePawns & leftHalfMask) - __builtin_popcountll(whitePawns & rightHalfMask));
        sum -= 2 * (balance_black - balance_white); // Penalize imbalance more subtly than before, adjusting the weight as needed
        
        // if passage is clear, give a bonus

        if(!blackKings && !whiteKings) // bo damas on board
        {
            for(int j=0; j<8; j++)
            {
                if(!isWhiteTurn && check_index_has_blackPawn(4, j) && passage_is_clear(*this, 4, j, 1)) {
                    sum += 70;
                }
                if(isWhiteTurn && check_index_has_whitePawn(3, j) && passage_is_clear(*this, 3, j, 2)) {
                    sum -= 70;
                }
            }
        }
        
        // game finished
        
        // draw
        if((nb_black_pieces == 1) && (nb_white_pieces == 1))
            return 0;
        // if one of sides is on 1 piece its a draw
        if((nb_black_pieces == 1) && (nb_white_pieces > 1) && sum > 0)
            sum = 0;
        if((nb_white_pieces == 1) && (nb_white_pieces > 1) && sum < 0)
            sum = 0;

        return sum;
    }

    // overrided version without game history
    int evaluate_board() {

        // Count pieces using popcount (population count - counts the number of set bits)
        int nb_black_pawns = __builtin_popcountll(blackPawns);
        int nb_white_pawns = __builtin_popcountll(whitePawns);
        int nb_black_kings = __builtin_popcountll(blackKings);
        int nb_white_kings = __builtin_popcountll(whiteKings);

        int total_pieces = nb_black_pawns + nb_white_pawns + nb_black_kings + nb_white_kings;
        int nb_black_pieces = nb_black_pawns + nb_black_kings;
        int nb_white_pieces = nb_white_pawns + nb_white_kings;

        int sum = 0;
        double scaling_factor = 1;
        if(total_pieces >= 16)
            scaling_factor = 1.0 + (32.0 - total_pieces)/128.0; 
        else if (total_pieces >= 12)
            scaling_factor = 1.0 + (32.0 - total_pieces)/100.0; // in endgames, give more weight to the pawns and piece difference
        else if (total_pieces >= 8)
            scaling_factor = 1.0 + (32.0 - total_pieces)/64.0; // in endgames, give more weight to the pawns and piece difference
        else
            scaling_factor = 1.0 + (32.0 - total_pieces)/32.0; // in endgames, give more weight to the pawns and piece difference

        // Base scores for pawns and kings
        sum += scaling_factor * (100 * (nb_black_pawns - nb_white_pawns)); // Pawn difference
        sum += scaling_factor * (450 * (nb_black_kings - nb_white_kings)); // King difference

        
        constexpr uint64_t leftEdgeMask = 0x0101010101010101; // Left edge of the board
        constexpr uint64_t rightEdgeMask = 0x8080808080808080; // Right edge of the board
        
        // Edge bonuses specifically for the right and left edges
        // if in endgame dont give edge bonus
        if(total_pieces > 16)
        {
            int edge_bonus_black = 3 * (__builtin_popcountll(blackPawns & leftEdgeMask) + __builtin_popcountll(blackPawns & rightEdgeMask) + __builtin_popcountll(blackKings & leftEdgeMask) + __builtin_popcountll(blackKings & rightEdgeMask));
            int edge_bonus_white = 3 * (__builtin_popcountll(whitePawns & leftEdgeMask) + __builtin_popcountll(whitePawns & rightEdgeMask) + __builtin_popcountll(whiteKings & leftEdgeMask) + __builtin_popcountll(whiteKings & rightEdgeMask));
            sum += edge_bonus_black - edge_bonus_white;
        }

        // Corrected masks and calculations for pawn advancement
        uint64_t advancementMaskWhite = 0x000000FF00000000ULL; // 2nd row from the opponent's side for black
        uint64_t advancementMaskBlack = 0x00000000FF000000ULL; // 2nd row from the opponent's side for white
        sum += 5 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // More advancement bonuses
        advancementMaskWhite = 0x00000000FF000000ULL; // 3rd row from the opponent's side for black
        advancementMaskBlack = 0x000000FF00000000ULL; // 3rd row from the opponent's side for white
        
        // in early game only advance at the edges
        if(total_pieces > 26) {
            sum += 10 * (int)(__builtin_popcountll(blackPawns & (advancementMaskBlack & leftEdgeMask)));
            sum += 10 * (int)(__builtin_popcountll(blackPawns & (advancementMaskBlack & rightEdgeMask)));
            
            // even here don't let whit push
            sum += 10 * (int)( - __builtin_popcountll(whitePawns & advancementMaskWhite));
        }
        else // in middle ad endgame reward any advancement
            sum += 10 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x0000000000FF0000ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x0000FF0000000000ULL; // 4th row from the opponent's side for white
        sum += 15 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Extra for closer pawns
        advancementMaskWhite = 0x000000000000FF00ULL; // 4th row from the opponent's side for black
        advancementMaskBlack = 0x00FF000000000000ULL; // 4th row from the opponent's side for white
        sum +=  50 * (int)(__builtin_popcountll(blackPawns & advancementMaskBlack) - __builtin_popcountll(whitePawns & advancementMaskWhite));

        // Note: Adjust the row targeting and bonus points as needed to fit the game's strategy.


        // Balance bonus: Favor spreading pieces between the left and right halves
        constexpr uint64_t leftHalfMask = 0x0F0F0F0F0F0F0F0F; // Left 4 columns
        constexpr uint64_t rightHalfMask = 0xF0F0F0F0F0F0F0F0; // Right 4 columns
        int balance_black = abs(__builtin_popcountll(blackPawns & leftHalfMask) - __builtin_popcountll(blackPawns & rightHalfMask));
        int balance_white = abs(__builtin_popcountll(whitePawns & leftHalfMask) - __builtin_popcountll(whitePawns & rightHalfMask));
        sum -= 2 * (balance_black - balance_white); // Penalize imbalance more subtly than before, adjusting the weight as needed
        
        // if passage is clear, give a bonus
        for(int j=0; j<8; j++)
        {
            if(!isWhiteTurn && check_index_has_blackPawn(4, j) && passage_is_clear(*this, 4, j, 1)) {
                sum += 70;
            }
            if(isWhiteTurn && check_index_has_whitePawn(3, j) && passage_is_clear(*this, 3, j, 2)) {
                sum -= 70;
            }
        }
        

        // game finished
        // wins
        if(nb_black_pieces == 0)
            return MIN_EVAL;
        if(nb_white_pieces == 0)
            return MAX_EVAL;
        // draw
        if((nb_black_pieces == 1) && (nb_white_pieces == 1))
            return 0;
        // if one of sides is on 1 piece its a draw
        if((nb_black_pieces == 1) && (nb_white_pieces > 1) && sum > 0)
            sum = 0;
        if((nb_white_pieces == 1) && (nb_white_pieces > 1) && sum < 0)
            sum = 0;

        return sum;
    }

    bool player_won() {
        if(((__builtin_popcountll(blackKings) + __builtin_popcountll(blackPawns)) == 0) || ((__builtin_popcountll(whiteKings) + __builtin_popcountll(whitePawns)) == 0))
            return true;
        return false;
    }

    bool black_won() {
        if(((__builtin_popcountll(whiteKings) + __builtin_popcountll(whitePawns)) == 0))
            return true;
        return false;
    }

    bool white_won() {
        if(((__builtin_popcountll(blackKings) + __builtin_popcountll(blackPawns)) == 0))
            return true;
        return false;
    }

    bool draw() {
        if(((__builtin_popcountll(blackKings) + __builtin_popcountll(blackPawns)) == 1)
        && ((__builtin_popcountll(whiteKings) + __builtin_popcountll(whitePawns)) == 1))
            return true;
        
        
        return false;
    }

    bool draw(robin_hood::unordered_map<uint64_t, int> &gameHistory) {
        if(player_won())
            return false;
        if(((__builtin_popcountll(blackKings) + __builtin_popcountll(blackPawns)) == 1)
        && ((__builtin_popcountll(whiteKings) + __builtin_popcountll(whitePawns)) == 1))
            return true;    
        if(gameHistory[hash()] >= 3)
            return true;
        return false;
    }

    // Function to calculate hash key for a board layout and turn
    uint64_t calculateHashKey(char turn, std::array<std::array<uint64_t, 64>, 4> &zobristTable) {
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

        return hashKey;
    }

    // Hash method
    std::size_t hash() const {
        std::hash<uint64_t> hasher;
        size_t hashValue = 17; // Start with a non-zero constant
        
        // Combine the hash values of the bitmasks using a prime number
        hashValue = hashValue * 31 + hasher(whitePawns);
        hashValue = hashValue * 31 + hasher(blackPawns);
        hashValue = hashValue * 31 + hasher(whiteKings);
        hashValue = hashValue * 31 + hasher(blackKings);
        
        // Incorporate isWhiteTurn into the hash
        // Treat isWhiteTurn as an additional bit in the hash computation. 
        // You can use a simple conditional to add a unique value (like a small prime number) 
        // to distinguish between the two possible states.
        // hashValue = hashValue * 31 + (isWhiteTurn ? 1231 : 1237); // Prime numbers for true/false

        return hashValue;
    }

};


namespace std {
    template<>
    struct hash<BitmaskBoard> {
        std::size_t operator()(const BitmaskBoard& board) const noexcept {
            return board.hash(); // Use the hash method of BitmaskBoard
        }
    };
}
