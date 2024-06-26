#include "moveGeneration.hpp"
#include "checking.hpp"
#include "globals.hpp"
#include "move.hpp"
#include "util.hpp"
// #include <omp.h>

std::pair<std::vector<char*>, char> get_valid_moves(char row, char col, char color, char streak, BitmaskBoard& board_layout, char eat_direction)
{
    std::vector<char* > valid_moves;

    if(row>7 || col>7 || row<0 || col<0)
    {
        std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
        validMovesAndEatingPiece.first = { };
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
            if (board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
                break;
            if (board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
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
                    if (board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    else
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
            if (col-i < 0)
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
                    else
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
            if (row+i>=8)
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
                    else
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
                    else
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
            while(row-i>=0 && board_layout.get(row-i, col) == 0)
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
            while(col-i>=0 && board_layout.get(row, col-i) == 0)
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
        while(true && !ate_left)
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
                    if(board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    else
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
        while(true && !ate_right)
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
                    if (board_layout.check_index_has_whiteKing(row, col-i-j) || board_layout.check_index_has_whitePawn(row, col-i-j) || board_layout.check_index_has_blackKing(row, col-i-j) || board_layout.check_index_has_blackPawn(row, col-i-j))
                    {
                        i=10;
                        break;
                    }
                    else
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
        while(true && !ate_up)
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
                    if(board_layout.check_index_has_whiteKing(row+i+j, col) || board_layout.check_index_has_whitePawn(row+i+j, col) || board_layout.check_index_has_blackKing(row+i+j, col) || board_layout.check_index_has_blackPawn(row+i+j, col))
                    {
                        i=10;
                        break;
                    }
                    else
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
                    if (board_layout.check_index_has_whiteKing(row-i-j, col) || board_layout.check_index_has_whitePawn(row-i-j, col) || board_layout.check_index_has_blackKing(row-i-j, col) || board_layout.check_index_has_blackPawn(row-i-j, col))
                    {
                        i=10;
                        break;
                    }
                    else
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
            while(row-i>=0 && board_layout.get(row-i, col) == 0)
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
            while(col-i>=0 && board_layout.get(row, col-i) == 0)
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
                v_move[0] = row-1;
                v_move[1] = col;
                valid_moves.push_back(v_move);
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
                v_move[0] = row-1;
                v_move[1] = col;
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
    
    std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
    validMovesAndEatingPiece.first = valid_moves;
    validMovesAndEatingPiece.second = eating_piece;

    return validMovesAndEatingPiece;
}

// pieice[row1, col1] -> move[row2, col2]
char get_eat_direction(char row1, char col1, char row2, char col2)
{
    char eat_direction = 0;
    if (col1==col2)
    {
        if (row2>row1)
            eat_direction = 'd';
        else
            eat_direction = 'u';
    }
    else
    {
        if (col2>col1)
            eat_direction = 'r';
        else
            eat_direction = 'l';
    }
    return eat_direction;
}

char get_piece_to_eat(BitmaskBoard& board_layout, char row, char col, char r, char c, char color)
{
    char sumRowOver2;
    char sumColOver2;
    if (color == 1 || color == 2)
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
    }

    if (color == 1)
    {
        if (board_layout.check_index_has_whitePawn(sumRowOver2, col) || board_layout.check_index_has_whiteKing(sumRowOver2, col))
            return board_layout.get(sumRowOver2, col);
        
        else if (board_layout.check_index_has_whitePawn(row, sumColOver2) || board_layout.check_index_has_whiteKing(row, sumColOver2))
            return board_layout.get(row, sumColOver2);
    }
    else if (color == 2)
    {
        if (board_layout.check_index_has_blackPawn(sumRowOver2, col) || board_layout.check_index_has_blackKing(sumRowOver2, col))
            return board_layout.get(sumRowOver2, col);
        else if (board_layout.check_index_has_blackPawn(row, sumColOver2) || board_layout.check_index_has_blackKing(row, sumColOver2))
            return board_layout.get(row, sumColOver2);
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
            }
            else
            {
                big = row;
                small = r;
            }
            for(char k = small; k<big; k++)
            {
                if (board_layout.check_index_has_whitePawn(k, c) || board_layout.check_index_has_whiteKing(k, c))
                    return board_layout.get(k, c);
            }
        }
        else if (row ==r)
        {
            char small, big;
            if (c>col)
            {
                big = c;
                small = col;
            }
            else
            {
                big = col;
                small = c;
            }
            for(char k=small; k<big; k++)
            {
                if (board_layout.check_index_has_whitePawn(r, k) || board_layout.check_index_has_whiteKing(r, k))
                    return board_layout.get(r, k);
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
            }
            else
            {
                big = row;
                small = r;
            }
            for(char k = small; k<big; k++)
            {
                if (board_layout.check_index_has_blackPawn(k, c) || board_layout.check_index_has_blackKing(k, c))
                    return board_layout.get(k, c);
            }
        }
        else if (row ==r)
        {
            char small, big;
            if (c>col)
            {
                big = c;
                small = col;
            }
            else
            {
                big = col;
                small = c;
            }
            for(char k=small; k<big; k++)
            {
                if (board_layout.check_index_has_blackPawn(r, k) || board_layout.check_index_has_blackKing(r, k))
                    return board_layout.get(r, k);
            }
        }
    }
    return 0;
}

char eat_piece_if_possible(BitmaskBoard& board_layout, char row, char col, char r, char c, char color)
{
    char eat_direction = 0;
    char sumRowOver2;
    char sumColOver2;

    // Optimization: if row,col is adjacent to r,c, then no need to check for eating
    // if (abs(row-r) == 1 && abs(col-c) == 0)
    //     return eat_direction;
    // if (abs(row-r) == 0 && abs(col-c) == 1)
    //     return eat_direction;

    if (color == 1 || color == 2)
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
        
        eat_direction = get_eat_direction(row, col, r, c);
    }

    if (color == 1)
    {
        if (eat_direction == 'd' && board_layout.check_index_has_whitePawn(sumRowOver2, col) || board_layout.check_index_has_whiteKing(sumRowOver2, col))
        {
            board_layout.clearPosition(sumRowOver2, col);
            return eat_direction;
        }
        else if (board_layout.check_index_has_whitePawn(row, sumColOver2) || board_layout.check_index_has_whiteKing(row, sumColOver2))
        {
            board_layout.clearPosition(row, sumColOver2);
            return eat_direction;
        }
    }
    else if (color == 2)
    {
        if (eat_direction == 'u' && board_layout.check_index_has_blackPawn(sumRowOver2, col) || board_layout.check_index_has_blackKing(sumRowOver2, col))
        {
            board_layout.clearPosition(sumRowOver2, col);
            return eat_direction;
        }
        else if (board_layout.check_index_has_blackPawn(row, sumColOver2) || board_layout.check_index_has_blackKing(row, sumColOver2))
        {
            board_layout.clearPosition(row, sumColOver2);
            return eat_direction;
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
                    board_layout.clearPosition(k, c);
                    return eat_direction;
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
                    board_layout.clearPosition(r, k);
                    return eat_direction;
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
                    board_layout.clearPosition(k, c);
                    return eat_direction;
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
                    board_layout.clearPosition(r, k);
                    return eat_direction;
                }
            }
        }
    }
    return eat_direction;
}

