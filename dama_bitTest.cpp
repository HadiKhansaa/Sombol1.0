#include "BitmaskBoard.hpp"

using namespace std;



//counters
int movesSeen = 0;
int counter = 0;
int cacheHits = 0;

//hashing stuff
std::mt19937_64 rng(std::random_device{}());
std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());
std::unordered_map<uint64_t, std::pair<uint64_t, int>> transpositionTable;

const int NUM_PIECES = 5;  // Number of piece types
const int NUM_SQUARES = 64;  // Number of squares on the board
const int NUM_TURNS = 2;  // Number of possible turns

uint64_t lookupTable[NUM_PIECES][NUM_SQUARES][NUM_TURNS];

void initializeLookupTable() {
    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (int square = 0; square < NUM_SQUARES; ++square) {
            for (int turn = 0; turn < NUM_TURNS; ++turn) {
                lookupTable[piece][square][turn] = distribution(rng);
            }
        }
    }
}

uint64_t calculateHashKey(BitmaskBoard boardLayout, char turn) {
    uint64_t hashKey = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int square = row * 8 + col;
            int piece = boardLayout.get(row, col);  // Assuming the board layout stores piece characters (e.g., '1', '2', '3', '4')

            if (true) {
                hashKey ^= lookupTable[piece][square][turn - 1];
            }
        }
    }
    return hashKey;
}

// Function to save the transposition table to a file
void saveTranspositionTable(const std::unordered_map<uint64_t, std::pair<char, int>>& transpositionTable, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    size_t numEntries = transpositionTable.size();
    file.write(reinterpret_cast<const char*>(&numEntries), sizeof(size_t));

    for (const auto& entry : transpositionTable) {
        uint64_t hashKey = entry.first;
        char depth = entry.second.first;
        int evaluation = entry.second.second;

        file.write(reinterpret_cast<const char*>(&hashKey), sizeof(uint64_t));
        file.write(reinterpret_cast<const char*>(&depth), sizeof(char));
        file.write(reinterpret_cast<const char*>(&evaluation), sizeof(int));
    }

    file.close();
}

// Function to load the transposition table from a file
std::unordered_map<uint64_t, std::pair<char, int>> loadTranspositionTable(const std::string &filename) {
    std::unordered_map<uint64_t, std::pair<char, int>> transpositionTable;

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return transpositionTable;
    }

    size_t numEntries;
    file.read(reinterpret_cast<char*>(&numEntries), sizeof(size_t));

    for (size_t i = 0; i < numEntries; ++i) {
        uint64_t hashKey;
        char depth;
        int evaluation;

        file.read(reinterpret_cast<char*>(&hashKey), sizeof(uint64_t));
        file.read(reinterpret_cast<char*>(&depth), sizeof(char));
        file.read(reinterpret_cast<char*>(&evaluation), sizeof(int));

        // cout << hashKey << " " << int(depth) << " " << evaluation << endl;
        transpositionTable[hashKey] = std::make_pair(depth, evaluation);
    }

    file.close();

    return transpositionTable;
}

// TODO: needs fixing its wrong
bool kol(BitmaskBoard board_layout, char row, char col, char color){
    if(color==1){
        if(row+1<8 && (row+1 == 2 || row+1 == 4) && row-1>0 && row-1==0) return true; //3am t2elo kol forwards
        if(col+1<8 && (col+1 == 2 || col+1 == 4) && col-1>0 && col-1==0) return true; //3am t2elo kol right
        if(col-1>0 && (col-1 == 2 || col-1 == 4) && col+1<8 && col+1==0) return true; //3am t2elo kol left
    }else if (color==2){
        if(row-1>0 && (row-1 == 1 || row-1 == 3) && row+1<8 && row+1==0) return true; //3am t2elo kol forwards
        if(col+1<8 && (col+1 == 1 || col+1 == 3) && col-1>0 && col-1==0) return true; //3am t2elo kol right
        if(col-1>0 && (col-1 == 1 || col-1 == 3) && col+1<8 && col+1==0) return true; //3am t2elo kol left
    }
    return false;
}

