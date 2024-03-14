#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint> // For uint64_t

class BitmaskBoard {    

public:
    uint64_t whitePawns, blackPawns, whiteKings, blackKings, emptySquares;

    uint64_t ONE = 1ULL, ZERO = 0ULL, INITIAL_POSITION = 0x00FFFF0000FFFF00ULL;

    void clearPosition(int i, int j) {
        uint64_t mask = ~(1ULL << (i * 8 + j));
        whitePawns &= mask;
        blackPawns &= mask;
        whiteKings &= mask;
        blackKings &= mask;
        // emptySquares |= mask;
    }

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
        int edge_bonus_black = 15 * (__builtin_popcountll(blackPawns & leftEdgeMask) + __builtin_popcountll(blackPawns & rightEdgeMask) + __builtin_popcountll(blackKings & leftEdgeMask) + __builtin_popcountll(blackKings & rightEdgeMask));
        int edge_bonus_white = 15 * (__builtin_popcountll(whitePawns & leftEdgeMask) + __builtin_popcountll(whitePawns & rightEdgeMask) + __builtin_popcountll(whiteKings & leftEdgeMask) + __builtin_popcountll(whiteKings & rightEdgeMask));
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
        sum -= 5 * (balance_black - balance_white); // Penalize imbalance more subtly than before, adjusting the weight as needed

        return sum;
    }


};

void test_evaluation_function() {
    BitmaskBoard board;
    //create this board
    // 0 0 0 0 0 0 0 0
    // 1 1 1 1 1 1 1 1
    // 0 0 0 0 1 1 1 1
    // 1 1 1 1 0 0 0 0
    // 0 0 0 0 0 0 0 0
    // 2 2 2 2 2 2 2 2
    // 2 2 2 2 2 2 2 2
    // 0 0 0 0 0 0 0 0
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(j == 0 && i == 3)
                board.set_blackPawn(i, j);

            if(j == 7 && i == 5)
                board.set_whitePawn(i, j);
            
            if(i==1 || i==2)
                board.set_blackPawn(i, j);
            else if(i == 6 || i==7)
                board.set_whitePawn(i, j);
        }
    }

    //print the board
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            std::cout<<(int)board.get(i, j)<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout << "Evaluation: " << board.evaluate_board() << std::endl; // Should print 0

}

int main() {
    test_evaluation_function();
    
    // int n; 
    // std::cin>>n;
    // std::vector<BitmaskBoard> all_moves(n); 
    // BitmaskBoard boardCopy = board; // Using the copy constructor
    
    // clock_t begin = clock();
    
    // for(int i=0; i<n; i++)
    // {
    //     board.set_whiteKing(i, i);
    // }
    
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // std::cout<<"Time: "<<time_spent<<std::endl;


    // BitmaskBoard boardCopy = board; // Using the copy constructor
    // std::cout << "Piece at (0,0) in original: " << (int)board.get(0, 0) << std::endl; // Should print 1
    // std::cout << "Piece at (7,7) in original: " << (int)board.get(7, 7) << std::endl; // Should print 4
    
    // board.set(0, 0, 2);
    // // The copy should have the same state as the original
    // std::cout << "Piece at (0,0) in copy: " << (int)boardCopy.get(0, 0) << std::endl; // Should also print 1
    // std::cout << "Piece at (7,7) in copy: " << (int)boardCopy.get(7, 7) << std::endl; // Should also print 4

    return 0;
}
