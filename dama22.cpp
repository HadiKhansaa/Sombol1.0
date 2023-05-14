#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h> 
#include <vector>
#include <set>
#include <memory>
#include <windows.h>
using namespace std;

int movesSeen = 0;
pair<vector<int*>, char> get_valid_moves(int row,int col, int color, int streak, vector<int*> valid_moves,  int* board_layout[8], char eat_direction)
{
    if(row>7 || col>7 || row<0 || col<0)
    {
        pair<vector<int*>, char> validMovesAndEatingPiece;
        validMovesAndEatingPiece.first = {};
        validMovesAndEatingPiece.second = 0;
        return validMovesAndEatingPiece;
    }
    bool ate_up = (eat_direction == 'u');
    bool ate_down = (eat_direction == 'd');
    bool ate_left = (eat_direction == 'l');
    bool ate_right = (eat_direction == 'r');

    int *v_move= new int[2];

    char eating_piece=0;
    
    //black dama
    if(color == 3)
    {
        //right
        int i=1;
        while(true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout[row][col+i] == 3 || board_layout[row][col+i] == 1)
                break;
            if (col+i<8 && board_layout[row][col+i] == 4 || board_layout[row][col+i] == 2)
            {
                int j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout[row][col+i+j] == 4 || board_layout[row][col+i+j] == 2 || board_layout[row][col+i+j] == 3 || board_layout[row][col+i+j] == 1)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout[row][col+i+j] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row;
                        v_move[1] = col+i+j;
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
            if (board_layout[row][col-i] == 3 || board_layout[row][col-i] == 1)
                break;
            if (board_layout[row][col-i] == 4 || board_layout[row][col-i] == 2)
            {
                int j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j] == 4 || board_layout[row][col-i-j] == 2 || board_layout[row][col-i-j] == 3 || board_layout[row][col-i-j] == 1)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row;
                        v_move[1] = col-i-j;
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
            if (board_layout[row+i][col] == 3 || board_layout[row+i][col] == 1)
                break;
            if (board_layout[row+i][col] == 4 || board_layout[row+i][col] == 2)
            {
                int j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col] == 4 || board_layout[row+i+j][col] == 2 || board_layout[row+i+j][col] == 3 || board_layout[row+i+j][col] == 1)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row+i+j;
                        v_move[1] = col;
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
            if (board_layout[row-i][col] == 3 || board_layout[row-i][col] == 1)
                break;
            if (board_layout[row-i][col] == 4 || board_layout[row-i][col] == 2)
            {
                int j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] == 4 || board_layout[row-i-j][col] == 2 || board_layout[row-i-j][col] == 3 || board_layout[row-i-j][col] == 1)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row-i-j;
                        v_move[1] = col;
                        valid_moves.push_back(v_move);
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
            while(col+i<8 && board_layout[row][col+i] == 0)
            {
                v_move = new int[2];
                v_move[0] = row;
                v_move[1] = col+i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && row>0 && row<8 && col-i<8 && board_layout[row][col-i] == 0)
            {
                v_move = new int[2];
                v_move[0] = row;
                v_move[1] = col-i;
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(row+i<8 && board_layout[row+i][col] == 0)
            {
                v_move = new int[2];
                v_move[0] = row+i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout[row-i][col] == 0)
            {
                v_move = new int[2];
                v_move[0] = row-i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
        }
    }

    if(color == 4)
    {
        //right
        int i=1;
        while( true && !ate_left)
        {
            if (col+i >= 8)
                break;
            if (col+i<8 && board_layout[row][col+i] == 4 || board_layout[row][col+i] == 2)
                break;
            if (col+i<8 && board_layout[row][col+i] == 3 || board_layout[row][col+i] == 1)
            {
                int j=1;
                while(true)
                {
                    if (col+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col+i+j] != 0)
                    {
                        i=10;
                        break;
                    }
                    if (col+i+j<8 && board_layout[row][col+i+j] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row;
                        v_move[1] = col+i+j;
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
            if (board_layout[row][col-i] == 4 || board_layout[row][col-i] == 2)
                break;
            if (board_layout[row][col-i] == 3 || board_layout[row][col-i] == 1)
            {
                int j=1;
                while(true)
                {
                    if (col-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j]!=0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row][col-i-j] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row;
                        v_move[1] = col-i-j;
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
            if (board_layout[row+i][col] == 4 || board_layout[row+i][col] == 2)
                break;
            if (board_layout[row+i][col] == 3 || board_layout[row+i][col] == 1)
            {
                int j=1;
                while(true)
                {
                    if (row+i+j>=8)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col]!=0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row+i+j][col] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row+i+j;
                        v_move[1] = col;
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
            if (board_layout[row-i][col] == 4 || board_layout[row-i][col] == 2)
                break;
            if (board_layout[row-i][col] == 3 || board_layout[row-i][col] == 1)
            {
                int j=1;
                while(true)
                {
                    if (row-i-j<0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] != 0)
                    {
                        i=10;
                        break;
                    }
                    if (board_layout[row-i-j][col] == 0)
                    {
                        v_move = new int[2];
                        v_move[0] = row-i-j;
                        v_move[1] = col;
                        valid_moves.push_back(v_move);
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
            while(col+i<8 && board_layout[row][col+i] == 0)
            {
                v_move = new int[2];
                v_move[0] = row;
                v_move[1] = col+i;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(col-i>0 && board_layout[row][col-i] == 0)
            {
                v_move = new int[2];
                v_move[0] = row;
                v_move[1] = col-i;
                valid_moves.push_back(v_move);
                i+=1;
            }

            i=1;
            while(row+i<8 && board_layout[row+i][col] == 0)
            {
                v_move = new int[2];
                v_move[0] = row+i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
            
            i=1;
            while(row-i>0 && board_layout[row-i][col] == 0)
            {
                v_move = new int[2];
                v_move[0] = row-i;
                v_move[1] = col;
                valid_moves.push_back(v_move);
                i+=1;
            }
        }
    }

    if(color == 1)
    {
        //streak
        if (row+2<8 && (board_layout[row+1][col]==2 || board_layout[row+1][col]==4) && board_layout[row+2][col]==0 && streak>=0)
        {
            v_move = new int[2];
            streak+=1;
            v_move[0] = row+2;
            v_move[1] = col;
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            

        if (col+2<8 && (board_layout[row][col+1]==2 || board_layout[row][col+1]==4) && board_layout[row][col+2]==0 && streak>=0 && !ate_left)
        {
            v_move = new int[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col+2;
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            

        if (col-2>=0 && (board_layout[row][col-1]==2 || board_layout[row][col-1]==4) && board_layout[row][col-2]==0 && streak>=0 &&  !ate_right)
        {
            v_move = new int[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col-2;
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            
        if (row+1<8 && board_layout[row+1][col]==0 && streak==0)
        {
            v_move = new int[2];
            v_move[0] = row+1;
            v_move[1] = col;
            valid_moves.push_back(v_move);
        }
        if (col+1<8 && board_layout[row][col+1]==0 && streak==0)
        {
            v_move = new int[2];
            v_move[0] = row;
            v_move[1] = col+1;
            valid_moves.push_back(v_move);
        }
        if (col-1>=0 && board_layout[row][col-1]==0 && streak==0)
        {
            v_move = new int[2];
            v_move[0] = row;
            v_move[1] = col-1;
            valid_moves.push_back(v_move);
        }
    }
    else if (color == 2)
    {
        //streak
        
        if (row-2>=0 && (board_layout[row-1][col]==1 || board_layout[row-1][col]==3) && board_layout[row-2][col]==0 && streak>=0)
        {
            v_move = new int[2];
            streak+=1;
            v_move[0] = row-2;
            v_move[1] = col;
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }

        if (col+2<8 && (board_layout[row][col+1]==1 || board_layout[row][col+1]==3) && board_layout[row][col+2]==0 && streak>=0 && !ate_left)
        {
            v_move = new int[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col+2;
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }
            

        if (col-2>=0 && (board_layout[row][col-1]==1 || board_layout[row][col-1]==3) && board_layout[row][col-2]==0 && streak>=0 &&  !ate_right)
        {
            v_move = new int[2];
            streak+=1;
            v_move[0] = row;
            v_move[1] = col-2;
            valid_moves.push_back(v_move);
            eating_piece = 'n';
        }

        if (row-1>=0 && board_layout[row-1][col]==0 && streak==0)
        {
            v_move = new int[2];
            v_move = new int[2];
            v_move[0] = row-1;
            v_move[1] = col;
            valid_moves.push_back(v_move);
        }
        if (col+1<8 && board_layout[row][col+1]==0 && streak==0)
        {
            v_move = new int[2];
            v_move[0] = row;
            v_move[1] = col+1;
            valid_moves.push_back(v_move);
        }
        if (col-1>=0 && board_layout[row][col-1]==0 && streak==0)
        {
            v_move = new int[2];
            v_move[0] = row;
            v_move[1] =  col-1;
            valid_moves.push_back(v_move);
        }
    
    }
    
    pair<vector<int*>, char> validMovesAndEatingPiece;
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

pair<int**, char> eat_piece_if_possible(int* board_layout[8], int row, int col, int r, int c, int color)
{
    char eat_direction = 0;
    int sumRowOver2;
    int sumColOver2;
    if (color == 1 || color == 2)
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
        
        eat_direction = get_eat_direction(r, c, row, col);
    }

    if (color == 1)
    {
        if (board_layout[sumRowOver2][col] == 2 || board_layout[sumRowOver2][col] == 4)
        {
            board_layout[sumRowOver2][col]=0;
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout[row][sumColOver2] == 2 || board_layout[row][sumColOver2] == 4)
        {
            board_layout[row][sumColOver2]=0;
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 2)
    {
        if (board_layout[sumRowOver2][col] == 1 || board_layout[sumRowOver2][col] == 3)
        {
            board_layout[sumRowOver2][col]=0;
            return make_pair(board_layout, eat_direction);
        }
        else if (board_layout[row][sumColOver2] == 1 || board_layout[row][sumColOver2] == 3)
        {
            board_layout[row][sumColOver2]=0;
            return make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 3)
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
                if (board_layout[k][c] == 2 || board_layout[k][c] == 4)
                {
                    board_layout[k][c]=0;
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
                if (board_layout[r][k] == 2 || board_layout[r][k] == 4)
                {
                    board_layout[r][k]=0;
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }                
    else if (color == 4)
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
                if (board_layout[k][c] == 1 || board_layout[k][c] == 3)
                {
                    board_layout[k][c]=0;
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
                if (board_layout[r][k] == 1 || board_layout[r][k] == 3)
                {
                    board_layout[r][k]=0;
                    return make_pair(board_layout, eat_direction);
                }
            }
        }
    }
    return make_pair(board_layout, eat_direction);
}

int** deepcopy2(int* board_layout[8])
{   
    int** new_board_layout = new int*[8];
    for (int i = 0; i < 8; i++)
        new_board_layout[i] = new int[8];
        
    
    //memcpy(new_board_layout, board_layout, 8*8*sizeof(int));
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            new_board_layout[i][j] = board_layout[i][j];
    return new_board_layout;
}

// vector<vector<int*>> eat_max2_not_dama(int row, int col,int* board_layout[8],  vector<vector<int*>> parent_list, int color, char eat_direction)
// {
//     pair<vector<int*>, char> validMovesAndEatingPiece;
//     validMovesAndEatingPiece  = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);
    
//     vector<int*> valid_moves = validMovesAndEatingPiece.first;
//     char eating_piece = validMovesAndEatingPiece.second;

//     bool fff = (eating_piece != 0);

//     if (fff)
//     {
//         vector <int*> new_list;
//         vector <vector<int*>> big_dama_list = vector <vector<int*>>();
//         for(auto move : valid_moves)
//         {
//             eat_direction = get_eat_direction(move[0], move[1], row, col);
//             eating_piece = get_valid_moves(move[0], move[1], color, 0, {}, board_layout, eat_direction).second;

//             fff = (eating_piece != 0);

//             if(fff)
//             {
//                 int parent[2];
//                 parent[0] = row; parent[1] = col;
//                 //global malloc move
//                 int* move2 = (int*)malloc(sizeof(int)*2);
//                 move2[0] = move[0]; move2[1] = move[1];

//                 new_list.push_back(move2);
//                 for(auto value : parent_list)
//                 {
//                     if (parent[0] == value[int(value.size())-1][0] && parent[1] == value[int(value.size())-1][1])
//                     {
//                         vector<int*> new_value = vector<int*>((int)value.size());

//                         //copy value
//                         int i=0;
//                         for(i=0; i<(int)value.size(); i++)
//                             new_value[i] = value[i];
//                         new_value.push_back(move2); //add move
                
//                         big_dama_list.push_back(new_value);
//                     }
//                 }
//             }
//         }
//         if (!big_dama_list.empty())
//         {
//             //parent_list = big_dama_list;
//             parent_list = vector<vector<int*>>((int)big_dama_list.size());
//             for (int i=0; i<(int)big_dama_list.size(); i++)
//                 parent_list[i] = big_dama_list[i];
//         }
//         if (new_list.empty())
//         {
//             vector <vector<int*>> aux_list2 = vector <vector<int*>>();
//             for(auto value : parent_list)
//             {
//                 int* last = value[int(value.size())-1];
//                 int* pre_last = value[int(value.size())-2];

//                 eat_direction = get_eat_direction(last[0], last[1], pre_last[0], pre_last[1]);

//                 pair<vector<int*>, char> validMovesAndEatingPiece;
//                 validMovesAndEatingPiece  = get_valid_moves(last[0], last[1], color, 0, {}, board_layout, eat_direction);
    
//                 vector<int*> valid_moves2 = validMovesAndEatingPiece.first;
//                 char eating_piece = validMovesAndEatingPiece.second;

//                 fff = (eating_piece !=0);

//                 if(fff)
//                 {
//                     for(auto move : valid_moves2)
//                     {
//                         //global malloc move
//                         int* move2 = (int*)malloc(sizeof(int)*2);
//                         move2[0] = move[0]; move2[1] = move[1];

//                         vector<int*> new_value = vector<int*>((int)value.size());

//                         //copy value
//                         int i=0;
//                         for(i=0; i<(int)value.size(); i++)
//                             new_value[i] = value[i];
//                         new_value.push_back(move2); //add move

//                         aux_list2.push_back(new_value);
//                     }
//                 }
//             }
//             return aux_list2;
//         }
//         else
//         {
//             vector <vector<int*>> maxp = vector <vector<int*>>();
//             valid_moves = new_list;
//             for (auto move : valid_moves)
//             {
//                 int **new_board_layout = deepcopy2(board_layout);
//                 pair<int**, char> layoutAndDirection;

//                 layoutAndDirection = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color);
                
//                 new_board_layout = layoutAndDirection.first; //new
//                 eat_direction = layoutAndDirection.second; //new
                
//                 vector<vector<int*>> p2 = vector<vector<int*>>();
//                 p2 = eat_max2_not_dama(move[0], move[1], board_layout, parent_list, color, eat_direction);
//                 if(maxp.empty() || p2[0].size() > maxp[0].size())
//                 {
//                     maxp = p2;
//                 }
//             }
//             return maxp;
//         }
//     }
//     else
//         return {};
//     return {};
// }

bool equals(vector<int*> a, vector<int*> b)
{
    for(int i=0; i<(int)a.size(); i++)
        if(a[i][0]!=b[i][0] || a[i][1]!=b[i][1])
            return false;
    return true;
}

bool doesnt_have(vector <vector<int*>> a, vector<int*> b){
    for(auto element:a)
        if(equals(element, b))
            return false;
    return true;
}

std::vector<std::vector<int*>> eat_max2(int row, int col,int* board_layout[8],  vector<vector<int*>> parent_list, int color, char eat_direction) {
    
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);

    bool dama_nom = (eating_piece == 'd');

    if (dama_nom) {
        std::vector<int*> new_list;
        std::vector<std::vector<int*>> big_dama_list;
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
                int* parent = new int[2];  parent[0] = row; parent[1] = col;
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (parent[0] ==  value.back()[0] && parent[1] ==  value.back()[1]) {
                        std::vector<int*> big_dama = value;
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
            std::vector<std::vector<int*>> aux_list2;
            aux_list2.clear();
            int counter = 0;
            for (vector<int*> value : parent_list) {
                int* last = value.back();
                int* pre_last = value[int(value.size())-2];

                int sizeLast = int(sizeof(last)/sizeof(last[0]));
                int sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

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
                        std::vector<int*> aux_list3 = value;
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
            std::vector<std::vector<int*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(deepcopy2(board_layout), row, col, move[0], move[1], color);
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

std::vector<std::vector<int*>> eat_max2_not_dama(int row, int col,int* board_layout[8],  vector<vector<int*>> parent_list, int color, char eat_direction) {
    
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);

    bool dama_nom = (eating_piece != 0);

    if (dama_nom) {
        std::vector<int*> new_list;
        std::vector<std::vector<int*>> big_dama_list;
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
                int* parent = new int[2];  parent[0] = row; parent[1] = col;
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (parent[0] ==  value.back()[0] && parent[1] ==  value.back()[1]) {
                        std::vector<int*> big_dama = value;
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
            std::vector<std::vector<int*>> aux_list2;
            aux_list2.clear();
            int counter = 0;
            for (vector<int*> value : parent_list) {
                int* last = value.back();
                int* pre_last = value[int(value.size())-2];

                int sizeLast = int(sizeof(last)/sizeof(last[0]));
                int sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

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
                        std::vector<int*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
            }
            parent_list = aux_list2;
            return parent_list;
            
        } else {
            valid_moves = new_list;
            std::vector<std::vector<int*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(deepcopy2(board_layout), row, col, move[0], move[1], color);
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


vector<vector<int*>> eat_max2_me(int row, int col,int* board_layout[8],  vector<vector<int*>> parent_list, int color, char eat_direction)
{
    pair<vector<int*>, char> validMovesAndEatingPiece;
    validMovesAndEatingPiece  = get_valid_moves(row, col, color, 0, {}, board_layout, eat_direction);
    
    vector<int*> valid_moves = validMovesAndEatingPiece.first;
    char eating_piece = validMovesAndEatingPiece.second;

    bool dama_nom = (eating_piece == 'd');

    if (dama_nom)
    {
        vector <int*> new_list ;
        vector <vector<int*>> big_dama_list = vector <vector<int*>>();
        // cout<<"valid moves size: "<<valid_moves.size()<<endl;
        new_list.clear();
        for(auto move : valid_moves)
        {
            // cout<<"move: "<<move[0]<<"-"<<move[1]<<endl;
            dama_nom = false;
            eat_direction = get_eat_direction(move[0], move[1], row, col);
            eating_piece = get_valid_moves(move[0], move[1], color, 0, {}, board_layout, eat_direction).second;

            dama_nom = (eating_piece == 'd');

            if(dama_nom)
            {
                int parent[2];
                parent[0] = row; parent[1] = col;
                //global malloc move
                int* move2 = new int[2];
                move2[0] = move[0]; move2[1] = move[1];

                new_list.push_back(move2);
                for(auto value : parent_list)
                {
                    // cout<<"value: "<<value[int(value.size())-1][0]<<"-"<<value[int(value.size())-1][1]<<endl;
                    if (parent[0] == value[int(value.size())-1][0] && parent[1] == value[int(value.size())-1][1])
                    {
                        vector<int*> new_value = vector<int*>();   

                        //copy value
                        int i=0;
                        for(i=0; i<(int)value.size(); i++)
                        {
                            int * temp3 = new int[2];
                            temp3[0] = value[i][0];
                            temp3[1] = value[i][1];
                            new_value.push_back(temp3);
                        }
                        new_value.push_back(move2); //add move
                
                        big_dama_list.push_back(new_value);
                    }
                }
            }
        }
        if (!big_dama_list.empty())
        {
            // parent_list = vector<vector<int*>>((int)big_dama_list.size());
            parent_list = big_dama_list;
            // for (int i=0; i<(int)big_dama_list.size(); i++)
            //     parent_list[i] = big_dama_list[i];
        }
        if (new_list.empty())
        {
            vector <vector<int*>> aux_list2 = vector <vector<int*>>();
            for(vector<int*> value : parent_list)
            {
                int * last = new int[2];
                int * pre_last = new int[2];
                last[0] = value[int(value.size())-1][0];
                last[1] = value[int(value.size())-1][1];
                pre_last[0] = value[int(value.size())-2][0];
                pre_last[1] = value[int(value.size())-2][1];

                int sizeLast = int(sizeof(last)/sizeof(last[0]));
                int sizePreLast = sizeof(pre_last)/sizeof(pre_last[0]);

                eat_direction = get_eat_direction(last[0], last[1], pre_last[0], pre_last[1]);
               
                pair<vector<int*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(last[0], last[1], color, 0, {}, board_layout, eat_direction);
    
                vector<int*> valid_moves2 = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                dama_nom = (eating_piece == 'd');

                if(dama_nom)
                {
                    for(auto move : valid_moves2)
                    {
                        //global malloc move
                        int* move2 = (int*)malloc(sizeof(int)*2);
                        move2[0] = move[0]; move2[1] = move[1];

                        vector<int*> new_value = vector<int*>((int)value.size());

                        //copy value
                        int i=0;
                        for(i=0; i<(int)value.size(); i++)
                        {
                            int * temp3 = new int[2];
                            temp3[0] = value[i][0];
                            temp3[1] = value[i][1];
                            new_value[i] = temp3;
                        }
                        new_value.push_back(move2); //add move
                        aux_list2.push_back(new_value);
                    }
                    parent_list = aux_list2;
                }
            }
            return parent_list;
        }
        else
        {
            vector <vector<int*>> maxp = vector <vector<int*>>();
            valid_moves = new_list;
            for (auto move : valid_moves)
            {
                int **new_board_layout = deepcopy2(board_layout);
                pair<int**, char> layoutAndDirection;

                layoutAndDirection = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color);
                
                new_board_layout = layoutAndDirection.first; //new
                eat_direction = layoutAndDirection.second; //new

                vector<vector<int*>> p2 = vector<vector<int*>>();
                p2 = eat_max2(move[0], move[1], new_board_layout, big_dama_list, color, eat_direction);

                // for(int i=0; i<(int)p2.size(); i++)
                // {
                //     for(int j=0; j<(int)p2[i].size(); j++)
                //     {
                //         std::cout<< p2[i][j][0]<<"-"<<p2[i][j][1]<<endl;
                //     }
                //     std::cout<<endl;
                // }
                // if(maxp.empty() || p2[0].size() > maxp[0].size())
                // {
                //     maxp = p2;
                // }

                /////////////////////////////////////////////
                
                if(maxp.empty())
                    maxp = p2;
                else{
                    if (int(p2[0].size())>int(maxp[0].size()))
                        maxp = p2;
                    else if (int(p2[0].size())==int(maxp[0].size()))
                        for (auto li : p2)
                            if (doesnt_have(maxp, li))
                                maxp.push_back(li);
                }
            }
            // for(auto l : maxp)
            //     for(auto value : l)
            //         std::cout<<value[0]<<"-"<<value[1]<<endl;
            // std::cout<<endl;
            return maxp;
        }
    }
    else
        return {};
    return {};
}

int evaluate_int(int* board_layout[8], int turn)
{
    movesSeen++;
    int sum = 0, balance_black = 0, balance_red = 0, all_pieces=0, red_pieces=0, black_pieces = 0;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            int piece = board_layout[i][j];

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

vector<int*> check_for_force(int* board_layout[8], int turn)
{
    bool aktar_flag = false;
    vector<int*> aktar_list = vector<int*>();
    vector<int*> force_list = vector<int*>();
    int maxLength = 3, listSize = 0;

    for(int row2=0; row2<8; row2++)
    {
        for(int col2 = 0; col2<8; col2++)
        {
            int color = board_layout[row2][col2];
            if(((color==2 || color==4) && turn==2) || ((color==1 || color==3) && turn==1))
            {
                bool fff = false;
                vector<vector<int*>> parent_list2;
                
                //get_valid_moves stuff
                pair<vector<int*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(row2, col2, color, 0, {}, board_layout, 0);
                vector<int*> valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                bool dama_nom = eating_piece == 'd';
                fff = eating_piece!=0;

                if(fff)
                {
                    //vector<int*> parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                    parent_list2 = vector<vector<int*>>();
                    for(int* move : valid_moves)
                    {
                        //parent_list2.append([(row2, col2), move])
                        int* temp = (int*)malloc(sizeof(int)*2);
                        vector<int*> tempValue = vector<int*>();
                        temp[0] = row2; temp[1] = col2;
                        tempValue.push_back(temp);
                        tempValue.push_back(move);

                        parent_list2.push_back(tempValue);
                    }

                    if(color == 1 || color == 2)
                    {
                        vector<vector<int*>> parent_list = vector<vector<int*>>();
                        parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                        if(!parent_list.empty())
                            listSize = parent_list[0].size();
                        if(!parent_list.empty() && listSize>=maxLength)
                        {
                            if(listSize>maxLength)
                                aktar_list.clear();
                            maxLength = listSize;
                            aktar_flag = true;
                            for(vector<int*> value : parent_list)
                                aktar_list.push_back(value[0]);
                            
                        }
                    }
                    else if (color == 3 || color == 4)
                    {
                        vector<vector<int*>> parent_list = vector<vector<int*>>();
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
                            for(vector<int*> value : parent_list)
                                aktar_list.push_back(value[0]);
                        }
                    }

                    if(!aktar_flag)
                    {
                        int* piece = (int*)malloc(sizeof(int)*2);
                        piece[0] = row2; piece[1] = col2;
                        force_list.push_back(piece);
                    }
                    
                }

            }
        }
    }

    if(aktar_flag)
        force_list = aktar_list;
    return force_list;
}
/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////
vector<int*> get_all_pieces(int* board_layout[8], int color)
{
    vector<int*> pieces = vector<int*>();
    
    int i, j;
    for(i=0; i<8; i++)
    {
        for(j=0; j<8; j++)
        {
            if( (color == 1 && (board_layout[i][j]==1 || board_layout[i][j] == 3)) || (color == 2 && (board_layout[i][j] == 2 || board_layout[i][j] == 4)) )
                {
                    //make piece
                    int* piece = (int*)malloc(sizeof(int)*2);
                    piece[0] = i;
                    piece[1] = j;
                    pieces.push_back(piece);
                    //cout<<"j: "<<pieces[i][j]<<endl;

                }
        }
    }
    return pieces;
}

int** move_piece(int* piece, int* move, int* board_layout[8], vector<vector<int*>> parent_list, int color)
{
    int row = piece[0];
    int col = piece[1];

    int r = move[0];
    int c = move[1];

    int listSize = parent_list.size();
    int index=0;
    int value_length;

    if(!parent_list.empty())
    {
        // for(auto eep : parent_list)
        //     for(auto eep2 : eep)
        //         cout<<eep2[0]<<'-'<<eep2[1]<<endl;
        for(vector<int*> value : parent_list)
        {
            value_length = value.size();
            auto temp = value[value_length-1];
            // cout<<"size of value: "<<sizeof(value[listSize-1])/sizeof(int)<<endl;
            if(temp[0]==r && temp[1]==c)
            {
                int z=0;
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

    int aux;
    aux = board_layout[row][col];
    board_layout[row][col]=board_layout[r][c];
    board_layout[r][c]=aux;

    //check for dama promotion
    if(!r && color==2)
        board_layout[r][c] = 4;
    if(r==7 && color==1)
        board_layout[r][c] = 3;
    
    return board_layout;
    
}

pair<vector<int**>, vector<int*>> get_all_moves(int* board_layout[8], int color)
{
    vector<int**> moves = vector<int**>();
    vector<int *> pieces;

    //create force list
    vector<int *> force_list;
    vector<int *> valid_moves;
    vector<vector<int *>> parent_list = vector<vector<int*>>();
    bool dama_nom = false, fff = false;

    force_list = check_for_force(board_layout, color);

    if(!force_list.empty())
        pieces = force_list;
    else
        pieces = get_all_pieces(board_layout, color);
    
    for(int* piece : pieces)
    {
        int color2 = board_layout[piece[0]][piece[1]];
        fff = dama_nom = false;
        if(color2 == 1 || color2 == 2)
        {
            pair<vector<int*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
            
            valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            fff = eating_piece!=0;

            if(fff)
            {
                vector<vector<int*>> parent_list2 = vector<vector<int*>>();
                for(int* move : valid_moves)
                {
                    //std::cout<<move[0]<<" "<<move[1]<<endl;
                    //parent_list2.append([(row2, col2), move])
                    int* temp = (int*)malloc(sizeof(int)*2);
                    vector<int*> tempValue = vector<int*>();
                    temp[0] = piece[0]; temp[1] = piece[1];
                    tempValue.push_back(temp);
                    tempValue.push_back(move);

                    parent_list2.push_back(tempValue);
                }

                parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                vector<int*> aux_list = vector<int*>();
                if(!parent_list.empty())
                {
                    for(vector<int*> value: parent_list)
                        aux_list.push_back(value[int(value.size()) - 1]);
                    valid_moves = aux_list;
                }

            }
        }

        else if (color2 ==3 || color2 == 4)
        {
            pair<vector<int*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, {}, board_layout, 0);
            
            vector<int*> valid_moves = validMovesAndEatingPiece.first;
            char eating_piece = validMovesAndEatingPiece.second;

            dama_nom = eating_piece=='d';

            if(dama_nom)
            {
                vector<vector<int*>> parent_list2 = vector<vector<int*>>();
                for(int* move : valid_moves)
                {
                    //parent_list2.append([(row2, col2), move])
                    int* temp = (int*)malloc(sizeof(int)*2);
                    vector<int*> tempValue = vector<int*>();
                    temp[0] = piece[0]; temp[1] = piece[1];
                    tempValue.push_back(temp);
                    tempValue.push_back(move);

                    parent_list2.push_back(tempValue);
                }

                parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, 0);
                vector<int*> aux_list = vector<int*>();
                if(!parent_list.empty())
                {
                    for(vector<int*> value: parent_list)
                        aux_list.push_back(value[int(value.size()) - 1]);
                    valid_moves = aux_list;
                }
            }
        }

        for(int* move : valid_moves)
        {
            //std::cout<<move[0]<<" "<<move[1]<<endl;
            int **temp_board_layout = deepcopy2(board_layout);
            // if(!fff && ! dama_nom)
            //     parent_list = {};
            temp_board_layout = move_piece(piece, move, temp_board_layout, parent_list, color2);
            moves.push_back(temp_board_layout);
        }
    }

    return make_pair(moves, force_list);

}

void printBoard(int** board)
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

pair<int, int**> minimax_pro2(int depth, int max_player, int* board_layout[8], int alpha, int beta, int akel_depth, int akel_player, int akling)
{
    int evaluation, maxEval, minEval;
    int** best_move;
    //  = (int**) malloc(sizeof(int*)*8);
    // for(int j=0; j<8; j++) best_move[j] = (int*)malloc(sizeof(int)*8);

    if(depth<=0)
    {
        int turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        
        // cout<<evaluate_int(board_layout, turn)<<endl;
        return make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<endl;
        // Sleep(500);

        maxEval = -100000;
        pair<vector <int**>, vector<int*> > allandForce = get_all_moves(board_layout, 1);
        vector <int**> all_moves;
        all_moves = allandForce.first;
        vector<int*> force_list = allandForce.second;

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
        // Sleep(500);

        minEval = 100000;
        pair<vector <int**>, vector<int*> > allandForce = get_all_moves(board_layout, 2);
        vector <int**> all_moves;
        all_moves = allandForce.first;
        vector<int*> force_list = allandForce.second;

        for (auto move : all_moves)
        {
            if (!force_list.empty() && akel_depth<5)
                evaluation = minimax_pro2(depth, true, move, alpha, beta, akel_depth+1, true, true).first;
            else
            {
                if( akel_player == true && akel_depth>2)
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
/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

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



    int* test2[8];  
    int array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    test2[0] = array;
    int array2[8] = {0, 1, 1, 1, 1, 1, 1, 1};
    test2[1] = array2;
    int array3[8] = {0, 0, 1, 0, 0, 1, 1, 1};
    test2[2] = array3;
    int array4[8] = {1, 1, 0, 1, 0, 0, 0, 0};
    test2[3] = array4;
    int array5[8] = {0, 0, 0, 2, 0, 2, 2, 2};
    test2[4] = array5;
    int array6[8] = {2, 2, 2, 0, 2, 0, 0, 0};
    test2[5] = array6;
    int array7[8] = {2, 2, 2, 2, 2, 2, 2, 2};
    test2[6] = array7;
    int array8[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    test2[7] = array8;
    int** test3 = deepcopy2(test2);
    
    pair<int, int**> minimaxResult;
    
    clock_t begin = clock();
    // minimaxResult = minimax_pro2(7, true, test2, INT_MIN, INT_MAX, 0, true, false);
    clock_t end = clock();
    // std::cout<<minimaxResult.first/100.0<<endl;
    // int** boardResult = minimaxResult.second;
    // printBoard(boardResult);
    // cout<<movesSeen<<endl;

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

     int piece[2], move[2];
     piece[0] = 5; piece[1] = 4;
     move[0] = 5; move[1] = 2;

    //  std::cout<<test3[1][2]<<endl;

     vector<int*> parent_list5;
     int *g = new int[2], *h = new int[2], *l = new int[2], *p = new int[2];
     g[0]=4;g[1]=3;h[0]=2;h[1]=3;l[0]=1;l[1]=1;p[0]=1;p[1]=3;
     parent_list5.push_back(g);
     parent_list5.push_back(h);
    //  parent_list5.push_back(l);
    //  parent_list5.push_back(p);
    
     vector<vector<int*>> parent_list6;
     parent_list6.push_back(parent_list5);

    //  test3 = move_piece(piece, move, test2, parent_list6, 4);
     
    //  std::cout<<test3[1][2]<<endl;

    //  vector<int*> force_list = check_for_force(test2, 2);
     vector<vector<int*>> parent_list = eat_max2_not_dama(4, 3, test2, parent_list6, 2, 0);
    for(auto value : parent_list){
        for(auto l : value)
            std::cout<<l[0]<<"-"<<l[1]<<endl;
        cout<<endl;
    }

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

/*int main()
{

    double time_spent = 0.0;

    clock_t begin = clock();

    for(int i=0; i<1000000; i++)
    {
        int v_move[2] = {1, 2};
        vector<int*> valid_moves;
        valid_moves.push_back(v_move);
    }

    clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
    cout<< time_spent;

    return 0;
}*/