std::vector<std::vector<char*>> eat_max2(char row, char col, BitmaskBoard& board_layout,  std::vector<std::vector<char*>> parent_list, char color, char eat_direction) {
    if(color == 1 || color == 2)
        eat_direction = 0; // no need for eat direction for normal pieces
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, board_layout, eat_direction);
    
    bool dama_nom = (eating_piece != 0);

    if(dama_nom) 
    {
        std::vector<std::vector<char*>> updated_parent_list;
        // create a new list where only the moves that have a next akel
        std::vector<char*> updated_valid_moves;
        for(auto &move : valid_moves)
        {
            if(color == 3 || color == 4) //dama
            {
                eat_direction = get_eat_direction(row, col, move[0], move[1]);
                dama_nom = check_dama_has_akel(move[0], move[1], color, board_layout, eat_direction);
            }
            else if (color == 1 || color == 2){
                char turn = (color == 1 || color == 3) ? 1 : 2;
                dama_nom = check_if_piece_can_capture(move[0], move[1], board_layout, turn);
            }
            if(dama_nom)
            {
                updated_valid_moves.push_back(move);
                // update the parent list
                for (auto &list : parent_list) {
                    if (row == list.back()[0] && col == list.back()[1]) {
                        std::vector<char*> updated_list(list);
                        updated_list.push_back(move);
                        // if(doesnt_have(updated_parent_list, updated_list))
                            // updated_parent_list.push_back(updated_list);
                        updated_parent_list.push_back(updated_list);
                    }
                }
            }
        }

        if(updated_parent_list.empty())
        {
            for(auto &move : valid_moves)
            {
                // update the parent list
                for (auto &list : parent_list) {
                    if ((row == list.back()[0]) && (col == list.back()[1])) {
                        std::vector<char*> updated_list(list);
                        updated_list.push_back(move);
                        // if(doesnt_have(updated_parent_list, updated_list))
                            // updated_parent_list.push_back(updated_list);
                        updated_parent_list.push_back(updated_list);
                    }
                }
            }
            if(updated_parent_list.empty())
                std::cout<<"Error in eat_max2"<<std::endl;

            if(updated_parent_list.empty())
                return parent_list;
            
            return updated_parent_list;
        }

        // recursively run eat_max2 with new parent list and new board for each move
        std::vector<std::vector<char*>> maxp;
        for (auto &move : updated_valid_moves) {
            BitmaskBoard board_layout2 = board_layout;

            // remove the piece
            eat_direction = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color);
            board_layout2.clearPosition(row, col); // added
            board_layout2.set(move[0], move[1], color);


            if(color == 1 || color == 2)
                eat_direction = 0; // no need for eat direction for normal pieces
            
            std::vector<std::vector<char*>> p2 = eat_max2(move[0], move[1], board_layout2, updated_parent_list, color, eat_direction);
            
            if (maxp.empty())
                maxp = p2;
            else {
                if (!p2.empty() && p2[0].size() > maxp[0].size()) {
                    maxp = p2;
                } else if (!p2.empty() && (p2[0].size() == maxp[0].size())) { 
                    for (auto li : p2) 
                        maxp.push_back(li);
                }
            }
        }
        if(maxp.empty())
            std::cout<<"Error in eat_max2"<<std::endl;
        return maxp; // return the maximum parent list
    }
    else
    {   
        // if(parent_list.empty())
        //     std::cout<<"Error in eat_max2"<<std::endl;
        return parent_list; // the current list is the correct final one, return it
    }

    std::cout<<"Error in eat_max2"<<std::endl;
    return {};
}