pair<vector<char*>, char> get_valid_moves(char row,char col, char color, char streak, vector<char*> valid_moves,  BitmaskBoard board_layout, char eat_direction)
{
    if(row>7 || col>7 || row<0 || col<0)
    {
        pair<vector<char*>, char> validMovesAndEatingPiece;
        validMovesAndEatingPiece.first = {};
        validMovesAndEatingPiece.second = 0;
        return validMovesAndEatingPiece;
    }
    bool ate_up = (eat_direction == 'u');
    bool ate_down = (eat_direction == 'd');
    bool ate_left = (eat_direction == 'l');
    bool ate_right = (eat_direction == 'r');

    char *v_move= new char[2];

    char eating_piece=0;
    
    //black dama
    if(color == 3)
    {
        //right
        char i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
                break;
            if (col+i<8 && board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
            {
                char pieceToEat = board_layout.get(row, col+i);
                char j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout.get(row, col+i+j) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col+i+j;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }

        //left
        i=1;
        while(true && !ate_right)
        {
            if (col-i < 0 || col>7 || row>7)
                break;
            if (board_layout.check_index_has_blackKing(row, col-i) || board_layout.check_index_has_blackPawn(row, col-i))
                break;
            if (board_layout.check_index_has_whiteKing(row, col-i) || board_layout.check_index_has_whitePawn(row, col-i))
            {
                char pieceToEat = board_layout.get(row, col-i);
                char j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row, col-i-j) || board_layout.check_index_has_whitePawn(row, col-i-j) || board_layout.check_index_has_blackKing(row, col-i-j) || board_layout.check_index_has_blackPawn(row, col-i-j))
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col-i-j) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col-i-j;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        i=1; //down
        while(true && !ate_up)
        {
            if (row+i>=8 || col>=8 || row+i<0 || col<0)
                break;
            if (board_layout.check_index_has_blackKing(row+i, col) || board_layout.check_index_has_blackPawn(row+i, col))
                break;
            if (board_layout.check_index_has_whiteKing(row+i, col) || board_layout.check_index_has_whitePawn(row+i, col))
            {
                char pieceToEat = board_layout.get(row+i, col);
                char j=1;
                while(true)
                {

                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row+i+j, col) || board_layout.check_index_has_whitePawn(row+i+j, col) || board_layout.check_index_has_blackKing(row+i+j, col) || board_layout.check_index_has_blackPawn(row+i+j, col))
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row+i+j, col) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row+i+j;
                        v_move[1] = col;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
            
        i=1; //up
        while(true && !ate_down)
        {
            if (row-i<0)
                break;
            if (board_layout.check_index_has_blackKing(row-i, col) || board_layout.check_index_has_blackPawn(row-i, col))
                break;
            if (board_layout.check_index_has_whiteKing(row-i, col) || board_layout.check_index_has_whitePawn(row-i, col))
            {
                char pieceToEat = board_layout.get(row-i, col);
                char j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.check_index_has_whiteKing(row-i-j, col) || board_layout.check_index_has_whitePawn(row-i-j, col) || board_layout.check_index_has_blackKing(row-i-j, col) || board_layout.check_index_has_blackPawn(row-i-j, col))
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row-i-j, col) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row-i-j;
                        v_move[1] = col;
                        if(pieceToEat == 2)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        if (eating_piece==0)
        {
            i=1;
            while(row+i<8 && board_layout.get(row+i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row+i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout.get(row-i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row-i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(col+i<8 && board_layout.get(row, col+i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && row>0 && row<8 && col-i<8 && board_layout.get(row, col-i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-i;
                valid_moves.push_back(v_move);
                i+=1;
            }

            
        }
    }

    if(color == 4)
    {
        //right
        char i=1;
        while( true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
                break;
            if (col+i<8 && board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
            {
                char pieceToEat = board_layout.get(row, col+i);
                char j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col+i+j) != 0)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout.get(row, col+i+j) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col+i+j;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }

        //left
        i=1;
        while( true && !ate_right)
        {
            if (col-i < 0)
                break;
            if (board_layout.check_index_has_whiteKing(row, col-i) || board_layout.check_index_has_whitePawn(row, col-i))
                break;
            if (board_layout.check_index_has_blackKing(row, col-i) || board_layout.check_index_has_blackPawn(row, col-i))
            {
                char pieceToEat = board_layout.get(row, col-i);
                char j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col-i-j)!=0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row, col-i-j) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row;
                        v_move[1] = col-i-j;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        i=1; //down
        while( true && !ate_up)
        {
            if (row+i>7)
                break;
            if (board_layout.check_index_has_whiteKing(row+i, col) || board_layout.check_index_has_whitePawn(row+i, col))
                break;
            if (board_layout.check_index_has_blackKing(row+i, col) || board_layout.check_index_has_blackPawn(row+i, col))
            {
                char pieceToEat = board_layout.get(row+i, col);
                char j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row+i+j, col)!=0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row+i+j, col) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row+i+j;
                        v_move[1] = col;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
            
        i=1; //up
        while(true && !ate_down)
        {
            if (row-i<0)
                break;
            if (board_layout.check_index_has_whiteKing(row-i, col) || board_layout.check_index_has_whitePawn(row-i, col))
                break;
            if (board_layout.check_index_has_blackKing(row-i, col) || board_layout.check_index_has_blackPawn(row-i, col))
            {
                char pieceToEat = board_layout.get(row-i, col);
                char j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row-i-j, col) != 0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout.get(row-i-j, col) == 0)
                    {
                        v_move = new char[2];
                        v_move[0] = row-i-j;
                        v_move[1] = col;
                        if(pieceToEat == 1)
                            valid_moves.push_back(v_move);
                        else
                            valid_moves.insert(valid_moves.begin(), v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        if (eating_piece==0)
        {
            i=1;
            while(row+i<8 && board_layout.get(row+i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row+i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout.get(row-i, col) == 0)
            {
                v_move = new char[2];
                v_move[0] = row-i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(col+i<8 && board_layout.get(row, col+i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && board_layout.get(row, col-i) == 0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
        }
    }

    if(color == 1)
    {
        //streak
        if (row+2<8 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)) && board_layout.get(row+2, col)==0 && streak>=0)
        {
            char pieceToEat = board_layout.get(row+1, col);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+2;
            v_move[1] = col;
            if(pieceToEat == 2)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }   

        if (col+2<8 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)) && board_layout.get(row, col+2)==0 && streak>=0 && !ate_left)
        {
            char pieceToEat = board_layout.get(row, col+1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col+2;
            if(pieceToEat == 2)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }
            

        if (col-2>=0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)) && board_layout.get(row, col-2)==0 && streak>=0 &&  !ate_right)
        {
            char pieceToEat = board_layout.get(row, col-1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col-2;
            if(pieceToEat == 2)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }
            
        //ordering of moves
        if(row == 1 || row == 2){ //right and left moves are more important
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;

                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-1;
                valid_moves.push_back(v_move);
            }
            if (row+1<8 && board_layout.get(row+1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row+1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }

            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // }
            
        }
        else{
            if (row+1<8 && board_layout.get(row+1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row+1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;
                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col-1;
                valid_moves.push_back(v_move);
            }

            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // }
            
        }
    }
    else if (color == 2)
    {
        //streak
        
        if (row-2>=0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)) && board_layout.get(row-2, col)==0 && streak>=0)
        {
            char pieceToEat = board_layout.get(row-1, col);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row-2;
            v_move[1] = col;
            if(pieceToEat == 1)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }

        if (col+2<8 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)) && board_layout.get(row, col+2)==0 && streak>=0 && !ate_left)
        {
            char pieceToEat = board_layout.get(row, col+1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col+2;
            if(pieceToEat == 1)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        } 

        if (col-2>=0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)) && board_layout.get(row, col-2)==0 && streak>=0 &&  !ate_right)
        {
            char pieceToEat = board_layout.get(row, col-1);
            v_move = new char[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col-2;
            if(pieceToEat == 1)
                valid_moves.push_back(v_move);
            else
                valid_moves.insert(valid_moves.begin(), v_move);
            eating_piece = 'n';
        }

        if(row == 6 || row == 5){ //right and left moves are more important
            if (row-1>=0 && board_layout.get(row-1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move = new char[2];
                v_move[0] = row-1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;
                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] =  col-1;
                valid_moves.push_back(v_move);
            }
            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // } 
        }
        else{
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] = col+1;
                valid_moves.push_back(v_move);
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                v_move = new char[2];
                v_move[0] = row;
                v_move[1] =  col-1;
                valid_moves.push_back(v_move);
            }
            if (row-1>=0 && board_layout.get(row-1, col)==0 && streak==0)
            {
                v_move = new char[2];
                v_move = new char[2];
                v_move[0] = row-1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                // valid_moves.insert(valid_moves.begin(), v_move);
            }
            // if(valid_moves.size() > 1)
            // {
            //     char* first_move = valid_moves[0];
            //     char* last_move = valid_moves[valid_moves.size()-1];
            //     //if kol first and not kol last switch them
            //     if(first_move != last_move && kol(board_layout, first_move[0], first_move[1], color) && !kol(board_layout, last_move[0], last_move[1], color)){
            //         //switch first and last moves
            //         valid_moves[0] = last_move;
            //         valid_moves[valid_moves.size()-1] = first_move;
            //     }
            // }
        }
    
    }
    
    pair<vector<char*>, char> validMovesAndEatingPiece;
    validMovesAndEatingPiece.first = valid_moves;
    validMovesAndEatingPiece.second = eating_piece;

    return validMovesAndEatingPiece;
}

char get_eat_direction(char row1, char col1, char row2, char col2)
{
    char eat_direction = 0;
    if (col1==col2)
    {
        if (row1>row2)
            eat_direction = 'd';
        else
            eat_direction = 'u';
    }
    else
    {
        if (col1>col2)
            eat_direction = 'r';
        else
            eat_direction = 'l';
    }
    return eat_direction;
}

pair<BitmaskBoard, char> eat_piece_if_possible(BitmaskBoard board_layout, char row, char col, char r, char c, char color)
{
    char eat_direction = 0;
    char sumRowOver2;
    char sumColOver2;
    if (color == 1 || color == 2)
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
        
        eat_direction = get_eat_direction(r, c, row, col);
    }

    if (color == 1)
    {
        if (board_layout.check_index_has_whitePawn(sumRowOver2, col) || board_layout.check_index_has_whiteKing(sumRowOver2, col))
        {
            board_layout.set(sumRowOver2, col, 0);
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout.check_index_has_whitePawn(row, sumColOver2) || board_layout.check_index_has_whiteKing(row, sumColOver2))
        {
            board_layout.set(row, sumColOver2, 0);
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 2)
    {
        if (board_layout.check_index_has_blackPawn(sumRowOver2, col) || board_layout.check_index_has_blackKing(sumRowOver2, col))
        {
            board_layout.set(sumRowOver2, col, 0);
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout.check_index_has_blackPawn(row, sumColOver2) || board_layout.check_index_has_blackKing(row, sumColOver2))
        {
            board_layout.set(row, sumColOver2, 0);
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 3)
    {
        if (c==col)
        {
            char big, small;
            if (r>row)
            {
                big = r;
                small = row;
                eat_direction = 'd';
            }
            else
            {
                big = row;
                small = r;
                eat_direction = 'u';
            }
            for(char k = small; k<big; k++)
            {
                if (board_layout.check_index_has_whitePawn(k, c) || board_layout.check_index_has_whiteKing(k, c))
                {
                    board_layout.set(k, c, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
        else if (row ==r)
        {
            char small, big;
            if (c>col)
            {
                big = c;
                small = col;
                eat_direction = 'r';
            }
            else
            {
                big = col;
                small = c;
                eat_direction = 'l';
            }
            for(char k=small; k<big; k++)
            {
                if (board_layout.check_index_has_whitePawn(r, k) || board_layout.check_index_has_whiteKing(r, k))
                {
                    board_layout.set(r, k, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }                
    else if (color == 4)
    {
        if (c==col)
        {
            char big, small;
            if (r>row)
            {
                big = r;
                small = row;
                eat_direction = 'd';
            }
            else
            {
                big = row;
                small = r;
                eat_direction = 'u';
            }
            for(char k = small; k<big; k++)
            {
                if (board_layout.check_index_has_blackPawn(k, c) || board_layout.check_index_has_blackKing(k, c))
                {
                    board_layout.set(k, c, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
        else if (row ==r)
        {
            char small, big;
            if (c>col)
            {
                big = c;
                small = col;
                eat_direction = 'r';
            }
            else
            {
                big = col;
                small = c;
                eat_direction = 'l';
            }
            for(char k=small; k<big; k++)
            {
                if (board_layout.check_index_has_blackPawn(r, k) || board_layout.check_index_has_blackKing(r, k))
                {
                    board_layout.set(r, k, 0);
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }
    return make_pair(board_layout, eat_direction);
}

bool equals(vector<char*> a, vector<char*> b)
{
    for(char i=0; i<(char)a.size(); i++)
        if(a[i][0]!=b[i][0] || a[i][1]!=b[i][1])
            return false;
    return true;
}

bool doesnt_have(vector <vector<char*>> a, vector<char*> b){
    for(auto element:a)
        if(equals(element, b))
            return false;
    return true;
}

std::vector<std::vector<char*>> eat_max2(char row, char col,BitmaskBoard board_layout,  vector<vector<char*>> parent_list, char color, char eat_direction) {
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);

    bool dama_nom = (eating_piece == 'd');

    if (dama_nom) {
        std::vector<char*> new_list;
        std::vector<std::vector<char*>> big_dama_list;
        big_dama_list.clear();
        new_list.clear();
        for (auto move : valid_moves) {
            dama_nom = false;
            
            if (move[1] == col) {
                if (move[0] > row) {
                    eat_direction = 'd';
                } else {
                    eat_direction = 'u';
                }
            } else {
                if (move[1] > col) {
                    eat_direction = 'r';
                } else {
                    eat_direction = 'l';
                }
            }
            // Call get_valid_moves function
            auto  eating_piece = get_valid_moves(move[0], move[1], color, 0, {}, board_layout, eat_direction).second;
            dama_nom = (eating_piece == 'd');
    
            if (dama_nom) {
                char* parent = new char[2];  parent[0] = row; parent[1] = col;
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (parent[0] == value.back()[0] && parent[1] == value.back()[1]) {
                        std::vector<char*> big_dama = value;
                        big_dama.push_back(move);
                        big_dama_list.push_back(big_dama);
                    }
                }
            }
        }
        if (!big_dama_list.empty()) {
            parent_list = big_dama_list;
        } 
        if (new_list.empty()) {
            std::vector<std::vector<char*>> aux_list2;
            aux_list2.clear();
            char counter = 0;
            for (vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[char(value.size())-2];

                char sizeLast = char(sizeof(last)/sizeof(last[0]));
                char sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

                if (last[1] == pre_last[1]) {
                    if (last[0] > pre_last[0]) {
                        eat_direction = 'd';
                    } else {
                        eat_direction = 'u';
                    }
                } else {
                    if (last[1] > pre_last[1]) {
                        eat_direction = 'r';
                    } else {
                        eat_direction = 'l';
                    }
                }
                // Call get_valid_moves function
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0], last[1], color, 0, {}, board_layout, eat_direction);

                dama_nom = (eating_piece == 'd');

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
            }
            parent_list = aux_list2;
            return parent_list;
            
        } else {
            valid_moves = new_list;
            std::vector<std::vector<char*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                BitmaskBoard board_layout2 = board_layout;
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color);
                auto p2 = eat_max2(move[0], move[1], new_board_layout, parent_list, color, eat_direction);
                
                if (maxp.empty()) {
                    maxp = p2;
                } else {
                    if (!p2.empty() && p2[0].size() > maxp[0].size()) {
                        maxp = p2;
                    } else if (!p2.empty() && p2[0].size() == maxp[0].size()) {
                        for (auto li : p2) {
                            if (doesnt_have(maxp, li)) {
                                maxp.push_back(li);
                            }
                        }
                    }
                }
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty vector
    }
}

std::vector<std::vector<char*>> eat_max2_not_dama(char row, char col,BitmaskBoard board_layout,  vector<vector<char*>> parent_list, char color, char eat_direction) {
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);
    bool dama_nom = (eating_piece != 0);
    if (dama_nom) {
        std::vector<char*> new_list;
        std::vector<std::vector<char*>> big_dama_list;
        big_dama_list.clear();
        new_list.clear();
        for (auto move : valid_moves) {
            dama_nom = false;
            
            if (move[1] == col) {
                if (move[0] > row) {
                    eat_direction = 'd';
                } else {
                    eat_direction = 'u';
                }
            } else {
                if (move[1] > col) {
                    eat_direction = 'r';
                } else {
                    eat_direction = 'l';
                }
            }
            // Call get_valid_moves function
            auto  eating_piece = get_valid_moves(move[0], move[1], color, 0, {}, board_layout, eat_direction).second;
            dama_nom = (eating_piece != 0);
    
            if (dama_nom) {
                char* parent = new char[2];  parent[0] = row; parent[1] = col;
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (parent[0] ==  value.back()[0] && parent[1] ==  value.back()[1]) {
                        std::vector<char*> big_dama = value;
                        big_dama.push_back(move);
                        big_dama_list.push_back(big_dama);
                    }
                }
            }
        }
        if (!big_dama_list.empty()) {
            parent_list = big_dama_list;
        } 
        if (new_list.empty()) {
            std::vector<std::vector<char*>> aux_list2;
            aux_list2.clear();
            char counter = 0;
            for (vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[char(value.size())-2];

                char sizeLast = char(sizeof(last)/sizeof(last[0]));
                char sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

                if (last[1] == pre_last[1]) {
                    if (last[0] > pre_last[0]) {
                        eat_direction = 'd';
                    } else {
                        eat_direction = 'u';
                    }
                } else {
                    if (last[1] > pre_last[1]) {
                        eat_direction = 'r';
                    } else {
                        eat_direction = 'l';
                    }
                }
                // Call get_valid_moves function
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0], last[1], color, 0, {}, board_layout, eat_direction);

                dama_nom = (eating_piece!=0);

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
            }
            parent_list = aux_list2;
            return parent_list;
            
        } else {
            valid_moves = new_list;
            std::vector<std::vector<char*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                BitmaskBoard board_layout2 = board_layout;
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color);
                auto p2 = eat_max2_not_dama(move[0], move[1], new_board_layout, parent_list, color, eat_direction);
                if(maxp.empty() || p2[0].size() > maxp[0].size())
                    maxp = p2;
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty vector
    }
}

bool passage_is_clear(BitmaskBoard board_layout, char row, char col, char turn) {
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

// bitwise optimized: tested
vector<char*> get_all_pieces(BitmaskBoard board_layout, char color)
{
    vector<char*> pieces = vector<char*>();
    char i, j;
    if(color == 1)
    {
        uint64_t black_pieces = board_layout.get_color_mask(1);
        for(i=7; i>=0; i--)
        {
            for(j=0; j<8; j++)
            {
                if(black_pieces & (1ULL << (i*8+j)))
                {
                    //make piece
                    char* piece = new char[2];
                    piece[0] = i;
                    piece[1] = j;
                    pieces.push_back(piece);
                }
            }
        }
    }
    else
    {
        uint64_t white_pieces = board_layout.get_color_mask(2);
        for(i=0; i<8; i++)
        {
            for(j=0; j<8; j++)
            {
                if(white_pieces & (1ULL << (i*8+j)))
                {
                    //make piece
                    char* piece = new char[2];
                    piece[0] = i;
                    piece[1] = j;
                    pieces.push_back(piece);
                }
            }
        }
    }
    return pieces;
}

pair< vector<char*>, bool > check_for_possible_capture(BitmaskBoard board_layout, char turn)
{
    vector<char*> pieces = get_all_pieces(board_layout, turn);
    for(auto piece:pieces){
        char row = piece[0];
        char col = piece[1];
        if((board_layout.check_index_has_blackKing(row, col) && turn==1) || (board_layout.check_index_has_whiteKing(row, col) && turn == 2))
        {
            char eating_piece = get_valid_moves(row, col, board_layout.get(row, col), 0, {}, board_layout, 0).second;
            if(eating_piece!=0)
                return make_pair(pieces, true);
        }
        else if(turn == 2){
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)))
                return make_pair(pieces, true);
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)))
                return make_pair(pieces, true);
            if(row-2>=0 && board_layout.get(row-2, col)==0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)))
                return make_pair(pieces, true);
        }
        else{
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)))
                return make_pair(pieces, true);
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)))
                return make_pair(pieces, true);
            if(row+2<8 && board_layout.get(row+2, col)==0 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)))
                return make_pair(pieces, true);
        }
    }
    return make_pair(pieces, false);
}

