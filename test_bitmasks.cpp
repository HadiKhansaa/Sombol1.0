#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint> // For uint64_t

class BitmaskBoard {
private:
    uint64_t whitePawns, blackPawns, whiteKings, blackKings;

public:
    // Default constructor initializes all masks to 0
    BitmaskBoard() : whitePawns(0), blackPawns(0), whiteKings(0), blackKings(0) {}

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

    void clearPosition(int i, int j) {
        uint64_t mask = ~(1ULL << (i * 8 + j));
        whitePawns &= mask;
        blackPawns &= mask;
        whiteKings &= mask;
        blackKings &= mask;
    }

    void set(int i, int j, char piece) {
        clearPosition(i, j); // Clear the piece at (i, j) if any

        if(piece == 0) return; // Do nothing for empty piece
        
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
        uint64_t pos = 1ULL << (i * 8 + j);
        whitePawns |= pos;
    }

    void set_blackPawn(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        blackPawns |= pos;
    }

    void set_whiteKing(int i, int j) {
        uint64_t pos = 1ULL << (i * 8 + j);
        whiteKings |= pos;
    }

    void set_blackKing(int i, int j) {
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

    // Define the < operator
    bool operator<(const BitmaskBoard& other) const {
        // Implement comparison logic here
        // This is a simple example comparing a hypothetical member variable
        // You should adjust this to fit your class design and comparison criteria
        return blackKings + blackPawns < whiteKings + whitePawns;
    }

    int evaluate_board() {
        // Count pieces using popcount (population count - counts the number of set bits)
        int black_pawns = __builtin_popcountll(blackPawns);
        int white_pawns = __builtin_popcountll(whitePawns);
        int black_kings = __builtin_popcountll(blackKings);
        int white_kings = __builtin_popcountll(whiteKings);

        int sum = 0;

        // Basic evaluation based on piece counts
        sum += 100 * (black_pawns - white_pawns); // Pawn difference
        sum += 350 * (black_kings - white_kings); // King difference

        // Additional evaluations could go here

        return sum;
    }
};

int main() {
    BitmaskBoard board;
    board.set(0, 0, 1); // Set a black pawn at position (0,0)
    board.set(7, 7, 4); // Set a white king at position (7,7)
    
    int n; 
    std::cin>>n;
    std::vector<BitmaskBoard> all_moves(n); 
    BitmaskBoard boardCopy = board; // Using the copy constructor
    
    clock_t begin = clock();
    
    for(int i=0; i<n; i++)
    {
        board.set_whiteKing(i, i);
    }
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"Time: "<<time_spent<<std::endl;


    // BitmaskBoard boardCopy = board; // Using the copy constructor
    // std::cout << "Piece at (0,0) in original: " << (int)board.get(0, 0) << std::endl; // Should print 1
    // std::cout << "Piece at (7,7) in original: " << (int)board.get(7, 7) << std::endl; // Should print 4
    
    // board.set(0, 0, 2);
    // // The copy should have the same state as the original
    // std::cout << "Piece at (0,0) in copy: " << (int)boardCopy.get(0, 0) << std::endl; // Should also print 1
    // std::cout << "Piece at (7,7) in copy: " << (int)boardCopy.get(7, 7) << std::endl; // Should also print 4

    return 0;
}