std::vector<char*> get_all_pieces(BitmaskBoard& board_layout, char color)
{
    std::vector<char*> pieces = std::vector<char*>();
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

void move_piece(char* piece, char* move, BitmaskBoard& board_layout, std::vector<std::vector<char*>>& parent_list, char color)
{
    char row = piece[0];
    char col = piece[1];

    char r = move[0];
    char c = move[1];

    char listSize = parent_list.size();
    char index=0;
    char value_length=0;

    if(!parent_list.empty())
    {
        for(std::vector<char*> value : parent_list)
        {
            value_length = value.size();
            auto end_list = value[value_length-1];
            auto start_list = value[0];
            if(end_list[0]==r && end_list[1]==c && start_list[0]==row && start_list[1]==col)
            {
                char z=0;
                for(z = 0; z<value_length - 1; z++)
                    eat_piece_if_possible(board_layout, value[z][0], value[z][1], value[z+1][0], value[z+1][1], color);
                break;
            }
            index++;
        }
        // parent_list.erase(parent_list.begin()+index);
        if(value_length==0)  
            return;
    }
    else
        eat_piece_if_possible(board_layout, row, col, r, c, color);

    char aux;
    aux = board_layout.get(row, col);

    if(aux == 1 && r!=0)
        board_layout.set_blackPawn(r, c);
    else if(aux == 2 && r!=7)
        board_layout.set_whitePawn(r, c);
    else if(aux == 3)
        board_layout.set_blackKing(r, c);
    else if(aux == 4)
        board_layout.set_whiteKing(r, c);

    board_layout.clearPosition(row, col);

    //check for dama promotion
    if(r==0 && aux==2)
        board_layout.set_whiteKing(r, c);
    if(r==7 && aux==1)
        board_layout.set_blackKing(r, c);
    
}

std::vector<BitmaskBoard> get_all_moves2(BitmaskBoard& board_layout, char color, bool& isEmptyForceList)
{
    std::vector<BitmaskBoard> moves;
    // robin_hood::unordered_set<BitmaskBoard> moves_set;
    std::vector<char *> pieces;
    counter++;
    //create force list
    std::vector<char *> force_list;
    std::vector<char *> valid_moves;
    std::vector<std::vector<char *>> parent_list;
    bool dama_nom = false, fff = false;
    
    isEmptyForceList = false;

    if(board_layout.getBlackKings() == 0 && board_layout.getWhiteKings() == 0 && !board_layout.capture_available(color == 1 ? 0:1)) // if no king on board
        pieces = get_all_pieces(board_layout, color);
    else
        force_list = check_for_force(board_layout, color, pieces, parent_list);
    
    if(!force_list.empty())
        pieces = force_list;
    else
        isEmptyForceList = true;
    

    for(char* piece : pieces)
    {
        char color2 = board_layout.get(piece[0], piece[1]);
        fff = dama_nom = false;
        if(color2 == 1 || color2 == 2)
        {
            std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            fff = (eating_piece!=0);

            if(fff)
            {
                bool fff2 = false;
                std::vector<std::vector<char*>> parent_list2;
                std::vector<char*> tempValue;
                tempValue.push_back(piece);
                parent_list2.push_back(tempValue);
                for(char* move : valid_moves)
                {
                    // std::vector<char*> tempValue = std::vector<char*>();
                    // tempValue.push_back(piece);
                    // tempValue.push_back(move);
                    // parent_list2.push_back(tempValue);
                    // auto eat_direction = get_eat_direction(piece[0], piece[1], move[0], move[1]);

                    if(check_if_piece_can_capture(move[0], move[1], board_layout, color))
                        fff2 = true;
                }

                if(fff2)
                {
                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    std::vector<char*> aux_list;
                    if(!parent_list.empty())
                    {
                        for(std::vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }

            }
        }

        if (color2 == 3 || color2 == 4)
        {
            std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            dama_nom = (eating_piece=='d');

            if(dama_nom)
            {
                bool dama_nom2 = false;
                std::vector<std::vector<char*>> parent_list2;
                std::vector<char*> tempList;
                tempList.push_back(piece);
                parent_list2.push_back(tempList);
                for(char* move : valid_moves)
                {
                    // std::vector<char*> tempValue = std::vector<char*>();
                    // tempValue.push_back(piece);
                    // tempValue.push_back(move);
                    // parent_list2.push_back(tempValue);

                    auto eat_direction = get_eat_direction(piece[0], piece[1], move[0], move[1]);

                    if(check_dama_has_akel(move[0], move[1], color2, board_layout, eat_direction))
                        dama_nom2 = true;
                    // cout<<move[0]<<" "<<move[1]<<"\n";
                }
                if(dama_nom2)
                {
                    parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                    std::vector<char*> aux_list;

                    if(!parent_list.empty())
                    {
                        for(std::vector<char*> value: parent_list)
                            aux_list.push_back(value[char(value.size()) - 1]);
                        valid_moves = aux_list;
                    }
                }   
            }
        }
        
        for(char* move : valid_moves)
        {
            // std::cout<<int(piece[0])<<" "<<int(piece[1])<<" "<<int(move[0])<<" "<<int(move[1])<<"\n";

            printBoard(board_layout);
            std::cout<<std::endl;


            BitmaskBoard board_layout2 = board_layout;
            move_piece(piece, move, board_layout2, parent_list, color2);
            // if(moves_set.find(board_layout2) == moves_set.end())
            // {
            //     moves_set.insert(board_layout2);
            //     moves.push_back(board_layout2);
            // }
            moves.push_back(board_layout2);

            printBoard(board_layout2);
            std::cout<<std::endl;

            Sleep(1000);
        }
    }

    return moves;
}

std::vector<BitmaskBoard> get_all_moves(BitmaskBoard& board_layout, char color, bool& isEmptyForceList)
{
    std::vector<BitmaskBoard> moves;
    std::vector<char *> pieces;
    counter++;
    //create force list
    std::vector<char *> force_list;
    std::vector<char *> valid_moves;
    std::vector<std::vector<char *>> parent_list;
    bool dama_nom = false, fff = false;
    
    isEmptyForceList = false;

    // if(board_layout.getBlackKings() == 0 && board_layout.getWhiteKings() == 0 && !board_layout.capture_available(color == 1 ? 0:1)) // if no king on board
    //     pieces = get_all_pieces(board_layout, color);
    // else
        force_list = check_for_force(board_layout, color, pieces, parent_list);
    
    // if(!force_list.empty())
    //     pieces = force_list;
    // else
    //     isEmptyForceList = true;
    if(force_list.empty())
        isEmptyForceList = true;

    
    if(!isEmptyForceList) {
        for(auto list : parent_list)
        {
            BitmaskBoard board_layout2 = board_layout;
            move_piece(list[0], list[list.size()-1], board_layout2, parent_list, board_layout.get(list[0][0], list[0][1]));
            moves.push_back(board_layout2);
        }
        return moves;
    }
    
    // parent_list.clear();
    // no force moves
    for(char* piece : pieces)
    {
        char color2 = board_layout.get(piece[0], piece[1]);
        if(color2 == 1 || color2 == 2)
        {
            std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
            valid_moves = validMovesAndEatingPiece.first;
        }

        if (color2 == 3 || color2 == 4)
        {
            std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
            valid_moves = validMovesAndEatingPiece.first;
        }
        
        for(char* move : valid_moves)
        {
            BitmaskBoard board_layout2 = board_layout;
            move_piece(piece, move, board_layout2, parent_list, color2);
            moves.push_back(board_layout2);
        }
    }
    return moves;
}







// new moveGeneratio with Piece and Move classes
std::pair<std::vector<Move>, char> get_valid_moves2(Piece piece, char streak, BitmaskBoard& board_layout, char eat_direction)
{
    std::vector<Move> valid_moves;

    char row = piece.getRow();
    char col = piece.getCol();
    char color = board_layout.get(row, col);

    if(row>7 || col>7 || row<0 || col<0)
    {
        std::cout<<"moving out of bounds\n";
        std::pair<std::vector<Move>, char> validMovesAndEatingPiece;
        validMovesAndEatingPiece.second = 0;
        return validMovesAndEatingPiece;
    }

    bool ate_up = (eat_direction == 'u');
    bool ate_down = (eat_direction == 'd');
    bool ate_left = (eat_direction == 'l');
    bool ate_right = (eat_direction == 'r');

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
            if (board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
                break;
            if (board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
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
                    if (board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        if(pieceToEat == 2)
                            valid_moves.push_back(Move(row, col, row, col+i+j));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row, col+i+j));
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
            if (col-i < 0)
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
                    else
                    {
                        if(pieceToEat == 2)
                            valid_moves.push_back(Move(row, col, row, col-i-j));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row, col-i-j));
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
            if (row+i>=8)
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
                    else
                    {
                        if(pieceToEat == 2)
                            valid_moves.push_back(Move(row, col, row+i+j, col));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row+i+j, col));
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
                    else
                    {
                        if(pieceToEat == 2)
                            valid_moves.push_back(Move(row, col, row-i-j, col));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row-i-j, col));
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
                valid_moves.push_back(Move(row, col, row+i, col));
                i+=1;
            }
            
            i=1;
            while(row-i>=0 && board_layout.get(row-i, col) == 0)
            {
                valid_moves.push_back(Move(row, col, row-i, col));
                i+=1;
            }

            i=1;
            while(col+i<8 && board_layout.get(row, col+i) == 0)
            {
                valid_moves.push_back(Move(row, col, row, col+i));
                i+=1;
            }
            
            i=1;
            while(col-i>=0 && board_layout.get(row, col-i) == 0)
            {
                valid_moves.push_back(Move(row, col, row, col-i));
                i+=1;
            }

            
        }
    }

    if(color == 4)
    {
        //right
        char i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (board_layout.check_index_has_whiteKing(row, col+i) || board_layout.check_index_has_whitePawn(row, col+i))
                break;
            if (board_layout.check_index_has_blackKing(row, col+i) || board_layout.check_index_has_blackPawn(row, col+i))
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
                    if(board_layout.check_index_has_whiteKing(row, col+i+j) || board_layout.check_index_has_whitePawn(row, col+i+j) || board_layout.check_index_has_blackKing(row, col+i+j) || board_layout.check_index_has_blackPawn(row, col+i+j))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        if(pieceToEat == 1)
                            valid_moves.push_back(Move(row, col, row, col+i+j));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row, col+i+j));
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
                    if (board_layout.check_index_has_whiteKing(row, col-i-j) || board_layout.check_index_has_whitePawn(row, col-i-j) || board_layout.check_index_has_blackKing(row, col-i-j) || board_layout.check_index_has_blackPawn(row, col-i-j))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        if(pieceToEat == 1)
                            valid_moves.push_back(Move(row, col, row, col-i-j));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row, col-i-j));
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
                    if(board_layout.check_index_has_whiteKing(row+i+j, col) || board_layout.check_index_has_whitePawn(row+i+j, col) || board_layout.check_index_has_blackKing(row+i+j, col) || board_layout.check_index_has_blackPawn(row+i+j, col))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        if(pieceToEat == 1)
                            valid_moves.push_back(Move(row, col, row+i+j, col));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row+i+j, col));
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
                    if (board_layout.check_index_has_whiteKing(row-i-j, col) || board_layout.check_index_has_whitePawn(row-i-j, col) || board_layout.check_index_has_blackKing(row-i-j, col) || board_layout.check_index_has_blackPawn(row-i-j, col))
                    {
                        i=10;
                        break;
                    }
                    else
                    {
                        if(pieceToEat == 1)
                            valid_moves.push_back(Move(row, col, row-i-j, col));
                        else
                            valid_moves.insert(valid_moves.begin(), Move(row, col, row-i-j, col));
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
                valid_moves.push_back(Move(row, col, row+i, col));
                i+=1;
            }
            
            i=1;
            while(row-i>=0 && board_layout.get(row-i, col) == 0)
            {
                valid_moves.push_back(Move(row, col, row-i, col));
                i+=1;
            }

            i=1;
            while(col+i<8 && board_layout.get(row, col+i) == 0)
            {
                valid_moves.push_back( Move(row, col, row, col+i));
                i+=1;
            }
            
            i=1;
            while(col-i>=0 && board_layout.get(row, col-i) == 0)
            {
                valid_moves.push_back(Move(row, col, row, col-i));
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
            streak+=1;
            if(pieceToEat == 2)
                valid_moves.push_back(Move(row, col, row+2, col));
            else
                valid_moves.insert(valid_moves.begin(), Move(row, col, row+2, col));
            eating_piece = 'n';
        }   

        if (col+2<8 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)) && board_layout.get(row, col+2)==0 && streak>=0 && !ate_left)
        {
            char pieceToEat = board_layout.get(row, col+1);
            streak+=1;
            if(pieceToEat == 2)
                valid_moves.push_back(Move(row, col, row, col+2));
            else
                valid_moves.insert(valid_moves.begin(), Move(row, col, row, col+2));
            eating_piece = 'n';
        }
            
        if (col-2>=0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)) && board_layout.get(row, col-2)==0 && streak>=0 &&  !ate_right)
        {
            char pieceToEat = board_layout.get(row, col-1);
            streak+=1;
            if(pieceToEat == 2)
                valid_moves.push_back(Move(row, col, row, col-2));
            else
                valid_moves.insert(valid_moves.begin(), Move(row, col, row, col-2));
            eating_piece = 'n';
        }
            
        //ordering of moves
        if(row == 1 || row == 2){ //right and left moves are more important
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col+1));
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col-1));
            }
            if (row+1<8 && board_layout.get(row+1, col)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row+1, col));
            }
            
        }
        else{
            if (row+1<8 && board_layout.get(row+1, col)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row+1, col));
            }
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col+1));
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col-1));
            }  
        }
    }
    else if (color == 2)
    {
        //streak
        
        if (row-2>=0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)) && board_layout.get(row-2, col)==0 && streak>=0)
        {
            char pieceToEat = board_layout.get(row-1, col);
            streak+=1;
            if(pieceToEat == 1)
                valid_moves.push_back(Move(row, col, row-2, col));
            else
                valid_moves.insert(valid_moves.begin(), Move(row, col, row-2, col));
            eating_piece = 'n';
        }

        if (col+2<8 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)) && board_layout.get(row, col+2)==0 && streak>=0 && !ate_left)
        {
            char pieceToEat = board_layout.get(row, col+1);
            streak+=1;
            if(pieceToEat == 1)
                valid_moves.push_back(Move(row, col, row, col+2));
            else
                valid_moves.insert(valid_moves.begin(), Move(row, col, row, col+2));
            eating_piece = 'n';
        } 

        if (col-2>=0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)) && board_layout.get(row, col-2)==0 && streak>=0 &&  !ate_right)
        {
            char pieceToEat = board_layout.get(row, col-1);
            streak+=1;
            if(pieceToEat == 1)
                valid_moves.push_back(Move(row, col, row, col-2));
            else
                valid_moves.insert(valid_moves.begin(), Move(row, col, row, col-2));
            eating_piece = 'n';
        }

        if(row == 6 || row == 5){ //right and left moves are more important
            if (row-1>=0 && board_layout.get(row-1, col)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row-1, col));
            }
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col+1));
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col-1));
            }
        }
        else{
            if (col+1<8 && board_layout.get(row, col+1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col+1));
            }
            if (col-1>=0 && board_layout.get(row, col-1)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row, col-1));
            }
            if (row-1>=0 && board_layout.get(row-1, col)==0 && streak==0)
            {
                valid_moves.push_back(Move(row, col, row-1, col));
            }
        }
    
    }
    
    std::pair<std::vector<Move>, char> validMovesAndEatingPiece;
    validMovesAndEatingPiece.first = valid_moves;
    validMovesAndEatingPiece.second = eating_piece;

    return validMovesAndEatingPiece;
}