bool check_if_piece_can_capture(char row, char col, BitmaskBoard board_layout, char turn){
    if((board_layout.check_index_has_blackKing(row, col) && turn==1) || (board_layout.check_index_has_whiteKing(row, col) && turn == 2))
        {
            char eating_piece = get_valid_moves(row, col, board_layout.get(row, col), 0, {}, board_layout, 0).second;
            if(eating_piece!=0)
                return true;
        }
        else if(turn == 2){
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)))
                return true;
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)))
                return true;
            if(row-2>=0 && board_layout.get(row-2, col)==0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)))
                return true;
        }
        else{
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)))
                return true;
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)))
                return true;
            if(row+2<8 && board_layout.get(row+2, col)==0 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)))
                return true;
        }
    return false;
}

vector<char*> check_for_force(BitmaskBoard board_layout, char turn, vector<char*> *pieces = {})
{
    auto [pieces2, check] = check_for_possible_capture(board_layout, turn);
    *pieces = pieces2;
    if(check == false)
        return {};
    bool aktar_flag = false;
    vector<char*> aktar_list;
    vector<char*> force_list;
    char maxLength = 3, listSize = 0;

    for(char* piece : pieces2)
    {
        char row2=piece[0], col2=piece[1];
        if(!check_if_piece_can_capture(row2, col2, board_layout, turn))
            continue;
        char color = board_layout.get(row2, col2);
        if(((color==2 || color==4) && turn==2) || ((color==1 || color==3) && turn==1))
        {
            bool fff = false;
            vector<vector<char*>> parent_list2;
            
            //get_valid_moves stuff
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(row2, col2, color, 0, {}, board_layout, 0);
            
            char eating_piece = validMovesAndEatingPiece.second;
            bool dama_nom = eating_piece == 'd';
            fff = eating_piece!=0;

            if(fff)
            {
                vector<char*> valid_moves = validMovesAndEatingPiece.first;
                //vector<char*> parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                parent_list2 = vector<vector<char*>>();
                for(char* move : valid_moves)
                {
                    //parent_list2.append([(row2, col2), move])
                    char* temp = new char[2];
                    vector<char*> tempValue = vector<char*>();
                    temp[0] = row2; temp[1] = col2;
                    tempValue.push_back(temp);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }
                if(color == 1 || color == 2)
                {
                    vector<vector<char*>> parent_list;
                    parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                    if(!parent_list.empty())
                        listSize = parent_list[0].size();
                    if(!parent_list.empty() && listSize>=maxLength)
                    {
                        if(listSize>maxLength)
                            aktar_list.clear();
                        maxLength = listSize;
                        aktar_flag = true;
                        for(vector<char*> value : parent_list)
                            aktar_list.push_back(value[0]);
                        
                    }
                }
                else if (color == 3 || color == 4)
                {
                    vector<vector<char*>> parent_list;
                    // cout<<"parent_list2 size: "<<valid_moves.size()<<endl;
                    // for(auto value : parent_list2)
                        // cout<<value[0]<<value[1];
                    parent_list = eat_max2(row2, col2, board_layout, parent_list2, color, 0);
                    if(!parent_list.empty())
                        listSize = parent_list[0].size();
                    if(!parent_list.empty() && listSize>=maxLength)
                    {
                        if(listSize>maxLength)
                            aktar_list.clear();
                        maxLength = listSize;
                        aktar_flag = true;
                        for(vector<char*> value : parent_list)
                            aktar_list.push_back(value[0]);
                    }
                }

                if(!aktar_flag)
                {
                    char* piece = new char[2];
                    piece[0] = row2; piece[1] = col2;
                    force_list.push_back(piece);
                }
            }
        }
    }
    if(aktar_flag)
        force_list = aktar_list;
    return force_list;
}
/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

