#include <iostream>
#include <tuple>
#include <fstream>
#include <cstring>
#include <time.h> 
#include <vector>
#include <set>
#include <memory>
#include <windows.h>
using namespace std;
int movesSeen = 0;
int counter = 0;
const char RED_DAMA = '4';
const char BLACK_DAMA = '3';
const char BLACK_PIECE = '1';
const char RED_PIECE = '2';
const char EMPTY_SQUARE = '0';

pair<vector<char*>, char> get_valid_moves(int row,int col, char color, int streak, vector<char*> valid_moves,  char* board_layout[8], char eat_direction)
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
    if(color == BLACK_DAMA)
    {
        //right
        int i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout[row][col+i] == BLACK_DAMA || board_layout[row][col+i] == BLACK_PIECE)
                break;
            if (col+i<8 && board_layout[row][col+i] == RED_DAMA || board_layout[row][col+i] == RED_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout[row][col+i+j] == RED_DAMA || board_layout[row][col+i+j] == RED_PIECE || board_layout[row][col+i+j] == BLACK_DAMA || board_layout[row][col+i+j] == BLACK_PIECE)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout[row][col+i+j] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row + '0';
                        v_move[1] = col+i+j + '0';
                        valid_moves.push_back(v_move);
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
            if (board_layout[row][col-i] == BLACK_DAMA || board_layout[row][col-i] == BLACK_PIECE)
                break;
            if (board_layout[row][col-i] == RED_DAMA || board_layout[row][col-i] == RED_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j] == RED_DAMA || board_layout[row][col-i-j] == RED_PIECE || board_layout[row][col-i-j] == BLACK_DAMA || board_layout[row][col-i-j] == BLACK_PIECE)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row+ '0';
                        v_move[1] = col-i-j+'0';
                        valid_moves.push_back(v_move);
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
            if (board_layout[row+i][col] == BLACK_DAMA || board_layout[row+i][col] == BLACK_PIECE)
                break;
            if (board_layout[row+i][col] == RED_DAMA || board_layout[row+i][col] == RED_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col] == RED_DAMA || board_layout[row+i+j][col] == RED_PIECE || board_layout[row+i+j][col] == BLACK_DAMA || board_layout[row+i+j][col] == BLACK_PIECE)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row+i+j+'0';
                        v_move[1] = col+'0';
                        valid_moves.push_back(v_move);
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
            if (board_layout[row-i][col] == BLACK_DAMA || board_layout[row-i][col] == BLACK_PIECE)
                break;
            if (board_layout[row-i][col] == RED_DAMA || board_layout[row-i][col] == RED_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] == RED_DAMA || board_layout[row-i-j][col] == RED_PIECE || board_layout[row-i-j][col] == BLACK_DAMA || board_layout[row-i-j][col] == BLACK_PIECE)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row-i-j+'0';
                        v_move[1] = col+'0';
                        valid_moves.push_back(v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        if (eating_piece== 0)
        {

            i=1;
            while(col+i<8 && board_layout[row][col+i] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row+'0';
                v_move[1] = col+i+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && row>0 && row<8 && col-i<8 && board_layout[row][col-i] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row+'0';
                v_move[1] = col-i+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(row+i<8 && board_layout[row+i][col] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row+i+'0';
                v_move[1] = col+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout[row-i][col] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row-i+'0';
                v_move[1] = col+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }
        }
    }

    if(color == RED_DAMA)
    {
        //right
        int i=1;
        while( true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout[row][col+i] == RED_DAMA || board_layout[row][col+i] == RED_PIECE)
                break;
            if (col+i<8 && board_layout[row][col+i] == BLACK_DAMA || board_layout[row][col+i] == BLACK_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col+i+j] != EMPTY_SQUARE)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout[row][col+i+j] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row+'0';
                        v_move[1] = col+i+j+'0';
                        valid_moves.push_back(v_move);
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
            if (board_layout[row][col-i] == RED_DAMA || board_layout[row][col-i] == RED_PIECE)
                break;
            if (board_layout[row][col-i] == BLACK_DAMA || board_layout[row][col-i] == BLACK_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j]!= EMPTY_SQUARE)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row+'0';
                        v_move[1] = col-i-j+'0';
                        valid_moves.push_back(v_move);
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
            if (board_layout[row+i][col] == RED_DAMA || board_layout[row+i][col] == RED_PIECE)
                break;
            if (board_layout[row+i][col] == BLACK_DAMA || board_layout[row+i][col] == BLACK_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col]!= EMPTY_SQUARE)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row+i+j+'0';
                        v_move[1] = col+'0';
                        valid_moves.push_back(v_move);
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
            if (board_layout[row-i][col] == RED_DAMA || board_layout[row-i][col] == RED_PIECE)
                break;
            if (board_layout[row-i][col] == BLACK_DAMA || board_layout[row-i][col] == BLACK_PIECE)
            {
                int j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] != EMPTY_SQUARE)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] == EMPTY_SQUARE)
                    {
                        v_move = new char[2];
                        v_move[0] = row-i-j+'0';
                        v_move[1] = col+'0';
                        valid_moves.push_back(v_move);
                        eating_piece = 'd';
                    }
                    j+=1;
                }
            }
            i+=1;
        }
        
        if (eating_piece== 0)
        {

            i=1;
            while(col+i<8 && board_layout[row][col+i] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row+'0';
                v_move[1] = col+i+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && board_layout[row][col-i] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row+'0';
                v_move[1] = col-i+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(row+i<8 && board_layout[row+i][col] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row+i+'0';
                v_move[1] = col+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout[row-i][col] == EMPTY_SQUARE)
            {
                v_move = new char[2];
                v_move[0] = row-i+'0';
                v_move[1] = col+'0';
                valid_moves.push_back(v_move);
                i+=1;
            }
        }
    }

    if(color == BLACK_PIECE)
    {
        //streak
        if (row+2<8 && (board_layout[row+1][col]== RED_PIECE || board_layout[row+1][col]==RED_DAMA) && board_layout[row+2][col]== EMPTY_SQUARE && streak>=0)
        {
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+2+'0';
            v_move[1] = col+'0';
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }   

        if (col+2<8 && (board_layout[row][col+1]== RED_PIECE || board_layout[row][col+1]==RED_DAMA) && board_layout[row][col+2]== EMPTY_SQUARE && streak>=0 && !ate_left)
        {
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+'0';
            v_move[1] = col+2+'0';
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            
        if (col-2>=0 && (board_layout[row][col-1]== RED_PIECE || board_layout[row][col-1]==RED_DAMA) && board_layout[row][col-2]== EMPTY_SQUARE && streak>=0 &&  !ate_right)
        {
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+'0';
            v_move[1] = col-2+'0';
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            
        if (row+1<8 && board_layout[row+1][col]== EMPTY_SQUARE && streak== 0)
        {
            v_move = new char[2];
            v_move[0] = row+1+'0';
            v_move[1] = col+'0';
            valid_moves.push_back(v_move);
        }
        if (col+1<8 && board_layout[row][col+1]== EMPTY_SQUARE && streak== 0)
        {
            v_move = new char[2];
            v_move[0] = row+'0';
            v_move[1] = col+1+'0';
            valid_moves.push_back(v_move);
        }
        if (col-1>=0 && board_layout[row][col-1]== EMPTY_SQUARE && streak== 0)
        {
            v_move = new char[2];
            v_move[0] = row+'0';
            v_move[1] = col-1+'0';
            valid_moves.push_back(v_move);
        }
    }
    else if (color == RED_PIECE)
    {
        //streak
        
        if (row-2>=0 && (board_layout[row-1][col]== BLACK_PIECE || board_layout[row-1][col]==BLACK_DAMA) && board_layout[row-2][col]== EMPTY_SQUARE && streak>=0)
        {
            v_move = new char[2];
            streak+=1;
            v_move[0] = row-2+'0';
            v_move[1] = col+'0';
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }

        if (col+2<8 && (board_layout[row][col+1]== BLACK_PIECE || board_layout[row][col+1]==BLACK_DAMA) && board_layout[row][col+2]== EMPTY_SQUARE && streak>=0 && !ate_left)
        {
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+'0';
            v_move[1] = col+2+'0';
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            

        if (col-2>=0 && (board_layout[row][col-1]== BLACK_PIECE || board_layout[row][col-1]==BLACK_DAMA) && board_layout[row][col-2]== EMPTY_SQUARE && streak>=0 &&  !ate_right)
        {
            v_move = new char[2];
            streak+=1;
            v_move[0] = row+'0';
            v_move[1] = col-2+'0';
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }

        if (row-1>=0 && board_layout[row-1][col]== EMPTY_SQUARE && streak== 0)
        {
            v_move = new char[2];
            v_move[0] = row-1+'0';
            v_move[1] = col+'0';
            valid_moves.push_back(v_move);
        }
        if (col+1<8 && board_layout[row][col+1]== EMPTY_SQUARE && streak== 0)
        {
            v_move = new char[2];
            v_move[0] = row+'0';
            v_move[1] = col+1+'0';
            valid_moves.push_back(v_move);
        }
        if (col-1>=0 && board_layout[row][col-1]== EMPTY_SQUARE && streak== 0)
        {
            v_move = new char[2];
            v_move[0] = row+'0';
            v_move[1] =  col-1+'0';
            valid_moves.push_back(v_move);
        }
    
    }
    
    pair<vector<char*>, char> validMovesAndEatingPiece;
    validMovesAndEatingPiece.first = valid_moves;
    validMovesAndEatingPiece.second = eating_piece;

    return validMovesAndEatingPiece;
}

char get_eat_direction(int row1, int col1, int row2, int col2)
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

pair<char**, char> eat_piece_if_possible(char* board_layout[8], int row, int col, int r, int c, char color)
{
    char eat_direction = 0;
    int sumRowOver2;
    int sumColOver2;
    if (color == '1' || color == '2')
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
        
        eat_direction = get_eat_direction(r, c, row, col);
    }

    if (color == BLACK_PIECE)
    {
        if (board_layout[sumRowOver2][col] == RED_PIECE || board_layout[sumRowOver2][col] == RED_DAMA)
        {
            board_layout[sumRowOver2][col]=EMPTY_SQUARE;
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout[row][sumColOver2] == RED_PIECE || board_layout[row][sumColOver2] == RED_DAMA)
        {
            board_layout[row][sumColOver2]=EMPTY_SQUARE;
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == '2')
    {
        if (board_layout[sumRowOver2][col] == '1' || board_layout[sumRowOver2][col] == '3')
        {
            board_layout[sumRowOver2][col]='0';
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout[row][sumColOver2] == '1' || board_layout[row][sumColOver2] == '3')
        {
            board_layout[row][sumColOver2]='0';
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == '3')
    {
        if (c==col)
        {
            int big, small;
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
            for(int k = small; k<big; k++)
            {
                if (board_layout[k][c] == '2' || board_layout[k][c] == '4')
                {
                    board_layout[k][c]='0';
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
        else if (row ==r)
        {
            int small, big;
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
            for(int k=small; k<big; k++)
            {
                if (board_layout[r][k] == '2' || board_layout[r][k] == '4')
                {
                    board_layout[r][k]='0';
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }                
    else if (color =='4')
    {
        if (c==col)
        {
            int big, small;
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
            for(int k = small; k<big; k++)
            {
                if (board_layout[k][c] == '1' || board_layout[k][c] == '3')
                {
                    board_layout[k][c]='0';
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
        else if (row ==r)
        {
            int small, big;
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
            for(int k=small; k<big; k++)
            {
                if (board_layout[r][k] == '1' || board_layout[r][k] == '3')
                {
                    board_layout[r][k]='0';
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }
    return make_pair(board_layout, eat_direction);
}

char** deepcopy2(char* board_layout[8])
{   
    char** new_board_layout = new char*[8];

    for (int i = 0; i < 8; i++) {
        new_board_layout[i] = new char[8];
        std::memcpy(new_board_layout[i], board_layout[i], 8 * sizeof(char));
    }
    return new_board_layout;
}

bool equals(vector<char*> a, vector<char*> b)
{
    for(int i=0; i<(int)a.size(); i++)
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

std::vector<std::vector<char*>> eat_max2(int row, int col,char* board_layout[8],  vector<vector<char*>> parent_list, char color, char eat_direction) {
    
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

            if (move[1]-'0' == col) {
                if (move[0]-'0' > row) {
                    eat_direction = 'd';
                } else {
                    eat_direction = 'u';
                }
            } else {
                if (move[1]-'0' > col) {
                    eat_direction = 'r';
                } else {
                    eat_direction = 'l';
                }
            }
    
            // Call get_valid_moves function
            auto  eating_piece = get_valid_moves(move[0]-'0', move[1]-'0', color, 0, {}, board_layout, eat_direction).second;
            dama_nom = (eating_piece == 'd');
    
            if (dama_nom) {
                char* parent = new char[2];
                parent[0] = row+'0'; 
                parent[1] = col+'0';
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
            int counter = 0;
            for (vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[int(value.size())-2];

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
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0]-'0', last[1]-'0', color, 0, {}, board_layout, eat_direction);

                dama_nom = (eating_piece == 'd');

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }

                }
                parent_list = aux_list2;
                return parent_list;
            }
            return parent_list;
            
        } else {
            valid_moves = new_list;
            std::vector<std::vector<char*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(deepcopy2(board_layout), row, col, move[0]-'0', move[1]-'0', color);
                auto p2 = eat_max2(move[0]-'0', move[1]-'0', new_board_layout, parent_list, color, eat_direction);
                
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

std::vector<std::vector<char*>> eat_max2_not_dama(int row, int col,char* board_layout[8],  vector<vector<char*>> parent_list, char color, char eat_direction) {
    
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
            
            if (move[1]-'0' == col) {
                if (move[0]-'0' > row) {
                    eat_direction = 'd';
                } else {
                    eat_direction = 'u';
                }
            } else {
                if (move[1]-'0' > col) {
                    eat_direction = 'r';
                } else {
                    eat_direction = 'l';
                }
            }
    
            // Call get_valid_moves function
            auto  eating_piece = get_valid_moves(move[0]-'0', move[1]-'0', color, 0, {}, board_layout, eat_direction).second;
            dama_nom = (eating_piece != 0);
    
            if (dama_nom) {
                char* parent = new char[2];  parent[0] = row+'0'; parent[1] = col+'0';
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
            int counter = 0;
            for (vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[int(value.size())-2];

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
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0]-'0', last[1]-'0', color, 0, {}, board_layout, eat_direction);

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
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(deepcopy2(board_layout), row, col, move[0]-'0', move[1]-'0', color);
                auto p2 = eat_max2_not_dama(move[0]-'0', move[1]-'0', new_board_layout, parent_list, color, eat_direction);
                if(maxp.empty() || p2[0].size() > maxp[0].size())
                    maxp = p2;
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty vector
    }
}

bool passage_is_clear(char* board_layout[8], int row, int col, char turn) {
    if (turn == '1') {
        if (col == 7) {
            if (board_layout[6][7] != '2' && board_layout[6][6] != '2' && board_layout[6][7] != '4' && board_layout[6][6] != '4') {
                return true;
            }
            return false;
        }

        if (col == 0) {
            if (board_layout[6][0] != '2' && board_layout[6][1] != '2' && board_layout[6][0] != '4' && board_layout[6][1] != '4') {
                return true;
            }
            return false;
        }

        if ((board_layout[row + 1][col + 1] == '2' || board_layout[row + 1][col + 1] == '4') ^
            (board_layout[row + 1][col - 1] == '2' || board_layout[row + 1][col - 1] == '4')) {
            return false;
        }

        for (int j = -2; j <= 2; j++) {
            if (col + j < 0) {
                continue;
            }
            if (col + j > 7) {
                break;
            }
            if (board_layout[row + 2][col + j] == '2' || board_layout[row + 2][col + j] == '4') {
                return false;
            }
        }

        return true;
    }

    if (turn == '2') {
        if (col == 7) {
            if (board_layout[1][7] != '1' && board_layout[1][6] != '1' && board_layout[1][7] != '3' && board_layout[1][6] != '3') {
                return true;
            }
            return false;
        }

        if (col == 0) {
            if (board_layout[1][0] != '1' && board_layout[1][1] != '1' && board_layout[1][0] != '3' && board_layout[1][1] != '3') {
                return true;
            }
            return false;
        }

        if ((board_layout[row - 1][col + 1] == '1' || board_layout[row - 1][col + 1] == '3') ^
            (board_layout[row - 1][col - 1] == '1' || board_layout[row - 1][col - 1] == '3')) {
            return false;
        }

        for (int j = -2; j <= 2; j++) {
            if (col + j < 0) {
                continue;
            }
            if (col + j > 7) {
                break;
            }
            if (board_layout[row - 2][col + j] == '1' || board_layout[row - 2][col + j] == '3') {
                return false;
            }
        }

        return true;
    }

    return false; // Return false for any other turn value
}

vector<char*> get_all_pieces(char* board_layout[8], char color)
{
    vector<char*> pieces = vector<char*>();
    
    int i, j;

    if(color == '1' || color == '3')
    {
        for(i=7; i>=0; i--)
        {
            for(j=0; j<8; j++)
            {
                if(board_layout[i][j]=='1' || board_layout[i][j] == '3')
                    {
                        //make piece
                        char* piece = (char*)malloc(sizeof(char)*2);
                        piece[0] = i + '0';
                        piece[1] = j + '0';
                        pieces.push_back(piece);
                        //cout<<"j: "<<pieces[i][j]<<endl;

                    }
            }
        }
    }
    else
    {
        for(i=0; i<8; i++)
        {
            for(j=0; j<8; j++)
            {
                if(board_layout[i][j] == '2' || board_layout[i][j] == '4')
                    {
                        //make piece
                        char* piece = (char*)malloc(sizeof(char)*2);
                        piece[0] = i + '0';
                        piece[1] = j + '0';
                        pieces.push_back(piece);
                        //cout<<"j: "<<pieces[i][j]<<endl;

                    }
            }
        }
    }
    return pieces;
}

pair< vector<char*>, bool > check_for_possible_capture(char* board_layout[8], char turn)
{
    vector<char*> pieces = get_all_pieces(board_layout, turn);
    for(auto piece:pieces){
        int row = piece[0];
        int col = piece[1];
        if((board_layout[row-'0'][col-'0']=='3' && turn=='1') || (board_layout[row-'0'][col-'0']=='4' && turn == '2'))
        {
            char eating_piece = get_valid_moves(row-'0', col-'0', board_layout[row-'0'][col-'0'], 0, {}, board_layout, 0).second;
            if(eating_piece!=0)
                return make_pair(pieces, true);
        }
        else if(turn == '2'){
            if(col-2-'0'>=0 && board_layout[row-'0'][col-2-'0']=='0' && (board_layout[row-'0'][col-1-'0'] =='1' || board_layout[row-'0'][col-1-'0'] == '3'))
                return make_pair(pieces, true);
            if(col+2-'0'<8 && board_layout[row-'0'][col+2-'0']=='0' && (board_layout[row-'0'][col+1-'0'] =='1' || board_layout[row-'0'][col+1-'0'] == '3'))
                return make_pair(pieces, true);
            if(row-2-'0'>=0 && board_layout[row-2-'0'][col-'0']=='0' && (board_layout[row-1-'0'][col-'0'] == '1' || board_layout[row-1-'0'][col-'0'] == '3'))
                return make_pair(pieces, true);
        }
        else{
            if(col-2-'0'>=0 && board_layout[row-'0'][col-2-'0']=='0' && (board_layout[row-'0'][col-1-'0'] =='2' || board_layout[row-'0'][col-1-'0'] == '4'))
                return make_pair(pieces, true);
            if(col+2-'0'<8 && board_layout[row-'0'][col+2-'0']=='0' && (board_layout[row-'0'][col+1-'0'] =='2' || board_layout[row-'0'][col+1-'0'] == '4'))
                return make_pair(pieces, true);
            if(row+2-'0'<8 && board_layout[row+2-'0'][col-'0']==0 && (board_layout[row+1-'0'][col-'0'] == '2' || board_layout[row+1-'0'][col-'0'] == '4'))
                return make_pair(pieces, true);
        }
    }

    return make_pair(pieces, false);
}

bool check_if_piece_can_capture(int row, int col, char* board_layout[8],char turn){
    if((board_layout[row][col]=='3' && turn=='2') || (board_layout[row][col]=='4' && turn == '2'))
    {
        char eating_piece = get_valid_moves(row, col, board_layout[row][col], 0, {}, board_layout, 0).second;
        if(eating_piece!=0)
            return true;
    }
    else if(turn == '2'){
        if(col-2>=0 && board_layout[row][col-2]=='0' && (board_layout[row][col-1] =='1' || board_layout[row][col-1] == '3'))
            return true;
        if(col+2<8 && board_layout[row][col+2]=='0' && (board_layout[row][col+1] =='1' || board_layout[row][col+1] == '3'))
            return true;
        if(row-2>=0 && board_layout[row-2][col]=='0' && (board_layout[row-1][col] == '1' || board_layout[row-1][col] == '3'))
            return true;
    }
    else{
        if(col-2>=0 && board_layout[row][col-2]=='0' && (board_layout[row][col-1] =='2' || board_layout[row][col-1] == '4'))
            return true;
        if(col+2<8 && board_layout[row][col+2]=='0' && (board_layout[row][col+1] =='2' || board_layout[row][col+1] == '4'))
            return true;
        if(row+2<8 && board_layout[row+2][col]=='0' && (board_layout[row+1][col] == '2' || board_layout[row+1][col] == '4'))
            return true;
    }
    return false;
}

vector<char*> check_for_force(char* board_layout[8], char turn, vector<char*> *pieces = {})
{
    auto [pieces2, check] = check_for_possible_capture(board_layout, turn);
    *pieces = pieces2;
    if(check == false)
        return {};
    
    counter++;
    bool aktar_flag = false;
    vector<char*> aktar_list;
    vector<char*> force_list;
    int maxLength = 3, listSize = 0;


    for(char* piece : pieces2)
    {
        int row2=piece[0]-'0', col2=piece[1]-'0';

        if(!check_if_piece_can_capture(row2, col2, board_layout, turn))
            continue;
        char color = board_layout[row2][col2];
        if(((color=='2' || color=='4') && turn=='2') || ((color=='1' || color=='3') && turn=='1'))
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
                //vector<int*> parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                parent_list2 = vector<vector<char*>>();
                for(char* move : valid_moves)
                {
                    //parent_list2.append([(row2, col2), move])
                    char* temp = new char[2];
                    vector<char*> tempValue = vector<char*>();
                    temp[0] = row2+'0'; temp[1] = col2+'0';
                    tempValue.push_back(temp);
                    tempValue.push_back(move);
                    parent_list2.push_back(tempValue);
                }

                if(color == '1' || color == '2')
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
                else if (color == '3' || color == '4')
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
                    piece[0] = row2+'0'; piece[1] = col2+'0';
                    force_list.push_back(piece);
                }
            }
        }
    }

    if(aktar_flag)
        force_list = aktar_list;
    return force_list;
}

char** move_piece(char* piece, char* move, char* board_layout[8], vector<vector<char*>> parent_list, char color)
{
    int row = piece[0]-'0';
    int col = piece[1]-'0';

    int r = move[0]-'0';
    int c = move[1]-'0';

    int listSize = parent_list.size();
    int index=0;
    int value_length;

    if(!parent_list.empty())
    {

        for(vector<char*> value : parent_list)
        {
            value_length = value.size();
            auto temp = value[value_length-1];
            if((temp[0]-'0')==r && (temp[1]-'0')==c)
            {
                int z=0;
                for(z = 0; z<value_length; z++)
                {
                    if(z+1 >= value_length) break;
                    board_layout = eat_piece_if_possible(board_layout, value[z][0]-'0', value[z][1]-'0', value[z+1][0]-'0', value[z+1][1]-'0', color).first;
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
    aux = board_layout[row][col];
    board_layout[row][col]=board_layout[r][c];
    board_layout[r][c]=aux;

    //check for dama promotion
    if(r==0 && color=='2')
        board_layout[r][c] = '4';
    if(r==7 && color=='1')
        board_layout[r][c] = '3';
    
    return board_layout;
}

int evaluate_int(char* board_layout[8], char turn)
{
    movesSeen++;
    int sum = 0, balance_black = 0, balance_red = 0, all_pieces=0, red_pieces=0, black_pieces = 0;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            int piece = board_layout[i][j]-'0';

            if(piece == '1')
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
                    if(turn == '1')
                        sum+=40;
                }
                if(i==5)
                    sum+=40;
                if(i==4 && turn =='1')
                    sum+=30;
                    if(j==0 || j==7)
                        sum+=10;

                //4 important squares
                if(j==0 or j==7)
                {
                    if (i == 5 and turn == '1')
                        sum+=80;
                    if (i == 5 and turn == '2')
                        sum+=20;
                    if (i == 6 and turn == '1')
                        sum+=50;
                }

                //if on 5th row and passage is clear the piece is dangerous
                if (i==4 && turn==1 && passage_is_clear(board_layout, i, j, turn))
                    sum+=70;
                
            }
            else if(piece == '2')
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
                if(i==3 && turn =='2')
                    sum-=30;
                    if(j==0 || j==7)
                        sum-=10;

                //4 important squares
                if(j==0 or j==7)
                {
                    if (i == 2 and turn == '2')
                        sum-=80;
                    if (i == 2 and turn == '1')
                        sum-=20;
                    if (i == 1 and turn == '2')
                        sum-=50;
                }

                //if on 5th row and passage is clear the piece is dangerous
                if(i==3 && turn==2 && passage_is_clear(board_layout, i, j, turn))
                    sum-=70;
            }
            else if(piece=='3')
            {
                black_pieces+=1;
                sum+=450;
            }
            else if(piece=='4')
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

pair<vector<char**>, vector<char*>> get_all_moves(char* board_layout[8], char color, vector<char**> *moves = {})
{
    vector<char *> pieces;

    //create force list
    vector<char *> force_list;
    vector<char *> valid_moves;
    vector<vector<char *>> parent_list;
    bool dama_nom = false, fff = false;

    force_list = check_for_force(board_layout, color, &pieces);

    if(!force_list.empty())
        pieces = force_list;
    // else
    //     pieces = get_all_pieces(board_layout, color);
    
    for(char* piece : pieces)
    {
        char color2 = board_layout[piece[0]-'0'][piece[1]-'0'];
        fff = dama_nom = false;
        if(color2 == '1' || color2 == '2')
        {
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0]-'0', piece[1]-'0', color2, 0, {}, board_layout, 0);
            
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

                parent_list = eat_max2_not_dama(piece[0]-'0', piece[1]-'0', board_layout, parent_list2, color2, 0);
                vector<char*> aux_list;
                if(!parent_list.empty())
                {
                    for(vector<char*> value: parent_list)
                        aux_list.push_back(value[int(value.size()) - 1]);
                    valid_moves = aux_list;
                }

            }
        }

        if (color2 =='3' || color2 == '4')
        {
            pair<vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0]-'0', piece[1]-'0', color2, 0, {}, board_layout, 0);
            
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

                parent_list = eat_max2(piece[0]-'0', piece[1]-'0', board_layout, parent_list2, color2, 0);
                vector<char*> aux_list;
                if(!parent_list.empty())
                {
                    for(vector<char*> value: parent_list)
                        aux_list.push_back(value[int(value.size()) - 1]);
                    valid_moves = aux_list;
                }
            }
        }

        for(char* move : valid_moves)
        {
            // moves.push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
        }
    }

    return make_pair(*moves, force_list);
}

void printBoard(char** board)
{
    for(int i=0; i<8; i++)
    {
        for(int j=0;j<8; j++)
        {
            std::cout<<board[i][j]<<" ";
        }
        std::cout<<endl;
    }
}

bool playerWon(char* board_layout[8]){
    bool red = false, black = false;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            if(board_layout[i][j]=='1' || board_layout[i][j]=='3')
                black = true;
            if(board_layout[i][j]=='2' || board_layout[i][j]=='4')
                red = true;
            if(red && black)
                return false;
        }
    return true;
}

pair<int, char**> minimax_pro2(int depth, bool max_player, char* board_layout[8], int alpha, int beta, int akel_depth, int akel_player, int akling)
{
    int evaluation, maxEval, minEval;
    char** best_move;

    if(depth<=0 || playerWon(board_layout))
    {
        int turn;
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
        // Sleep(4500);

        best_move = NULL;
        maxEval = INT_MIN;
        vector <char**> all_moves;
        pair<vector <char**>, vector<char*> > allandForce = get_all_moves(board_layout, '1', &all_moves);
        vector<char*> force_list = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list.empty() && akel_depth<5)
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
        // Sleep(4500);

        best_move = NULL;
        minEval = INT_MAX;
        vector <char**> all_moves;
        pair<vector <char**>, vector<char*> > allandForce = get_all_moves(board_layout, '2', &all_moves);
        all_moves = allandForce.first;
        vector<char*> force_list = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list.empty() && akel_depth<5)
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

pair<int, char**> normal_minimax(int depth, int max_player, char* board_layout[8], int alpha, int beta)
{
    int evaluation, maxEval, minEval;
    char** best_move;

    if(depth<=0 || playerWon(board_layout))
    {
        int turn;
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
        // Sleep(500);

        best_move = NULL;
        maxEval = INT_MIN;
        vector <char**> all_moves;
        pair<vector <char**>, vector<char*> > allandForce = get_all_moves(board_layout, 1, &all_moves);
        vector<char*> force_list = allandForce.second;

        for (auto move : all_moves)
        {  
            if(move == NULL) continue;

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
        return make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(500);

        best_move = NULL;
        minEval = INT_MAX;
        vector <char**> all_moves;
        pair<vector <char**>, vector<char*> > allandForce = get_all_moves(board_layout, 2, &all_moves);
        vector<char*> force_list = allandForce.second;

        for (auto move : all_moves)
        {
            if(move == NULL) continue;

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
        return make_pair(minEval, best_move);
    }
}


int main()
{
    double time_spent = 0.0;
    // vector<int*> valid_moves;
    //for(int i=0; i<1000000; i++)
            //eat_piece_if_possible(test3, 7, 0, 4, 0, 3);
    /*for(int i=0; i<1000000; i++)
        evaluate_int(test2, 1);*/
    /*for(int i=0; i<1000000; i++)
    {
        get_valid_moves(2, 1, 1, 0, valid_moves, test2, 0);
    }*/

    vector<vector<int*>> parent_list2;
    int a[2] = {6, 0};
    int b[2] = {4, 0};
    vector<int*> v;
    v.push_back(a);
    v.push_back(b);
    parent_list2.push_back(v);

    // vector<int*> v2;
    // int d[2] = {5, 2};
    // int c[2] = {5, 0};
    // v2.push_back(d);
    // v2.push_back(c);
    // parent_list2.push_back(v2);

    /*
    valid_moves = get_valid_moves(3, 2, 1, 0, {}, test2, "").first;
    cout<<valid_moves[0][0]<<","<<valid_moves[0][1]<<endl;*/
    // for(int i=0; i<100000; i++)
    // {
    //     eat_max2(7, 0, test2, parent_list2, 3, 0);
    // }

    //testing
    //vector<int*> all_pieces = get_all_pieces(test2, 2);
    //vector<int *> valid_moves = get_valid_moves(6, 0, 4, 0, {}, test2, 0).first;
    //vector<vector<int*>> parent_list = eat_max2(6, 0, test2, parent_list2, 4, 0);
    // vector<int**> all_moves = get_all_moves(test2, 1).first;

    char* test2[8];  
    char array[8] =  {'0', '0', '0', '0', '0', '0', '0', '0'};
    test2[0] = array;
    char array2[8] = {'1', '1', '1', '1', '1', '1', '1', '1'};
    test2[1] = array2;
    char array3[8] = {'1', '1', '1', '1', '1', '1', '1', '1'};
    test2[2] = array3;
    char array4[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
    test2[3] = array4;
    char array5[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
    test2[4] = array5;
    char array6[8] = {'2', '2', '2', '2', '2', '2', '2', '2'};
    test2[5] = array6;
    char array7[8] = {'2', '2', '2', '2', '2', '2', '2', '2'};
    test2[6] = array7;
    char array8[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
    test2[7] = array8;
    // char** test3 = deepcopy2(test2);
    
    pair<int, char**> minimaxResult;
    
    clock_t begin = clock();
    // for(int i=0; i<1000000; i++)
    //     deepcopy2(test2);
    minimaxResult = minimax_pro2(7, false, test2, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = normal_minimax(8, false, test2, INT_MIN, INT_MAX);
    clock_t end = clock();
    std::cout<<minimaxResult.first/100.0<<endl;
    char** boardResult = minimaxResult.second;
    printBoard(boardResult);
    cout<<movesSeen<<endl;
    cout<<"counter: "<<counter<<endl;

    // std::ofstream outfile("minimaxResult.txt");
    // if (!outfile.is_open()) {
    //     std::cout << "Failed to open file" << std::endl;
    //     return 1;
    // }
    // for(int i=0; i<8; i++)
    // {
    //     for(int j=0;j<8; j++)
    //     {
    //         outfile << boardResult[i][j];
    //         outfile<<" ";
    //     }
    //     outfile<<endl;
    // }

    // for(int i=0; i<100000; i++)
    // {
    //     get_all_moves(test2,  1);
    // }

    // for(auto move : all_moves)
    // {
    //     for(int i=0; i<8; i++)
    //     {
    //         for(int j=0; j<8; j++)
    //             std::cout<<move[i][j]<<" ";
    //         std::cout<<endl;
    //     }

    //     std::cout<<endl<<endl;
    // }

    // for(auto piece : all_pieces)
    // {
    //     cout<<piece[0]<<"-"<<piece[1]<<endl;
    // }

    //  int piece[2], move[2];
    //  piece[0] = 5; piece[1] = 4;
    //  move[0] = 5; move[1] = 2;

    //  std::cout<<test3[1][2]<<endl;

    //  vector<int*> parent_list5;
    //  int *g = new int[2], *h = new int[2], *l = new int[2], *p = new int[2];
    //  g[0]=4;g[1]=3;h[0]=2;h[1]=3;l[0]=1;l[1]=1;p[0]=1;p[1]=3;
    //  parent_list5.push_back(g);
    //  parent_list5.push_back(h);
    //  parent_list5.push_back(l);
    //  parent_list5.push_back(p);
    
    //  vector<vector<int*>> parent_list6;
    //  parent_list6.push_back(parent_list5);

    //  test3 = move_piece(piece, move, test2, parent_list6, 4);
    //  std::cout<<test3[1][2]<<endl;

    // vector<int*> force_list = check_for_force(test2, 1);
    // auto allMoves = get_all_moves(test2, 1);
    // vector<vector<int*>> parent_list = eat_max2_not_dama(4, 3, test2, parent_list6, 2, 0);
    // for(auto value : allMoves.first){
    //     printBoard(value);
    // }

    // for(int j=0; j<(int)valid_moves.size(); j++)
    // {
    //     cout<< valid_moves[j][0]<<"-"<<valid_moves[j][1]<<endl;
    // }

    // for(int i=0; i<(int)parent_list.size(); i++)
    // {
    //     for(int j=0; j<(int)parent_list[i].size(); j++)
    //     {
    //         cout<< parent_list[i][j][0]<<"-"<<parent_list[i][j][1]<<endl;
    //     }
    //     cout<<endl;
    // }

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
    std::cout<< time_spent;

    //valid_moves = vector<int*>(); 
}