void move_piece2(Move move, BitmaskBoard& board_layout, std::vector<std::vector<Piece>>& parent_list, char color)
{
    char row = move.getFromRow();
    char col = move.getFromCol();

    char r = move.getToRow();
    char c = move.getToCol();

    char listSize = parent_list.size();
    char index=0;
    char value_length=0;

    if(!parent_list.empty())
    {
        for(std::vector<Piece> value : parent_list)
        {
            value_length = value.size();
            auto end_list = value[value_length-1];
            auto start_list = value[0];
            if(end_list.getRow() == r && end_list.getCol()==c && start_list.getRow()==row && start_list.getCol()==col)
            {
                char z=0;
                for(z = 0; z<value_length - 1; z++)
                    eat_piece_if_possible(board_layout, value[z].getRow(), value[z].getCol(), value[z+1].getRow(), value[z+1].getCol(), color);
                break;
            }
            index++;
        }
        parent_list.erase(parent_list.begin()+index); // remove this index so that later move with same src dst take different path in parent list
        // if(value_length==0)  
        //     return;
    }
    // else
        // eat_piece_if_possible(board_layout, row, col, r, c, color);

    char aux;
    aux = board_layout.get(row, col);

    if(aux == 1 && r!=0)
        board_layout.set_blackPawn(r, c);
    else if(aux == 2 && r!=7)
        board_layout.set_whitePawn(r, c);
    else if(aux == 3)
        board_layout.set_blackKing(r, c);
    else if(aux == 4)
        board_layout.set_whiteKing(r, c);

    board_layout.clearPosition(row, col);

    //check for dama promotion
    if(r==0 && aux==2)
        board_layout.set_whiteKing(r, c);
    if(r==7 && aux==1)
        board_layout.set_blackKing(r, c);
}