BitmaskBoard move_piece(char* piece, char* move, BitmaskBoard board_layout, vector<vector<char*>> parent_list, char color)
{
    char row = piece[0];
    char col = piece[1];

    char r = move[0];
    char c = move[1];

    char listSize = parent_list.size();
    char index=0;
    char value_length;

    if(!parent_list.empty())
    {

        for(vector<char*> value : parent_list)
        {
            value_length = value.size();
            auto temp = value[value_length-1];
            if(temp[0]==r && temp[1]==c)
            {
                char z=0;
                for(z = 0; z<value_length; z++)
                {
                    if(z+1 >= value_length) break;
                    board_layout = eat_piece_if_possible(board_layout, value[z][0], value[z][1], value[z+1][0], value[z+1][1], color).first;
                }
                break;
            }
            index++;
        }
        parent_list.erase(parent_list.begin()+index);
        if(value_length==0)
            return board_layout;
    }
    else
        board_layout = eat_piece_if_possible(board_layout, row, col, r, c, color).first;

    char aux;
    aux = board_layout.get(row, col);
    board_layout.set(row, col, board_layout.get(r, c));
    board_layout.set(r, c, aux);

    //check for dama promotion
    if(r==0 && color==2)
        board_layout.set_whiteKing(r, c);
    if(r==7 && color==1)
        board_layout.set_blackKing(r, c);
    
    return board_layout;
}