std::vector<Piece> get_all_pieces2(BitmaskBoard& board_layout, char color)
{
    std::vector<Piece> pieces;
    Piece piece;
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
                    pieces.push_back(Piece(i, j, board_layout.get(i, j)));
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
                    pieces.push_back(Piece(i, j, board_layout.get(i, j)));
                }
            }
        }
    }
    return pieces;
}

std::vector<std::vector<Piece>> eat_max3(Piece piece, BitmaskBoard& board_layout, std::vector<std::vector<Piece>> parent_list, char eat_direction, char color) {
    if(color == 1 || color == 2)
        eat_direction = 0; // no need for eat direction for normal pieces
    auto [valid_moves, eating_piece] = get_valid_moves2(piece, 0, board_layout, eat_direction);
    
    bool dama_nom = (eating_piece != 0);

    if(dama_nom) 
    {
        std::vector<std::vector<Piece>> updated_parent_list;
        // create a new list where only the moves that have a next akel
        std::vector<Move> updated_valid_moves;
        for(auto &move : valid_moves)
        {
            
            if(color == 3 || color == 4) //dama
            {
                eat_direction = get_eat_direction(move.getFromRow(), move.getFromCol(), move.getToRow(), move.getToCol());
                dama_nom = check_dama_has_akel(move.getToRow(), move.getToCol(), color, board_layout, eat_direction);
            }
            else if (color == 1 || color == 2){
                char turn = (color == 1 || color == 3) ? 1 : 2;
                dama_nom = check_if_piece_can_capture(move.getToRow(), move.getToCol(), board_layout, turn);
            }
            if(dama_nom)
            {
                updated_valid_moves.push_back(move);
                // update the parent list
                for (auto &list : parent_list) {
                    if (piece.getRow() == list.back().getRow() && piece.getCol() == list.back().getCol()) {
                        std::vector<Piece> updated_list(list);
                        updated_list.push_back(Piece(move.getToRow(), move.getToCol(), color));
                        // if(doesnt_have(updated_parent_list, updated_list))
                            // updated_parent_list.push_back(updated_list);
                        updated_parent_list.push_back(updated_list);
                    }
                }
            }
        }

        if(updated_parent_list.empty())
        {
            for(auto &move : valid_moves)
            {
                // update the parent list
                for (auto &list : parent_list) {
                    if ((piece.getRow() == list.back().getRow()) && (piece.getCol() == list.back().getCol())) {
                        std::vector<Piece> updated_list(list);
                        updated_list.push_back(Piece(move.getToRow(), move.getToCol(), color));
                        // if(doesnt_have(updated_parent_list, updated_list))
                            // updated_parent_list.push_back(updated_list);
                        updated_parent_list.push_back(updated_list);
                    }
                }
            }
            if(updated_parent_list.empty())
                std::cout<<"Error in eat_max2"<<std::endl;

            if(updated_parent_list.empty())
                return parent_list;
            
            return updated_parent_list;
        }

        // recursively run eat_max2 with new parent list and new board for each move
        std::vector<std::vector<Piece>> maxp;
        for (auto &move : updated_valid_moves) {
            BitmaskBoard board_layout2 = board_layout;

            // remove the piece
            eat_direction = eat_piece_if_possible(board_layout2, piece.getRow(), piece.getCol(), move.getToRow(), move.getToCol(), color);
            board_layout2.clearPosition(piece.getRow(), piece.getCol()); // added
            // board_layout2.set(move.getToRow(), move.getToCol(), color);

            if(color == 1)
                board_layout2.set_blackPawn(move.getToRow(), move.getToCol());
            else if(color == 2)
                board_layout2.set_whitePawn(move.getToRow(), move.getToCol());
            else if(color == 3)
                board_layout2.set_blackKing(move.getToRow(), move.getToCol());
            else if(color == 4)
                board_layout2.set_whiteKing(move.getToRow(), move.getToCol());


            if(color == 1 || color == 2)
                eat_direction = 0; // no need for eat direction for normal pieces
            
            std::vector<std::vector<Piece>> p2 = eat_max3(Piece(move.getToRow(), move.getToCol(), color), board_layout2, updated_parent_list, eat_direction, color);
            
            if (maxp.empty())
                maxp = p2;
            else {
                if (!p2.empty() && p2[0].size() > maxp[0].size()) {
                    maxp = p2;
                } else if (!p2.empty() && (p2[0].size() == maxp[0].size())) { 
                    for (auto li : p2) 
                        maxp.push_back(li);
                }
            }
        }
        if(maxp.empty())
            std::cout<<"Error in eat_max2"<<std::endl;
        return maxp; // return the maximum parent list
    }
    else
    {   
        // if(parent_list.empty())
        //     std::cout<<"Error in eat_max2"<<std::endl;
        return parent_list; // the current list is the correct final one, return it
    }

    std::cout<<"Error in eat_max2"<<std::endl;
    return {};
}

std::vector<Move> get_all_valid_moves(BitmaskBoard& board_layout, char color, bool& isEmptyForceList, std::vector<std::vector<Piece>>& main_parent_list)
{
    std::vector<Move> moves;
    std::vector<Piece> pieces;
    std::vector<Move> force_list;

    // if(board_layout.getBlackKings() == 0 && board_layout.getWhiteKings() == 0 && !board_layout.capture_available(color == 1 ? 0:1)) // if no king on board
    //     pieces = get_all_pieces2(board_layout, color);
    // else
    force_list = check_for_force2(board_layout, color, pieces, main_parent_list);
    
   
    if(!force_list.empty()) {
        isEmptyForceList = false;
        return force_list;
    }
    else 
        isEmptyForceList = true;
    
    // no force moves
    for(auto piece : pieces)
    {
        std::pair<std::vector<Move>, char> validMovesAndEatingPiece;
        validMovesAndEatingPiece  = get_valid_moves2(piece, 0, board_layout, 0); 
        for(auto move : validMovesAndEatingPiece.first)
            moves.push_back(move);
    }
    return moves;
}

std::vector<BitmaskBoard> get_all_moves3(BitmaskBoard& board_layout, char color, bool& isEmptyForceList)
{
    auto main_parent_list = std::vector<std::vector<Piece>>();
    auto all_valid_moves = get_all_valid_moves(board_layout, color, isEmptyForceList, main_parent_list);

    std::vector<BitmaskBoard> moves;
    counter++;
    
    // if(!isEmptyForceList)
    // {
    //     for (int moveIndex = 0; moveIndex<main_parent_list.size(); moveIndex++)
    //     {
    //         auto list = main_parent_list[moveIndex];
    //         BitmaskBoard board_layout2 = board_layout;
    //         auto listSize = list.size();
    //         move_piece2(Move(list[0].getRow(), list[0].getCol(), list[listSize - 1].getRow(), list[listSize - 1].getCol()), board_layout2, main_parent_list, board_layout.get(list[0].getRow(), list[0].getCol()));
    //         moves.push_back(board_layout2);
    //     }
    // }
    // else 
    // {
        // if(all_valid_moves.size() == 1 && main_parent_list[0].size() > 9)
        // {
        //     // print main parent list
        //     for(auto list : main_parent_list)
        //     {
        //         for(auto piece : list)
        //             std::cout<<int(piece.getRow())<<"-"<<int(piece.getCol())<<" ";
        //         std::cout<<"\n";
        //     }
        // }
        for(auto move : all_valid_moves)
        {
            // if(!isEmptyForceList && main_parent_list.size() > 1)
            // {
            //     printBoard(board_layout);
            //     std::cout<<std::endl;
            // }
            
            // if(main_parent_list.size() == 0 && !isEmptyForceList)
            //     break;
            BitmaskBoard board_layout2 = board_layout;
            move_piece2(move, board_layout2, main_parent_list, board_layout.get(move.getFromRow(), move.getFromCol()));
            moves.push_back(board_layout2);

            // if(!isEmptyForceList && main_parent_list.size() > 1)
            // {
            //     printBoard(board_layout2);
            //     std::cout<<std::endl;
            //     std::cout<<"-------------------\n";
            //     // Sleep(1000);
            // }
        }
    // }
    // if(main_parent_list.size() > 0 && !isEmptyForceList)
    //     std::cout<<main_parent_list.size();
    return moves;
}