char evaluate_int2(BitmaskBoard board_layout, char turn)
{
    // auto [pieces, check] = check_for_possible_capture(board_layout, turn);
    // if(check){
    //     if(turn == 1){
    //         auto force_list = check_for_force(board_layout, 1, &pieces);
    //         if(force_list.size() == 1){
    //             auto forced_piece = force_list[0];
    //             auto [valid_moves, parent_list] = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]]);
    //             if(valid_moves.size()==1)
    //             {
    //                 auto forced_move = valid_moves[0];
    //                 board_layout = move_piece(forced_piece, forced_move, board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]]);
    //                 turn = 2;
    //             }
    //         }
    //     }else{
    //         auto force_list = check_for_force(board_layout, 2, &pieces);
    //         if(force_list.size() == 1){
    //             auto forced_piece = force_list[0];
    //             auto [valid_moves, parent_list] = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]]);
    //             if(valid_moves.size()==1)
    //             {
    //                 auto forced_move = valid_moves[0];
    //                 board_layout = move_piece(forced_piece, forced_move, board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]]);
    //                 turn = 1;
    //             }
    //         }
    //     }
    // }
    movesSeen++;
    char sum = 0, balance_black = 0, balance_red = 0, all_pieces=0, red_pieces=0, black_pieces = 0;
    for(char i=0; i<8; i++)
        for(char j=0; j<8; j++)
        {
            char piece = board_layout.get(i, j);

            if(piece == 1)
            {
                black_pieces+=1;
                if(j==0 || j==7)
                    sum+=7;
                //spreading pieces
                if(j<=3)
                    balance_black+=1;
                else
                    balance_black-=1;

                sum+=100; //piece = 1 pnt

                sum+=i; //the hiegher the better

                if(i == 6)
                {
                    sum+=150;
                    if(turn == 1)
                        sum+=40;
                }
                if(i==5)
                    sum+=40;
                if(i==4 && turn ==1)
                    sum+=30;
                    if(j==0 || j==7)
                        sum+=10;

                //4 important squares
                if(j==0 or j==7)
                {
                    if (i == 5 and turn == 1)
                        sum+=80;
                    if (i == 5 and turn == 2)
                        sum+=20;
                    if (i == 6 and turn == 1)
                        sum+=50;
                }

                //if on 5th row and passage is clear the piece is dangerous
                if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
                    sum+=70;
                
            }
            else if(piece == 2)
            {

                red_pieces+=1;
                if(j==0 || j==7)
                    sum-=7;
                //spreading pieces
                if(j<=3)
                    balance_red+=1;
                else
                    balance_red-=1;

                sum-=100; //piece = 1 pnt

                sum-=(7-i); //the hiegher the better

                if(i == 1)
                {
                    sum-=150;
                    if(turn == 2)
                        sum-=40;
                }
                if(i==2)
                    sum-=40;
                if(i==3 && turn ==2)
                    sum-=30;
                    if(j==0 || j==7)
                        sum-=10;

                //4 important squares
                if(j==0 or j==7)
                {
                    if (i == 2 and turn == 2)
                        sum-=80;
                    if (i == 2 and turn == 1)
                        sum-=20;
                    if (i == 1 and turn == 2)
                        sum-=50;
                }

                //if on 5th row and passage is clear the piece is dangerous
                if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
                    sum-=70;
            }
            else if(piece==3)
            {
                black_pieces+=1;
                sum+=450;
            }
            else if(piece==4)
            {
                red_pieces+=1;
                sum-=450;
            }

        }
    
    sum = sum - 100*abs(balance_black)/20 + 100*abs(balance_red)/20;

    all_pieces = black_pieces + red_pieces;

    if (red_pieces>black_pieces)
        sum += all_pieces/2;
    else if (black_pieces>red_pieces)
        sum -= all_pieces/2;

    if (black_pieces==1 and red_pieces==1)
        sum = 0;

    return sum;
}

int count_all_pieces(BitmaskBoard board_layout, char turn)
{
    int sum = 0;
    for(char i=0; i<8; i++)
        for(char j=0; j<8; j++)
        {
            char piece = board_layout.get(i, j);
            if(turn == 1){
                if(piece == 1 || piece == 3)
                    sum+=1;
            }
            else{
                if(piece == 2 || piece == 4)
                    sum+=1;
            }
        }
    return sum;
}

int evaluate_int(BitmaskBoard board_layout, char turn)
{
    movesSeen++;
    return board_layout.evaluate_board();

    // int sum = 0, balance_black = 0, balance_red = 0, all_pieces=0, red_pieces=0, black_pieces = 0;
    // int all_pieces_red = count_all_pieces(board_layout, 2);
    // int all_pieces_black = count_all_pieces(board_layout, 1);
    // int red_pawns = 0, black_pawns = 0;
    // int red_damas = 0, black_damas = 0;
    // for(char i=0; i<8; i++)
    //     for(char j=0; j<8; j++)
    //     {
    //         char piece = board_layout.get(i, j);
    //         if(piece == 0)
    //             continue;

    //         if(false && all_pieces_red + all_pieces_black <=9) //end game
    //         {
    //             if(piece == 1)
    //             {
    //                 black_pieces+=1;
    //                 black_pawns+=1;
    //                 sum+=100; //piece = 1 pnt
    //                 sum+=i; //the hiegher the better

    //                 if(i == 6)
    //                 {
    //                     sum+=150;
    //                     if(turn == 1)
    //                         sum+=40;
    //                 }
    //                 if(i==5)
    //                     sum+=40;
    //                 if(i==4 && turn ==1)
    //                     sum+=30;
    //                     if(j==0 || j==7)
    //                         sum+=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 5 and turn == 1)
    //                         sum+=80;
    //                     if (i == 5 and turn == 2)
    //                         sum+=20;
    //                     if (i == 6 and turn == 1)
    //                         sum+=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
    //                     sum+=70;
                    
    //             }
    //             else if(piece == 2)
    //             {
    //                 red_pawns+=1;
    //                 red_pieces+=1;
    //                 sum-=100; //piece = 1 pnt
    //                 sum-=(7-i); //the hiegher the better

    //                 if(i == 1)
    //                 {
    //                     sum-=150;
    //                     if(turn == 2)
    //                         sum-=40;
    //                 }
    //                 if(i==2)
    //                     sum-=40;
    //                 if(i==3 && turn ==2)
    //                     sum-=30;
    //                     if(j==0 || j==7)
    //                         sum-=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 2 and turn == 2)
    //                         sum-=80;
    //                     if (i == 2 and turn == 1)
    //                         sum-=20;
    //                     if (i == 1 and turn == 2)
    //                         sum-=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
    //                     sum-=70;
    //             }
    //             else if(piece==3)
    //             {
    //                 black_damas+=1;
    //                 black_pieces+=1;
    //                 sum+=350;
    //             }
    //             else if(piece==4)
    //             {
    //                 red_damas+=1;
    //                 red_pieces+=1;
    //                 sum-=350;
    //             }
    //         }
    //         else{
    //             if(piece == 1)
    //             {
    //                 black_pieces+=1;
    //                 if(j==0 || j==7)
    //                     sum+=7;
    //                 //spreading pieces
    //                 if(j<=3)
    //                     balance_black+=1;
    //                 else
    //                     balance_black-=1;

    //                 sum+=100; //piece = 1 pnt
    //                 sum+=i; //the hiegher the better

    //                 if(i == 6)
    //                 {
    //                     sum+=150;
    //                     if(turn == 1)
    //                         sum+=40;
    //                 }
    //                 if(i==5)
    //                     sum+=40;
    //                 if(i==4 && turn ==1)
    //                     sum+=30;
    //                     if(j==0 || j==7)
    //                         sum+=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 5 and turn == 1)
    //                         sum+=80;
    //                     if (i == 5 and turn == 2)
    //                         sum+=20;
    //                     if (i == 6 and turn == 1)
    //                         sum+=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
    //                     sum+=70;
                    
    //             }
    //             else if(piece == 2)
    //             {

    //                 red_pieces+=1;
    //                 if(j==0 || j==7)
    //                     sum-=7;
    //                 //spreading pieces
    //                 if(j<=3)
    //                     balance_red+=1;
    //                 else
    //                     balance_red-=1;

    //                 sum-=100; //piece = 1 pnt
    //                 sum-=(7-i); //the hiegher the better

    //                 if(i == 1)
    //                 {
    //                     sum-=150;
    //                     if(turn == 2)
    //                         sum-=40;
    //                 }
    //                 if(i==2)
    //                     sum-=40;
    //                 if(i==3 && turn ==2)
    //                     sum-=30;
    //                     if(j==0 || j==7)
    //                         sum-=10;

    //                 //4 important squares
    //                 if(j==0 or j==7)
    //                 {
    //                     if (i == 2 and turn == 2)
    //                         sum-=80;
    //                     if (i == 2 and turn == 1)
    //                         sum-=20;
    //                     if (i == 1 and turn == 2)
    //                         sum-=50;
    //                 }

    //                 //if on 5th row and passage is clear the piece is dangerous
    //                 if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
    //                     sum-=70;
    //             }
    //             else if(piece==3)
    //             {
    //                 black_pieces+=1;
    //                 sum+=450;
    //                 // if(i==0 && j==0) //corners in home for dama are good
    //                 // {
    //                 //     if(board_layout.check_index_has_blackPawn(1, 0) && board_layout.check_index_has_blackPawn(2, 0) && board_layout.check_index_has_blackPawn(3, 0))
    //                 //         sum+=30;
    //                 //     if(board_layout.check_index_has_blackPawn(1, 0) && board_layout.check_index_has_blackPawn(2, 0))
    //                 //         sum+=20;
    //                 // }
    //                 // if(i==0 && j==7){
    //                 //     if(board_layout.check_index_has_blackPawn(1, 7) && board_layout.check_index_has_blackPawn(2, 7) && board_layout.check_index_has_blackPawn(3, 7))
    //                 //         sum+=30;
    //                 //     if(board_layout.check_index_has_blackPawn(1, 7) && board_layout.check_index_has_blackPawn(2, 7))
    //                 //         sum+=20;
    //                 // }
    //                 // if(i!=0 && i!=7 && j!=0 && j!=7) //dama is bad in center
    //                 //     sum+=10;
    //             }
    //             else if(piece==4)
    //             {
    //                 red_pieces+=1;
    //                 sum-=450;
    //                 // if(i==7 && j==7) //corners in home for dama are good
    //                 // {
    //                 //     if(board_layout.check_index_has_whitePawn(6, 7) && board_layout.check_index_has_whitePawn(5, 7) && board_layout.check_index_has_whitePawn(4, 7))
    //                 //         sum-=10;
    //                 //     if(board_layout.check_index_has_whitePawn(6, 7) && board_layout.check_index_has_whitePawn(5, 7))
    //                 //         sum-=5;
    //                 // }
    //                 // if(i==7 && j==0){
    //                 //     if(board_layout.check_index_has_whitePawn(6, 0) && board_layout.check_index_has_whitePawn(5, 0) && board_layout.check_index_has_whitePawn(4, 0))
    //                 //         sum-=10;
    //                 //     if(board_layout.check_index_has_whitePawn(6, 0) && board_layout.check_index_has_whitePawn(5, 0))
    //                 //         sum-=5;
    //                 // }
    //                 // if(i!=0 && i!=7 && j!=0 && j!=7) //dama is bad in center
    //                 //     sum-=10;
    //             }
    //         }
    //     }
    
    // // if(all_pieces_red + all_pieces_black <= 9){

    // //     if(black_damas==2 && red_damas==1 && red_pawns<=3){
    // //         sum = 0;
    // //     }
    // //     if(red_damas==2 && black_damas==1 && black_pawns<=3){
    // //         sum = 0;
    // //     }
    // // }

    //     sum = sum - 100*abs(balance_black)/20 + 100*abs(balance_red)/20;

    //     all_pieces = black_pieces + red_pieces;

    //     if (red_pieces>black_pieces)
    //         sum += all_pieces/2;
    //     else if (black_pieces>red_pieces)
    //         sum -= all_pieces/2;
    

    // if (black_pieces==1 && red_pieces==1)
    //     sum = 0;

    // return sum;
}

pair<vector<BitmaskBoard>, bool> get_all_moves(BitmaskBoard board_layout, char color, vector<BitmaskBoard> *moves = {})
{
    vector<char *> pieces;
    counter++;
    //create force list
    vector<char *> force_list;
    vector<char *> valid_moves;
    bool forceListEmpty = false;
    vector<vector<char *>> parent_list;
    bool dama_nom = false, fff = false;

    force_list = check_for_force(board_layout, color, &pieces);

    if(!force_list.empty())
        pieces = force_list;
    else
        forceListEmpty = true;
    
    for(char* piece : pieces)
    {
        char color2 = board_layout.get(piece[0], piece[1]);
        fff = dama_nom = false;
        if(color2 == 1 || color2 == 2)
        {
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            fff = eating_piece!=0;

            if(fff)
            {
                vector<vector<char*>> parent_list2;
                for(char* move : valid_moves)
                {
                    vector<char*> tempValue = vector<char*>();
                    tempValue.push_back(piece);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }

                parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                vector<char*> aux_list;
                if(!parent_list.empty())
                {
                    for(vector<char*> value: parent_list)
                        aux_list.push_back(value[char(value.size()) - 1]);
                    valid_moves = aux_list;
                }

            }
        }

        if (color2 ==3 || color2 == 4)
        {
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            dama_nom = eating_piece=='d';

            if(dama_nom)
            {
                vector<vector<char*>> parent_list2;
                for(char* move : valid_moves)
                {
                    vector<char*> tempValue = vector<char*>();
                    tempValue.push_back(piece);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }

                parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                vector<char*> aux_list;
                if(!parent_list.empty())
                {
                    for(vector<char*> value: parent_list)
                        aux_list.push_back(value[char(value.size()) - 1]);
                    valid_moves = aux_list;
                }
            }
        }

        for(char* move : valid_moves)
        {
            // moves.push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            BitmaskBoard board_layout2 = board_layout;
            moves->push_back(move_piece(piece, move, board_layout2, parent_list, color2));
        }
    }

    return make_pair(*moves, forceListEmpty);
}

void printBoard(BitmaskBoard board_layout)
{
    for(char i=0; i<8; i++)
    {
        for(char j=0;j<8; j++)
        {
            std::cout<<int(board_layout.get(i, j))<<" ";
        }
        std::cout<<endl;
    }
}

bool playerWon(BitmaskBoard board_layout){
    bool red = false, black = false;
    for(char i=0; i<8; i++)
        for(char j=0; j<8; j++)
        {
            if(board_layout.check_index_has_blackPawn(i, j) || board_layout.check_index_has_blackKing(i, j))
                black = true;
            if(board_layout.check_index_has_whitePawn(i, j) || board_layout.check_index_has_whiteKing(i, j))
                red = true;
            if(red && black)
                return false;
        }
    return true;
}

pair<int, BitmaskBoard> normal_minimax_based(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            // cout<<"found in hash table"<<endl;
            cacheHits++;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        
        vector<char *> pieces;
        counter++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 1, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }

                }
            }

            if (color2 ==3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);
                evaluation = normal_minimax_based(depth-1, false, current_board, alpha, beta).first;
                if(evaluation>maxEval)
                {
                    maxEval = evaluation;
                    best_move = current_board;
                }

                alpha = max(alpha, maxEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        vector<char *> pieces;
        counter++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 2, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }

                }
            }

            if (color2 ==3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);
                evaluation = normal_minimax_based(depth-1, true, current_board, alpha, beta).first;
                if(evaluation<minEval)
                {
                    minEval = evaluation;
                    best_move = current_board;
                }

                beta = min(beta, minEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return make_pair(minEval, best_move);
    }
}

//function to get all moves(8x8 boards) sevaluate them using evalueate_int and sort them based on the evaluation
void sort_moves(vector<BitmaskBoard> *moves, char turn)
{
    vector<pair<int, BitmaskBoard>> moves_and_eval;
    for(BitmaskBoard move : *moves)
    {
        int eval = evaluate_int(move, turn);
        // transpositionTable[calculateHashKey(move, turn)] = std::make_pair(1, eval);
        moves_and_eval.push_back(make_pair(eval, move));
    }
    if(turn == 1)
        sort(moves_and_eval.begin(), moves_and_eval.end(), greater<pair<int, BitmaskBoard>>());
    else
        sort(moves_and_eval.begin(), moves_and_eval.end());
    moves->clear();
    for(pair<int, BitmaskBoard> move : moves_and_eval)
    {
        // cout<<move.first<<" ";
        moves->push_back(move.second);
    }
    // cout<<endl;
}

std::pair<int, BitmaskBoard> minimax_pro2_hash(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta, char akel_depth, char akel_player, char akling)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            cacheHits++;
            // cout<<"found in hash table"<<endl;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if (depth <= 0)
    {
        char turn;
        if (max_player)
            turn = 1;
        else
            turn = 2;
        int eval = evaluate_int(board_layout, turn);
        transpositionTable[hashKey] = std::make_pair(0, eval);
        return std::make_pair(eval, board_layout);
    }

    if (max_player)
    {
        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool> allandForce = get_all_moves(board_layout, 1, &all_moves);
        sort_moves(&all_moves, 1);
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth < 5)
                evaluation = minimax_pro2_hash(depth, false, move, alpha, beta, akel_depth + 1, false, true).first;
            else
            {
                if (akel_player == false && akel_depth > 2)
                    evaluation = minimax_pro2_hash(0, false, move, alpha, beta, 100, false, false).first;
                else
                    evaluation = minimax_pro2_hash(depth - 1, false, move, alpha, beta, 0, false, false).first;
            }

            if (evaluation > maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = std::max(alpha, maxEval);
            if (beta <= alpha)
                break;
        }

        transpositionTable[hashKey] = std::make_pair(depth, maxEval);

        return std::make_pair(maxEval, best_move);
    }
    else
    {
        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2, &all_moves);
        sort_moves(&all_moves, 2);
        all_moves = allandForce.first;
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth < 5)
                evaluation = minimax_pro2_hash(depth, true, move, alpha, beta, akel_depth + 1, true, true).first;
            else
            {
                if ((akel_player == true) && akel_depth > 2)
                    evaluation = minimax_pro2_hash(0, true, move, alpha, beta, 100, true, false).first;
                else
                    evaluation = minimax_pro2_hash(depth - 1, true, move, alpha, beta, 0, true, false).first;
            }

            if (evaluation < minEval)
            {
                minEval = evaluation;
                best_move = move;
            }

            beta = std::min(beta, minEval);
            if (beta <= alpha)
                break;
        }

        transpositionTable[hashKey] = std::make_pair(depth, minEval);

        return std::make_pair(minEval, best_move);
    }
}

pair<int, BitmaskBoard> minimax_pro2(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta, char akel_depth, char akel_player, char akling)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool> allandForce = get_all_moves(board_layout, 1, &all_moves);
        // sort_moves(&all_moves, 1);
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth<5)
                evaluation = minimax_pro2(depth, false, move, alpha, beta, akel_depth+1, false, true).first;
            else
            {
                if( akel_player == false && akel_depth>2)
                    evaluation = minimax_pro2(0, false, move, alpha, beta, 100, false, false).first;
                else
                    evaluation = minimax_pro2(depth-1, false, move, alpha, beta, 0, false, false).first;
            }

            if(evaluation>maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = max(alpha, maxEval);
            if(beta<=alpha)
                break;
        }
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2, &all_moves);
        // sort_moves(&all_moves, 2);
        all_moves = allandForce.first;
        bool force_list_empty = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list_empty && akel_depth<5)
                evaluation = minimax_pro2(depth, true, move, alpha, beta, akel_depth+1, true, true).first;
            else
            {
                if( (akel_player == true) && akel_depth>2)
                    evaluation = minimax_pro2(0, true, move, alpha, beta, 100, true, false).first;
                else
                    evaluation = minimax_pro2(depth-1, true, move, alpha, beta, 0, true, false).first;
            }

            if(evaluation<minEval)
            {
                minEval = evaluation;
                best_move = move;
            }

            beta = min(beta, minEval);
            if(beta<=alpha)
                break;
        }
        return make_pair(minEval, best_move);
    }
}

pair<int, BitmaskBoard> normal_minimax(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            // cout<<"found in hash table"<<endl;
            cacheHits++;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 1, &all_moves);
        bool force_list = allandForce.second;

        for (auto move : all_moves)
        {
            evaluation = normal_minimax(depth-1, false, move, alpha, beta).first;
            if(evaluation>maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = max(alpha, maxEval);
            if(beta<=alpha)
                break;

        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;
        pair<vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2, &all_moves);
        all_moves = allandForce.first;
        bool force_list = allandForce.second;

        for (auto move : all_moves)
        {
            evaluation = normal_minimax(depth-1, true, move, alpha, beta).first;
            if(evaluation<minEval)
            {
                minEval = evaluation;
                best_move = move;
            }

            beta = min(beta, minEval);
            if(beta<=alpha)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return make_pair(minEval, best_move);
    }
}

pair<int, BitmaskBoard> minimax_pro2_based(char depth, char max_player, BitmaskBoard board_layout, int alpha, int beta, char akel_player, char akel_depth, char akling)
{
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    uint64_t hashKey = calculateHashKey(board_layout, max_player ? 1 : 2);

    if (transpositionTable.count(hashKey) > 0) {
        std::pair<char, int> storedValues = transpositionTable[hashKey];
        char storedDepth = storedValues.first;
        int storedEval = storedValues.second;

        if (storedDepth >= depth)
        {
            // if(depth>4)
            //     cout<<int(depth)<<endl;
            // cout<<"found in hash table"<<endl;
            cacheHits++;
            movesSeen++;
            return std::make_pair(storedEval, board_layout);
        }
    }

    if(depth<=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        int eval = evaluate_int(board_layout, turn);
        transpositionTable[hashKey] = std::make_pair(0, eval);
        return std::make_pair(eval, board_layout);
        // return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);

        maxEval = INT_MIN;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        
        vector<char *> pieces;
        counter++;
        movesSeen++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 1, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }

                }
            }

            if (color2 ==3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);

                        // cout<<move[0]<<" "<<move[1]<<endl;
                    }

                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);

                if (!force_list.empty() && akel_depth<5)
                    evaluation = minimax_pro2_based(depth, false, current_board, alpha, beta, akel_depth+1, false, true).first;
                else
                {
                    if(akel_player == false && akel_depth>2)
                        evaluation = minimax_pro2_based(0, false, current_board, alpha, beta, 100, false, false).first;
                    else
                        evaluation = minimax_pro2_based(depth-1, false, current_board, alpha, beta, 0, false, false).first;
                }

                if(evaluation>maxEval)
                {
                    maxEval = evaluation;
                    best_move = current_board;
                }

                alpha = max(alpha, maxEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(1000);
        movesSeen++;

        minEval = INT_MAX;
        vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        vector<char *> pieces;
        counter++;
        //create force list
        vector<char *> force_list;
        vector<char *> valid_moves;
        bool forceListEmpty = false;
        vector<vector<char *>> parent_list;
        bool dama_nom = false, fff = false;

        force_list = check_for_force(board_layout, 2, &pieces);

        if(!force_list.empty())
            pieces = force_list;
        else
            forceListEmpty = true;
        
        for(char* piece : pieces)
        {
            char color2 = board_layout.get(piece[0], piece[1]);
            fff = dama_nom = false;
            if(color2 == 1 || color2 == 2)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }

                }
            }

            if (color2 == 3 || color2 == 4)
            {
                pair<vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = eating_piece=='d';

                if(dama_nom)
                {
                    vector<vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        vector<char*> tempValue = vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                    }

                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }
            }
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                auto current_board = move_piece(piece, move, board_layout2, parent_list, color2);
                if (!force_list.empty() && akel_depth<5)
                    evaluation = minimax_pro2_based(depth, true, current_board, alpha, beta, akel_depth+1, true, true).first;
                else
                {
                    if( (akel_player == true) && akel_depth>2)
                        evaluation = minimax_pro2_based(0, true, current_board, alpha, beta, 100, true, false).first;
                    else
                        evaluation = minimax_pro2_based(depth-1, true, current_board, alpha, beta, 0, true, false).first;
                }

                if(evaluation<minEval)
                {
                    minEval = evaluation;
                    best_move = current_board;
                }

                beta = min(beta, minEval);
                if(beta<=alpha){
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            }
            if(exit)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return make_pair(minEval, best_move);
    }
}

/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

int main()
{
    initializeLookupTable();

    

    // sombolvsSombol(100, 8);
    double time_spent = 0.0;

    vector<vector<char*>> parent_list2;
    char a[2] = {6, 0};
    char b[2] = {4, 0};
    vector<char*> v;
    v.push_back(a);
    v.push_back(b);
    parent_list2.push_back(v);

    char* test2[8];  
    std::ifstream file("minimaxResult.txt");
    if (file.is_open()) {
        for (int i = 0; i < 8; ++i) {
            test2[i] = new char[8];
            for (int j = 0; j < 8; ++j) {
                if (!(file >> test2[i][j])) {
                    // Handle error if unable to read a character
                    std::cerr << "Error reading from file.\n";
                    return 1;
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file.\n";
        return 1;
    }

    auto test3 = test2;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            test2[i][j]-='0';
        }
    pair<int, BitmaskBoard> minimaxResult;
    

    
    BitmaskBoard curr_board;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            curr_board.set(i, j, test2[i][j]);
    //fetching TT from  file
    // transpositionTable = loadTranspositionTable("transposition_table.txt"); //(board, turn) -> (depth, evaluation)

    clock_t begin = clock();
    // for(char i=0; i<1000000; i++)
    //     deepcopy2_char(test2);
    // minimaxResult = minimax_pro2(6, true, test2, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = minimax_pro2_hash(6, true, test2, INT_MIN, INT_MAX, 0, true, false);
    minimaxResult = minimax_pro2_based(6, true, curr_board, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = normal_minimax_based(9, true, test2, INT_MIN, INT_MAX);
    // minimaxResult = normal_minimax(9, true, test2, INT_MIN, INT_MAX);

    // saveTranspositionTable(transpositionTable, "transposition_table.txt");

    //start
    clock_t end = clock();
    std::cout<<"Evaluation: "<<minimaxResult.first/100.0<<endl;
    BitmaskBoard boardResult = minimaxResult.second;
    printBoard(boardResult);
    std::cout<<"Positions Seen: "<<movesSeen<<endl;

    std::ofstream outfile("minimaxResult.txt");
    if (!outfile.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return 1;
    }

    // convert to char**
    char* curr_boardResult[8];
    for(int i=0; i<8; i++)
        curr_boardResult[i] = new char[8];
    
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            curr_boardResult[i][j] = boardResult.get(i, j);

    for(int i=0; i<8; i++)
    {
        for(int j=0;j<8; j++)
        {
            outfile << curr_boardResult[i][j]+'0'-'0';
            outfile<<" ";
        }
        outfile<<endl;
    }

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"Positions/sec: "<<movesSeen/time_spent<<endl;
    std::cout<<"get_all_moves: "<<counter<<endl;
    std::cout<<"cacheHits: "<<cacheHits<<endl;
    std::cout<<"Time: "<<time_spent<<endl<<endl;
    //end

    return 0;
}

