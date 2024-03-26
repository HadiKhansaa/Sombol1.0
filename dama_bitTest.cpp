#include "hashing.cpp"

//headers
// BitmaskBoard move_piece(char* piece, char* move, BitmaskBoard& board_layout, std::vector<std::vector<char*>>& parent_list, char color);
// std::pair<std::vector<char*>, bool> check_for_possible_capture(BitmaskBoard& board_layout, char turn);
bool check_if_piece_can_capture(char row, char col, BitmaskBoard& board_layout, char turn);
std::pair<int, BitmaskBoard> search(
    char depth, bool max_player, BitmaskBoard& board_layout,
    int alpha, int beta, char akel_player, char akel_depth, bool akling,
     std::unordered_map<BitmaskBoard, TranspositionTableValue>& transpositionTable);
//counters
int movesSeen = 0;
int counter = 0;
int cacheHits = 0;

// bool sakker_kesh(BitmaskBoard board_layout, char row, char col, char* v_move, char turn) {
//     char oppTurn;
//     if(turn == 1)
//         oppTurn = 2;
//     else
//         oppTurn = 1;
//     if(check_for_possible_capture(board_layout, oppTurn).second) //eza fi kesh
//     {
//         std::vector<std::vector<char*>> emptyV;
//         BitmaskBoard board_layout2 = move_piece(new char[2]{row, col}, v_move, board_layout, emptyV, turn);
//         if(!check_for_possible_capture(board_layout2, oppTurn).second) // eza tsakkar l kesh
//             return true;
//         else
//             return false;
//     }
//     return false;
// }

bool check_end_of_parent_list(std::vector<std::vector<char*>> parent_list, char* move){
    for (auto l : parent_list)
    {
        if (l[l.size() - 1][0] == move[0] && l[l.size() - 1][1] == move[1])
            return true;
    }
    return false;
}

// TODO: needs fixing its wrong
bool kol(BitmaskBoard& board_layout, char row, char col, char color){
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

bool check_dama_has_akel(char row, char col, char color, BitmaskBoard& board_layout, char eat_direction)
{
    bool ate_up = (eat_direction == 'u');
    bool ate_down = (eat_direction == 'd');
    bool ate_left = (eat_direction == 'l');
    bool ate_right = (eat_direction == 'r');

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
                    else
                        return true;
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
                    else
                        return true;
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
                    else
                        return true;
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
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }

        return false;
    }
    else
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
                    if (board_layout.get(row, col+i+j) != 0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
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
                    if (board_layout.get(row, col-i-j) != 0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
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
                    if (board_layout.get(row+i+j, col)!=0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
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
                    if (board_layout.get(row-i-j, col)!=0)
                    {
                        i=10;
                        break;
                    }
                    else
                        return true;
                    j+=1;
                }
            }
            i+=1;
        }
        return false;
    }
    return false;
}

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

std::pair<BitmaskBoard, char> eat_piece_if_possible(BitmaskBoard& board_layout, char row, char col, char r, char c, char color)
{
    char eat_direction = 0;
    char sumRowOver2;
    char sumColOver2;
    if (color == 1 || color == 2)
    {
        sumRowOver2 = (row + r)/2;
        sumColOver2 = (col+c)/2;
        
        eat_direction = get_eat_direction(row, col, r, c);
    }

    if (color == 1)
    {
        if (board_layout.check_index_has_whitePawn(sumRowOver2, col) || board_layout.check_index_has_whiteKing(sumRowOver2, col))
        {
            board_layout.clearPosition(sumRowOver2, col);
            return std::make_pair(board_layout, eat_direction);
        }
        else if (board_layout.check_index_has_whitePawn(row, sumColOver2) || board_layout.check_index_has_whiteKing(row, sumColOver2))
        {
            board_layout.clearPosition(row, sumColOver2);
            return std::make_pair(board_layout, eat_direction);
        }
    }
    else if (color == 2)
    {
        if (board_layout.check_index_has_blackPawn(sumRowOver2, col) || board_layout.check_index_has_blackKing(sumRowOver2, col))
        {
            board_layout.clearPosition(sumRowOver2, col);
            return std::make_pair(board_layout, eat_direction);
        }
        else if (board_layout.check_index_has_blackPawn(row, sumColOver2) || board_layout.check_index_has_blackKing(row, sumColOver2))
        {
            board_layout.clearPosition(row, sumColOver2);
            return std::make_pair(board_layout, eat_direction);
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
                    return std::make_pair(board_layout, eat_direction);
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
                    return std::make_pair(board_layout, eat_direction);
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
                    return std::make_pair(board_layout, eat_direction);
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
                    return std::make_pair(board_layout, eat_direction);
                }
            }
        }
    }
    return std::make_pair(board_layout, eat_direction);
}

bool equals(std::vector<char*>& a, std::vector<char*>& b)
{
    for(char i=0; i<(char)a.size(); i++)
        if(a[i][0]!=b[i][0] || a[i][1]!=b[i][1])
            return false;
    return true;
}

bool doesnt_have(std::vector <std::vector<char*>>& a, std::vector<char*>& b){
    for(auto element:a)
        if(equals(element, b))
            return false;
    return true;
}

std::vector<char*> better_parent_list(std::vector<char*>& a, std::vector<char*>& b, BitmaskBoard &board_layout, char color){
    int count1 = 0, count2 = 0;
    for(int i=0; i<a.size() - 1; i++){
        if(color == 2 || color == 4)
        {
            if(get_piece_to_eat(board_layout, a[i][0], a[i][1], a[i+1][0], a[i+1][1], color) == 3)
                count1+=1;
            if(get_piece_to_eat(board_layout, b[i][0], b[i][1], b[i+1][0], b[i+1][1], color) == 3)
                count2+=1;
        }
        else
        {
            if(get_piece_to_eat(board_layout, a[i][0], a[i][1], a[i+1][0], a[i+1][1], color) == 4)
                count1+=1;
            if(get_piece_to_eat(board_layout, b[i][0], b[i][1], b[i+1][0], b[i+1][1], color) == 4)
                count2+=1;
        }
    }
    
    if(count1>=count2)
        return a;
    return b;
}

std::vector<std::vector<char*>> deepcopy_parent_list(std::vector<std::vector<char*>>& parent_list){
    std::vector<std::vector<char*>> new_parent_list;
    for(auto element:parent_list){
        std::vector<char*> new_element;
        for(auto move:element){
            char* new_move = new char[2];
            new_move[0] = move[0];
            new_move[1] = move[1];
            new_element.push_back(new_move);
        }
        new_parent_list.push_back(new_element);
    }
    return new_parent_list;
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
            eat_direction = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color).second;
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
                        if(doesnt_have(maxp, li))
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

std::vector<std::vector<char*>> eat_max3(char row, char col, BitmaskBoard& board_layout, std::vector<std::vector<char*>>& parent_list, char color, char eat_direction) {
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, board_layout, eat_direction);
    bool dama_nom = (eating_piece == 'd');
    
    std::vector<char*> new_list;
    if (dama_nom) {
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
            // auto  eating_piece = get_valid_moves(move[0], move[1], color, 0, board_layout, eat_direction).second;

            // dama_nom = (eating_piece == 'd');
            dama_nom = check_dama_has_akel(move[0], move[1], color, board_layout, eat_direction);

            if (dama_nom) {
                new_list.push_back(move);
                for (auto value : parent_list) {
                    if (row == value.back()[0] && col == value.back()[1]) {
                        std::vector<char*> big_dama = value;
                        big_dama.push_back(move);
                        big_dama_list.push_back(big_dama);
                    }
                }
            }
        }
        if (!big_dama_list.empty()) {
            parent_list = big_dama_list;
            // big_dama_list.clear();
        }
        if (new_list.empty()) {   
            std::vector<std::vector<char*>> aux_list2;
            aux_list2.clear();
            char counter = 0;
            for (std::vector<char*> value : parent_list) {
                char* last = value.back();
                char* pre_last = value[char(value.size())-2];

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
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0], last[1], color, 0, board_layout, eat_direction);

                dama_nom = (eating_piece == 'd');

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
            }
            // if(!aux_list2.empty())
            //     return aux_list2;
            parent_list = aux_list2;
            return parent_list;

        } else {

            //else
            valid_moves = new_list;
            std::vector<std::vector<char*>> maxp;
            maxp.clear();
            for (auto move : valid_moves) {
                BitmaskBoard board_layout2 = board_layout;
                auto [new_board_layout, eat_direction] = eat_piece_if_possible(board_layout2, row, col, move[0], move[1], color);
                auto p2 = eat_max3(move[0], move[1], new_board_layout, parent_list, color, eat_direction);
                
                if (maxp.empty()) {
                    maxp = p2;
                } else {
                    if (!p2.empty() && p2[0].size() > maxp[0].size()) {
                        maxp = p2;
                    } else if (!p2.empty() && (p2[0].size() == maxp[0].size())) { 
                        for (auto li : p2) 
                            if (doesnt_have(maxp, li)) 
                                maxp.push_back(li);
                    }
                }
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty std::vector
    }
}

std::vector<std::vector<char*>> eat_max2_not_dama(char row, char col, BitmaskBoard& board_layout,  std::vector<std::vector<char*>>& parent_list, char color, char eat_direction) {
    // Call get_valid_moves function
    auto [valid_moves, eating_piece] = get_valid_moves(row, col, color, 0, board_layout, eat_direction);
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
            auto eating_piece = get_valid_moves(move[0], move[1], color, 0, board_layout, eat_direction).second;
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
            for (std::vector<char*> value : parent_list) {
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
                auto [valid_moves2, eating_piece] = get_valid_moves(last[0], last[1], color, 0, board_layout, eat_direction);

                dama_nom = (eating_piece!=0);

                if (dama_nom) {
                    for (auto move : valid_moves2) {
                        std::vector<char*> aux_list3 = value;
                        aux_list3.push_back(move);
                        aux_list2.push_back(aux_list3);
                    }
                }
            }
            if(!aux_list2.empty())
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
                else if (maxp[0].size() == p2[0].size()) // new
                {
                    for(auto li:p2)
                        if(doesnt_have(maxp, li))
                            maxp.push_back(li);
                }
            }
            return maxp;
        }
    } else {
        return {};  // Return an empty std::vector
    }
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

// bitwise optimized: tested
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

std::pair<std::vector<char*>, bool> check_for_possible_capture(BitmaskBoard& board_layout, char turn)
{
    std::vector<char*> pieces = get_all_pieces(board_layout, turn);
    for(auto piece:pieces){
        char row = piece[0];
        char col = piece[1];
        if((board_layout.check_index_has_blackKing(row, col) && turn==1) || (board_layout.check_index_has_whiteKing(row, col) && turn == 2))
        {
            char eating_piece = get_valid_moves(row, col, board_layout.get(row, col), 0, board_layout, 0).second;
            if(eating_piece!=0)
                return std::make_pair(pieces, true);
        }
        else if(turn == 2){
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_blackPawn(row, col-1) || board_layout.check_index_has_blackKing(row, col-1)))
                return std::make_pair(pieces, true);
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_blackPawn(row, col+1) || board_layout.check_index_has_blackKing(row, col+1)))
                return std::make_pair(pieces, true);
            if(row-2>=0 && board_layout.get(row-2, col)==0 && (board_layout.check_index_has_blackPawn(row-1, col) || board_layout.check_index_has_blackKing(row-1, col)))
                return std::make_pair(pieces, true);
        }
        else{
            if(col-2>=0 && board_layout.get(row, col-2)==0 && (board_layout.check_index_has_whitePawn(row, col-1) || board_layout.check_index_has_whiteKing(row, col-1)))
                return std::make_pair(pieces, true);
            if(col+2<8 && board_layout.get(row, col+2)==0 && (board_layout.check_index_has_whitePawn(row, col+1) || board_layout.check_index_has_whiteKing(row, col+1)))
                return std::make_pair(pieces, true);
            if(row+2<8 && board_layout.get(row+2, col)==0 && (board_layout.check_index_has_whitePawn(row+1, col) || board_layout.check_index_has_whiteKing(row+1, col)))
                return std::make_pair(pieces, true);
        }
    }
    return std::make_pair(pieces, false);
}

bool check_if_piece_can_capture(char row, char col, BitmaskBoard& board_layout, char turn){

    if((board_layout.check_index_has_blackKing(row, col) && turn==1) || (board_layout.check_index_has_whiteKing(row, col) && turn == 2))
    {
        char eating_piece = get_valid_moves(row, col, board_layout.get(row, col), 0, board_layout, 0).second;
        if(eating_piece!=0)
            return true;
        return false;
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

std::vector<char*> check_for_force(BitmaskBoard& board_layout, char turn, std::vector<char*> *pieces = { })
{
    auto [pieces2, check] = check_for_possible_capture(board_layout, turn);
    *pieces = pieces2;
    if(check == false)
        return {};
    bool aktar_flag = false;
    std::vector<char*> aktar_list;
    std::unordered_set<char*> aktar_set;
    std::vector<char*> force_list;
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
            std::vector<std::vector<char*>> parent_list2;
            
            //get_valid_moves stuff
            std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
            validMovesAndEatingPiece  = get_valid_moves(row2, col2, color, 0, board_layout, 0);
            
            char eating_piece = validMovesAndEatingPiece.second;
            bool dama_nom = (eating_piece == 'd');
            fff = eating_piece!=0;

            if(fff)
            {
                std::vector<char*> valid_moves = validMovesAndEatingPiece.first;
                //std::vector<char*> parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, 0);
                if(color == 1 || color == 2)
                {
                    parent_list2 = std::vector<std::vector<char*>>();
                    char* temp = new char[2];
                    std::vector<char*> tempValue;
                    temp[0] = row2; temp[1] = col2;
                    tempValue.push_back(temp);
                    parent_list2.push_back(tempValue);
                    // for(char* move : valid_moves)
                    // {
                    //     //parent_list2.append([(row2, col2), move])
                    //     char* temp = new char[2];
                    //     std::vector<char*> tempValue = std::vector<char*>();
                    //     temp[0] = row2; temp[1] = col2;
                    //     tempValue.push_back(temp);
                    //     tempValue.push_back(move);
                    //     parent_list2.push_back(tempValue);
                    // }

                    std::vector<std::vector<char*>> parent_list;
                    parent_list = eat_max2(row2, col2, board_layout, parent_list2, color, 0);
                    if(!parent_list.empty())
                        listSize = parent_list[0].size();
                    if(!parent_list.empty() && listSize>=maxLength)
                    {
                        if(listSize>maxLength){
                            aktar_set.clear();
                            // force_list.clear();
                        }
                        maxLength = listSize;
                        aktar_flag = true;
                        for(std::vector<char*> value : parent_list)
                            aktar_set.insert(value[0]);
                        
                    }
                }
                else if (color == 3 || color == 4)
                {
                    parent_list2 = std::vector<std::vector<char*>>();
                    std::vector<char*> tempValue = std::vector<char*>();
                    char* temp = new char[2];
                    temp[0] = row2; temp[1] = col2;
                    tempValue.push_back(temp);
                    parent_list2.push_back(tempValue);

                    // for(char* move : valid_moves)
                    // {
                    //     //parent_list2.append([(row2, col2), move])
                    //     char* temp = new char[2];
                    //     std::vector<char*> tempValue = std::vector<char*>();
                    //     temp[0] = row2; temp[1] = col2;
                    //     tempValue.push_back(temp);
                    //     tempValue.push_back(move);
                    //     parent_list2.push_back(tempValue);
                    // }

                    std::vector<std::vector<char*>> parent_list;
                    parent_list = eat_max2(row2, col2, board_layout, parent_list2, color, 0);
                    if(!parent_list.empty())
                        listSize = parent_list[0].size();
                    if(!parent_list.empty() && listSize>=maxLength)
                    {
                        if(listSize>maxLength){
                            aktar_set.clear();
                            // force_list.clear();
                        }
                        maxLength = listSize;
                        aktar_flag = true;
                        for(std::vector<char*> value : parent_list)
                            aktar_set.insert(value[0]);
                    }
                }

                if(!aktar_flag)
                {
                    char* piece = new char[2];
                    piece[0] = row2; piece[1] = col2;
                    force_list.push_back(piece);
                    // aktar_set.insert(piece);
                }
            }
        }
    }
    if(aktar_flag)
    {
        force_list.clear();
        for(auto element : aktar_set)
        {
            force_list.push_back(element);
        }
        // force_list = aktar_list;
    }
    return force_list;
}
/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

BitmaskBoard move_piece(char* piece, char* move, BitmaskBoard& board_layout, std::vector<std::vector<char*>>& parent_list, char color)
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
            auto temp = value[value_length-1];
            if(temp[0]==r && temp[1]==c)
            {
                char z=0;
                for(z = 0; z<value_length - 1; z++)
                    eat_piece_if_possible(board_layout, value[z][0], value[z][1], value[z+1][0], value[z+1][1], color).first;
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

    if(aux == 1)
        board_layout.set_blackPawn(r, c);
    else if(aux == 2)
        board_layout.set_whitePawn(r, c);
    else if(aux == 3)
        board_layout.set_blackKing(r, c);
    else if(aux == 4)
        board_layout.set_whiteKing(r, c);

    board_layout.clearPosition(row, col);

    //check for dama promotion
    if(r==0 && color==2)
        board_layout.set_whiteKing(r, c);
    if(r==7 && color==1)
        board_layout.set_blackKing(r, c);
    
    return board_layout;
}

char evaluate_int2(BitmaskBoard& board_layout, char turn)
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

int count_all_pieces(BitmaskBoard& board_layout, char turn)
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

int evaluate_int(BitmaskBoard& board_layout, char turn)
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
    //             }
    //             else if(piece==4)
    //             {
    //                 red_pieces+=1;
    //                 sum-=450;
    //             }
    //         }
    //     }

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

std::pair<std::vector<BitmaskBoard>, bool> get_all_moves(BitmaskBoard& board_layout, char color)
{
    std::vector<BitmaskBoard> moves;
    // std::unordered_set<BitmaskBoard> moves_set;
    std::vector<char *> pieces;
    counter++;
    //create force list
    std::vector<char *> force_list;
    std::vector<char *> valid_moves;
    bool forceListEmpty = false;
    std::vector<std::vector<char *>> parent_list;
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
            BitmaskBoard board_layout2 = board_layout;
            move_piece(piece, move, board_layout2, parent_list, color2);
            // if(moves_set.find(board_layout2) == moves_set.end())
            // {
            //     moves_set.insert(board_layout2);
            //     moves.push_back(board_layout2);
            // }
            moves.push_back(board_layout2);
        }
    }

    return std::make_pair(moves, forceListEmpty);
}

void printBoard(BitmaskBoard& board_layout)
{
    for(char i=0; i<8; i++)
    {
        for(char j=0;j<8; j++)
        {
            std::cout<<int(board_layout.get(i, j))<<" ";
        }
        std::cout<<"\n";
    }
}

bool playerWon(BitmaskBoard& board_layout){
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

void sort_moves(std::vector<BitmaskBoard> *moves, char turn)
{
    return ;
    // cout<<"\n";
}

std::pair<int, BitmaskBoard> minimax_pro2(char depth, char max_player, BitmaskBoard& board_layout, int alpha, int beta, char akel_depth, char akel_player, char akling)
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

        turn == 1 ? board_layout.setTurn(0) : board_layout.setTurn(1);
        return std::make_pair(board_layout.evaluate_board(), board_layout);
    }

    if(max_player)
    {
        maxEval = INT_MIN;
        
        std::pair<std::vector <BitmaskBoard>, bool> allandForce = get_all_moves(board_layout, 1);
        std::vector <BitmaskBoard> all_moves = allandForce.first;
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

            alpha = std::max(alpha, maxEval);
            if(beta<=alpha)
                break;
        }
        return std::make_pair(maxEval, best_move);
    }
    else
    {
        minEval = INT_MAX;

        std::pair<std::vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2);
        std::vector <BitmaskBoard> all_moves = allandForce.first;
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

            beta = std::min(beta, minEval);
            if(beta<=alpha)
                break;
        }
        return std::make_pair(minEval, best_move);
    }
}

std::pair<int, BitmaskBoard> normal_minimax(char depth, char max_player, BitmaskBoard& board_layout, int alpha, int beta)
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
            // cout<<"found in hash table"<<"\n";
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
        return std::make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<"\n";
        // Sleep(1000);

        maxEval = INT_MIN;
        std::pair<std::vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 1);
        std::vector <BitmaskBoard> all_moves = allandForce.first;
        bool force_list = allandForce.second;

        for (auto move : all_moves)
        {
            evaluation = normal_minimax(depth-1, false, move, alpha, beta).first;
            if(evaluation>maxEval)
            {
                maxEval = evaluation;
                best_move = move;
            }

            alpha = std::max(alpha, maxEval);
            if(beta<=alpha)
                break;

        }
        transpositionTable[hashKey] = std::make_pair(depth, maxEval);
        return std::make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<"\n";
        // Sleep(1000);

        minEval = INT_MAX;
        std::pair<std::vector <BitmaskBoard>, bool > allandForce = get_all_moves(board_layout, 2);
        std::vector <BitmaskBoard> all_moves = allandForce.first;
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

            beta = std::min(beta, minEval);
            if(beta<=alpha)
                break;
        }
        transpositionTable[hashKey] = std::make_pair(depth, minEval);
        return std::make_pair(minEval, best_move);
    }
}

std::pair<int, BitmaskBoard> normal_minimax_based(char depth, char max_player, BitmaskBoard& board_layout, int alpha, int beta)
{   
    
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    // board_layout.setTurn(max_player ? 0 : 1);
    // if (transpositionTable2.count(board_layout) > 0) {
    //     TranspositionTableValue value = transpositionTable2[board_layout];

    //     char storedDepth = value.getDepth();
    //     int storedEval = value.getEval();
    //     BitmaskBoard storedBoard = value.getBestMove();

    //     if (storedDepth >= depth)
    //     {
    //         cacheHits++;
    //         movesSeen++;
    //         return std::make_pair(storedEval, storedBoard);
    //     }
    // }

    if(depth <=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        int eval = evaluate_int(board_layout, turn);
        // transpositionTable[hashKey] = std::make_pair(0, eval);
        return std::make_pair(eval, board_layout);
        // return std::make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    

    if(max_player)
    {

        maxEval = INT_MIN;
        std::vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        
        std::vector<char *> pieces;
        counter++;
        movesSeen++;
        //create force list
        std::vector<char *> force_list;
        std::vector<char *> valid_moves;
        bool forceListEmpty = false;
        std::vector<std::vector<char *>> parent_list;
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
                std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    bool fff2 = false;
                    std::vector<std::vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        std::vector<char*> tempValue = std::vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                        if(check_if_piece_can_capture(move[0], move[1], board_layout, color2))
                            fff2 = true;
                    }

                    if(fff2)
                    {
                        parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
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

                dama_nom = eating_piece=='d';

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

            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                move_piece(piece, move, board_layout2, parent_list, color2);

                evaluation = normal_minimax_based(depth-1, false, board_layout2, alpha, beta).first;
                
                if(evaluation>maxEval)
                {
                    maxEval = evaluation;
                    best_move = board_layout2;
                }

                alpha = std::max(alpha, maxEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
            }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            
            if(exit){
                break;
            }
        }
        // board_layout.setTurn(1);
        // TranspositionTableValue value(maxEval, depth, best_move);
        // transpositionTable2[board_layout] = value;
        return std::make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<"\n";
        // Sleep(1000);
        movesSeen++;

        minEval = INT_MAX;
        std::vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        std::vector<char *> pieces;
        counter++;
        //create force list
        std::vector<char *> force_list;
        std::vector<char *> valid_moves;
        bool forceListEmpty = false;
        std::vector<std::vector<char *>> parent_list;
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
                std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    bool fff2 = false;
                    std::vector<std::vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        std::vector<char*> tempValue = std::vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                        if(check_if_piece_can_capture(move[0], move[1], board_layout, color2))
                            fff2 = true;
                    }

                    if(fff2)
                    {
                        parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
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

                dama_nom = eating_piece=='d';

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
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                move_piece(piece, move, board_layout2, parent_list, color2);

                evaluation = normal_minimax_based(depth-1, true, board_layout2, alpha, beta).first;
                
                if(evaluation<minEval)
                {
                    minEval = evaluation;
                    best_move = board_layout2;
                }

                beta = std::min(beta, minEval);
                if(beta<=alpha){
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
                
            }
            if(exit)
                break;
            

        }


        // board_layout.setTurn(0);
        // TranspositionTableValue value(minEval, depth, best_move);
        // transpositionTable2[board_layout] = value;
        return std::make_pair(minEval, best_move);
    }
}

std::pair<int, BitmaskBoard> minimax_pro2_based(char depth, char max_player, BitmaskBoard& board_layout, int alpha, int beta, char akel_player, char akel_depth, char akling)
{   

    // BitmaskBoard test_board;

    // // create this board in test_board
    // // 0 0 0 0 0 0 0 0
    // // 1 1 1 1 0 1 1 1
    // // 0 0 0 0 0 0 1 1
    // // 1 1 1 0 0 0 0 1
    // // 0 0 0 0 4 2 2 2
    // // 2 2 2 0 2 0 2 2
    // // 2 2 2 2 0 0 2 2
    // // 0 0 0 0 0 0 0 0

    // // test_board.set(0, 4, 4);
    // test_board.set(4, 4, 4);

    // test_board.set(1, 0, 1);
    // test_board.set(1, 1, 1);
    // test_board.set(1, 2, 1);
    // test_board.set(1, 3, 1);
    // test_board.set(1, 5, 1);
    // test_board.set(1, 6, 1);
    // test_board.set(1, 7, 1);
    // // test_board.set(2, 5, 1);
    // test_board.set(2, 6, 1);
    // test_board.set(2, 7, 1);
    // test_board.set(3, 0, 1);
    // test_board.set(3, 1, 1);
    // test_board.set(3, 2, 1);
    // test_board.set(3, 7, 1);
    // // test_board.set(4, 4, 2);
    // test_board.set(4, 5, 2);
    // test_board.set(4, 6, 2);
    // test_board.set(4, 7, 2);
    // test_board.set(5, 0, 2);
    // test_board.set(5, 1, 2);
    // test_board.set(5, 2, 2);
    // test_board.set(5, 4, 2);
    // test_board.set(5, 6, 2);
    // test_board.set(5, 7, 2);
    // test_board.set(6, 0, 2);
    // test_board.set(6, 1, 2);
    // test_board.set(6, 2, 2);
    // test_board.set(6, 3, 2);
    // test_board.set(6, 6, 2);
    // test_board.set(6, 7, 2);
    
    int evaluation, maxEval, minEval;
    BitmaskBoard best_move;

    board_layout.setTurn(max_player ? 0 : 1);
    if (transpositionTable2.count(board_layout) > 0) {
        TranspositionTableValue value = transpositionTable2[board_layout];

        char storedDepth = value.getDepth();
        int storedEval = value.getEval();
        BitmaskBoard storedBoard = value.getBestMove();

        if (storedDepth >= depth)
        {
            cacheHits++;
            movesSeen++;
            return std::make_pair(storedEval, storedBoard);
        }
    }

    if(depth <=0 || playerWon(board_layout))
    {
        char turn;
        if(max_player)
            turn = 1;
        else
            turn = 2;
        int eval = evaluate_int(board_layout, turn);
        // transpositionTable[hashKey] = std::make_pair(0, eval);
        return std::make_pair(eval, board_layout);
        // return std::make_pair(evaluate_int(board_layout, turn), board_layout);
    }

    

    if(max_player)
    {
        // printBoard(board_layout);
        // cout<<"\n";
        // Sleep(1000);

        maxEval = INT_MIN;
        std::vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        
        std::vector<char *> pieces;
        counter++;
        movesSeen++;
        //create force list
        std::vector<char *> force_list;
        std::vector<char *> valid_moves;
        bool forceListEmpty = false;
        std::vector<std::vector<char *>> parent_list;
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
                std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    bool fff2 = false;
                    std::vector<std::vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        std::vector<char*> tempValue = std::vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                        if(check_if_piece_can_capture(move[0], move[1], board_layout, color2))
                            fff2 = true;
                    }

                    if(fff2)
                    {
                        parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
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

                dama_nom = eating_piece=='d';

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

            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                move_piece(piece, move, board_layout2, parent_list, color2);

                if (!force_list.empty() && akel_depth<5)
                    evaluation = minimax_pro2_based(depth, false, board_layout2, alpha, beta, akel_depth+1, false, true).first;
                else
                {
                    if(akel_player == false && akel_depth>2)
                        evaluation = minimax_pro2_based(0, false, board_layout2, alpha, beta, 100, false, false).first;
                    else
                        evaluation = minimax_pro2_based(depth-1, false, board_layout2, alpha, beta, 0, false, false).first;
                }

                if(evaluation>maxEval)
                {
                    maxEval = evaluation;
                    best_move = board_layout2;
                }

                alpha = std::max(alpha, maxEval);
                if(beta<=alpha)
                {
                    exit = true;
                    break;
                }
            }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
            
            if(exit){
                break;
            }
        }

        // free memory
        // for (auto move : valid_moves)
        //     delete move;
        
        // for (auto piece : pieces)
        //     delete piece;
        
        // for (auto pos : force_list)
        //     delete pos;
        
        // for (std::vector<char*> list : parent_list){
        //     for (char* pos : list)
        //         delete pos;
        // }

        board_layout.setTurn(1);
        TranspositionTableValue value(maxEval, depth, best_move);
        transpositionTable2[board_layout] = value;
        return std::make_pair(maxEval, best_move);
    }
    else
    {
        // printBoard(board_layout);
        // cout<<"\n";
        // Sleep(1000);
        movesSeen++;

        minEval = INT_MAX;
        std::vector <BitmaskBoard> all_moves;

        ////////////////////////////////////////
        std::vector<char *> pieces;
        counter++;
        //create force list
        std::vector<char *> force_list;
        std::vector<char *> valid_moves;
        bool forceListEmpty = false;
        std::vector<std::vector<char *>> parent_list;
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
                std::pair<std::vector<char*>, char> validMovesAndEatingPiece;
                validMovesAndEatingPiece  = get_valid_moves(piece[0], piece[1], color2, 0, board_layout, 0);
                
                valid_moves = validMovesAndEatingPiece.first;
                char eating_piece = validMovesAndEatingPiece.second;

                fff = eating_piece!=0;

                if(fff)
                {
                    bool fff2 = false;
                    std::vector<std::vector<char*>> parent_list2;
                    for(char* move : valid_moves)
                    {
                        std::vector<char*> tempValue = std::vector<char*>();
                        tempValue.push_back(piece);
                        tempValue.push_back(move);
                        parent_list2.push_back(tempValue);
                        if(check_if_piece_can_capture(move[0], move[1], board_layout, color2))
                            fff2 = true;
                    }

                    if(fff2)
                    {
                        parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, 0);
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

                dama_nom = eating_piece=='d';

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
            bool exit = false;
            for(char* move : valid_moves)
            {
                BitmaskBoard board_layout2 = board_layout;
                move_piece(piece, move, board_layout2, parent_list, color2);
                if (!force_list.empty() && akel_depth<5)
                    evaluation = minimax_pro2_based(depth, true, board_layout2, alpha, beta, akel_depth+1, true, true).first;
                else
                {
                    if((akel_player == true) && akel_depth>2)
                        evaluation = minimax_pro2_based(0, true, board_layout2, alpha, beta, 100, true, false).first;
                    else
                        evaluation = minimax_pro2_based(depth-1, true, board_layout2, alpha, beta, 0, true, false).first;
                }

                if(evaluation<minEval)
                {
                    minEval = evaluation;
                    best_move = board_layout2;
                }

                beta = std::min(beta, minEval);
                if(beta<=alpha){
                    exit = true;
                    break;
                }
                // moves->push_back(move_piece(piece, move, deepcopy2(board_layout), parent_list, color2));
                
            }
            if(exit)
                break;
            

        }


        // free memory
        // for (auto move : valid_moves)
        //     delete move;
        
        // for (auto piece : pieces)
        //     delete piece;
        
        // for (auto pos : force_list)
        //     delete pos;
        
        // for (std::vector<char*> list : parent_list){
        //     for (char* pos : list)
        //         delete pos;
        // }

        // if there's already an entry for this board
        board_layout.setTurn(0);
        TranspositionTableValue value(minEval, depth, best_move);
        transpositionTable2[board_layout] = value;
        return std::make_pair(minEval, best_move);
    }
}



void order_moves(
    BitmaskBoard& board, 
    std::vector<BitmaskBoard>& moves, 
    std::unordered_map<BitmaskBoard, TranspositionTableValue>& transpositionTable, 
    char previousDepth, bool isMaxPlayer) 
{

    // Sorting lambda function
    auto compareMoves = [&transpositionTable, &previousDepth, isMaxPlayer](BitmaskBoard& a, BitmaskBoard& b) -> bool {
        
        a.setTurn(isMaxPlayer ? 1 : 0);
        b.setTurn(isMaxPlayer ? 1 : 0);
        auto itA = transpositionTable.find(a);
        auto itB = transpositionTable.find(b);

        // If both moves are found, compare their evaluations
        if (itA != transpositionTable.end() && itB != transpositionTable.end()) {
            // Prioritize higher evaluations for simplicity; adjust based on your game's needs
            // heuristic that prefers the move that was found at a deeper depth
            if(isMaxPlayer)
                return itA->second.getEval() > itB->second.getEval();
            else
                return itA->second.getEval() < itB->second.getEval();
            // return itA->second.getEval() > itB->second.getEval();
        }
        // If only one move is found, prioritize it
        else if (itA != transpositionTable.end()) {
            // try to find b in the opposite turn
            b.setTurn(isMaxPlayer ? 0 : 1);
            auto itB = transpositionTable.find(b);
            if(itB != transpositionTable.end())
            {
                if(isMaxPlayer)
                    return itA->second.getEval() > itB->second.getEval();
                else
                    return itA->second.getEval() < itB->second.getEval();
            }
            
            return true;
        }
        else if (itB != transpositionTable.end()) {
            // try to find a in the opposite turn
            a.setTurn(isMaxPlayer ? 0 : 1);
            auto itA = transpositionTable.find(a);
            if(itA != transpositionTable.end())
            {
                if(isMaxPlayer)
                    return itA->second.getEval() > itB->second.getEval();
                else
                    return itA->second.getEval() < itB->second.getEval();
            }
            return false;
        }
        // If neither move is found, do simple evaluation of both boards

        return false;
    };

    // Sort moves based on their evaluations stored in the transposition table
    std::sort(moves.begin(), moves.end(), compareMoves);
}

std::pair<int, BitmaskBoard> search(
    char depth, bool max_player, BitmaskBoard& board_layout,
    int alpha, int beta, char akel_player, char akel_depth, bool akling,
     std::unordered_map<BitmaskBoard, TranspositionTableValue>& transpositionTable)
{

    // First, check if this board state is already in the transposition table
    board_layout.setTurn(max_player ? 0 : 1);
    auto it = transpositionTable.find(board_layout);
    if(it != transpositionTable.end()) {
        TranspositionTableValue& ttValue = it->second;
        // Ensure that the depth stored in the transposition table is at least as deep as the current search depth
        if(ttValue.getDepth() >= depth) {
            // If so, use the stored evaluation and potentially best move
            cacheHits++; // Assuming cacheHits is a metric you're tracking
            return std::make_pair(ttValue.getEval(), ttValue.getBestMove());
        }
    }

    // Base case remains the same
    if(depth <= 0 || board_layout.player_won()) {
        movesSeen++;
        int eval = board_layout.evaluate_board(); // Assuming evaluate_board is defined elsewhere
        return {eval, board_layout};
    }

    auto [moves, isEmptyForceList] = get_all_moves(board_layout, max_player ? 1 : 2);

    // Order moves based on transposition table
    order_moves(board_layout, moves, transpositionTable, depth - 1, max_player);

    int bestEval = max_player ? INT_MIN : INT_MAX;
    BitmaskBoard bestMove;

    for(auto move : moves) {
        int eval;
        BitmaskBoard resultBoard;

        // Recursively call minimax on each move
        if (!isEmptyForceList && akel_depth<5)
            eval = search(depth, !max_player, move, alpha, beta, akel_depth+1, !max_player, true, transpositionTable).first;
        else
        {
            if(!max_player && akel_depth>2)
                eval = max_player ? INT_MIN : INT_MAX; // throw line
                // eval = search(0, !max_player, move, alpha, beta, 100, !max_player, false, transpositionTable).first;
            else
                eval = search(depth-1, !max_player, move, alpha, beta, 0, !max_player, false, transpositionTable).first;
        }

        if(max_player) {
            if(eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
            alpha = std::max(alpha, bestEval);
        } else {
            if(eval < bestEval) {
                bestEval = eval;
                bestMove = move;
            }
            beta = std::min(beta, bestEval);
        }

        if(beta <= alpha) {
            break; // Alpha-beta pruning
        }
    }

    // Update transposition table if needed
    if(!transpositionTable.count(board_layout) || transpositionTable[board_layout].getDepth() < depth) {
        TranspositionTableValue value(bestEval, depth, bestMove);
        transpositionTable[board_layout] = value;
    }

    return {bestEval, bestMove};
}


void order_moves2(
    BitmaskBoard& board, 
    std::vector<BitmaskBoard>& moves, 
    std::unordered_map<BitmaskBoard, TTValue>& transpositionTable, 
    char previousDepth, bool isMaxPlayer) 
{

    // Sorting lambda function
    auto compareMoves = [&transpositionTable, &previousDepth, isMaxPlayer](BitmaskBoard& a, BitmaskBoard& b) -> bool {

        a.setTurn(isMaxPlayer ? 1 : 0);
        b.setTurn(isMaxPlayer ? 1 : 0);
        auto itA = transpositionTable.find(a);
        auto itB = transpositionTable.find(b);

        // If both moves are found, compare their evaluations
        if (itA != transpositionTable.end() && itB != transpositionTable.end()) {
            // Prioritize higher evaluations for simplicity; adjust based on your game's needs
            // heuristic that prefers the move that was found at a deeper depth
            if(isMaxPlayer)
                return itA->second.eval > itB->second.eval;
            else
                return itA->second.eval < itB->second.eval;
            // return itA->second.getEval() > itB->second.getEval();
        }
        // If only one move is found, prioritize it
        else if (itA != transpositionTable.end()) {
            // try to find b in the opposite turn
            b.setTurn(isMaxPlayer ? 0 : 1);
            auto itB = transpositionTable.find(b);
            if(itB != transpositionTable.end())
            {
                if(isMaxPlayer)
                    return itA->second.eval > itB->second.eval;
                else
                    return itA->second.eval < itB->second.eval;
            }
            
            return true;
        }
        else if (itB != transpositionTable.end()) {
            // try to find a in the opposite turn
            a.setTurn(isMaxPlayer ? 0 : 1);
            auto itA = transpositionTable.find(a);
            if(itA != transpositionTable.end())
            {
                if(isMaxPlayer)
                    return itA->second.eval > itB->second.eval;
                else
                    return itA->second.eval < itB->second.eval;
            }
            return false;
        }
        // If neither move is found, do simple evaluation of both boards

        return false;
    };

    // Sort moves based on their evaluations stored in the transposition table
    std::sort(moves.begin(), moves.end(), compareMoves);
}

int search2(
    char depth, bool max_player, BitmaskBoard& board_layout,
    int alpha, int beta, char akel_player, char akel_depth, bool akling,
     std::unordered_map<BitmaskBoard, TTValue>& transpositionTable,
      BitmaskBoard& best_move, char maxDepth, std::unordered_map<uint64_t, int>& gameHistory)
{
    int total_depth = depth + akel_depth;

    // First, check if this board state is already in the transposition table
    board_layout.setTurn(max_player ? 0 : 1);
    auto it = transpositionTable.find(board_layout);
    if(!(depth == maxDepth && akel_depth == 0) && it != transpositionTable.end()) {
        TTValue& ttValue = it->second;
        // Ensure that the depth stored in the transposition table is at least as deep as the current search depth
        if(ttValue.depth >= total_depth) {
            // If so, use the stored evaluation and potentially best move
            cacheHits++; // Assuming cacheHits is a metric you're tracking
            return ttValue.eval;
        }
    }
    // Base case remains the same
    if(depth <= 0 || board_layout.player_won()) {
        movesSeen++;
        int eval = board_layout.evaluate_board(gameHistory); // Assuming evaluate_board is defined elsewhere
        // TTValue value = {depth, eval};
        // transpositionTable[board_layout] = value;
        return eval;
    }

    auto [moves, isEmptyForceList] = get_all_moves(board_layout, max_player ? 1 : 2);

    // Order moves based on transposition table
    order_moves2(board_layout, moves, transpositionTable, total_depth - 1, max_player);

    int bestEval = max_player ? INT_MIN : INT_MAX;
    BitmaskBoard bestMove;
    for(auto move : moves) {
        int eval;
        BitmaskBoard resultBoard;

        // mimic playing the move
        move.setTurn(max_player ? 0 : 1);
        gameHistory[move.hash()]++;

        if(gameHistory[move.hash()] >= 3)
            eval = 0; // threefold draw
        // Recursively call minimax on each move
        else if (!isEmptyForceList && akel_depth<5)
            eval = search2(depth, !max_player, move, alpha, beta, !max_player, akel_depth+1, true, transpositionTable, bestMove, maxDepth, gameHistory);
        else
        {
            if(!max_player && akel_depth>2)
                // eval = max_player ? INT_MIN : INT_MAX; // throw line
                eval = search2(depth-1, !max_player, move, alpha, beta, !max_player, 100, false, transpositionTable, best_move, maxDepth, gameHistory);
            else
                eval = search2(depth-1, !max_player, move, alpha, beta, !max_player, 0, false, transpositionTable, bestMove, maxDepth, gameHistory);
        }

        if(max_player) {
            if(eval > bestEval) {
                bestEval = eval;
                if((depth == maxDepth) && (akel_depth == 0))
                    bestMove = move;
            }
            alpha = std::max(alpha, bestEval);
        } else {
            if(eval < bestEval) {
                bestEval = eval;
                if((depth == maxDepth) && (akel_depth == 0))
                    bestMove = move;
            }
            beta = std::min(beta, bestEval);
        }

        // remove the move from the game history
        gameHistory[move.hash()]--;

        if(beta <= alpha) {
            break; // Alpha-beta pruning
        }
    }

    // Update transposition table if needed
    if(!transpositionTable.count(board_layout) || transpositionTable[board_layout].depth < total_depth) {
        TTValue value = {total_depth, bestEval};
        transpositionTable[board_layout] = value;
    }

    if((depth == maxDepth) && (akel_depth == 0) && ((AI_IS_WHITE && max_player) || (!AI_IS_WHITE && !max_player)))
        best_move = bestMove;
    return bestEval;
}

BitmaskBoard get_best_move(BitmaskBoard& board, std::unordered_map<BitmaskBoard, TTValue>& transpositionTable, char depth, char turn) {
    auto all_moves = get_all_moves(board, turn).first;

    // Order moves based on transposition table

    BitmaskBoard best_move = all_moves[0];
    for (auto& move : all_moves) {
        move.setTurn(turn == 1 ? 0 : 1);

        auto it = transpositionTable.find(move);
        auto best_eval = transpositionTable[best_move].eval;

        if (turn == 1 && it != transpositionTable.end() && it->second.depth == depth && it->second.eval > best_eval) {
            best_move = move;
            break;
        }

        if (turn == 2 && it != transpositionTable.end() && it->second.depth == depth && it->second.eval < best_eval) {
            best_move = move;
            break;
        }
    }

    return best_move;
}

std::pair<int, BitmaskBoard> iterativeDeepening(BitmaskBoard& initialBoard, char maxDepth, bool isMaxPlayer, std::unordered_map<BitmaskBoard, TTValue>& transpositionTable, int maxTimeSeconds, std::unordered_map<uint64_t, int>& gameHistory) {
    int bestEval = isMaxPlayer ? INT_MIN : INT_MAX;
    BitmaskBoard bestMove = initialBoard;

    // add initial board to game history (white move)

    uint64_t hashKey = initialBoard.hash();
    gameHistory[hashKey]++;

    // if there is only a single move possible play it instantly
    auto [moves, isEmptyForceList] = get_all_moves(initialBoard, isMaxPlayer ? 1 : 2);
    if (moves.size() == 1) {
        // add best move to game history (white move)
        hashKey = moves[0].hash();
        gameHistory[hashKey]++;
        return std::make_pair(0, moves[0]);
    }

    double time_spent = 0.0;
    for (char depth = 2; depth <= maxDepth; ++depth) {
        auto begin = clock();

        // Call minimax for the current depth
        bestMove = initialBoard;
        auto eval = search2(depth, isMaxPlayer, initialBoard, INT_MIN, INT_MAX, isMaxPlayer, 0, false, transpositionTable, bestMove, depth, gameHistory);
        auto end = clock();
        
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        bestEval = eval;

        // check for time constraints here
        if (time_spent > 0.1*maxTimeSeconds) {
            std::cout<<"Depth reached: "<<int(depth)<<"\n\n";
            break;
        }
    }

    // add best move to game history (black move)
    hashKey = bestMove.hash();
    gameHistory[hashKey]++;

    return std::make_pair(bestEval, bestMove);
}

/////////////////////////////////////////////// AI STUFF ////////////////////////////////////////////

int main()
{
    // initializeLookupTable();
    // initializeZobristTable();

    // sombolvsSombol(100, 8);

    double time_spent = 0.0;

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
    std::pair<int, BitmaskBoard> minimaxResult;
    

    int nb_of_pieces = 0;
    BitmaskBoard curr_board;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            
            curr_board.set(i, j, test2[i][j]);
            if(curr_board.get(i, j) != 0)
                nb_of_pieces++;
        }
    }

   //test 
    // create this broad
    // 0 0 0 0 0 0 0 0
    // 1 0 0 1 0 1 1 1
    // 0 1 0 1 1 0 0 1
    // 1 0 1 0 0 1 2 2
    // 1 0 2 2 2 0 0 2
    // 0 2 2 2 0 0 2 0
    // 2 2 0 0 0 0 2 0
    // 0 0 0 0 0 0 0 0

    BitmaskBoard test_board;

    test_board.set(1, 0, 1);
    test_board.set(1, 3, 1);
    test_board.set(1, 5, 1);
    test_board.set(1, 6, 1);
    test_board.set(1, 7, 1);
    test_board.set(2, 1, 1);
    test_board.set(2, 3, 1);
    test_board.set(2, 4, 1);
    test_board.set(2, 7, 1);
    test_board.set(3, 0, 1);
    test_board.set(3, 2, 1);
    test_board.set(3, 5, 1);
    test_board.set(3, 6, 2);
    test_board.set(3, 7, 2);
    test_board.set(4, 0, 1);
    test_board.set(4, 2, 2);
    test_board.set(4, 3, 2);
    test_board.set(4, 4, 2);
    test_board.set(4, 7, 2);
    test_board.set(5, 1, 2);
    test_board.set(5, 2, 2);
    test_board.set(5, 3, 2);
    test_board.set(5, 6, 2);
    test_board.set(6, 0, 2);
    test_board.set(6, 1, 2);
    test_board.set(6, 6, 2);

    // auto pieces = get_all_pieces(test_board, 2);
    // std::vector<char *> pieces;
    // auto force_list = check_for_force(test_board, 2, &pieces);

    // for(auto piece : force_list)
    // {
    //     std::cout<<int(piece[0])<<" "<<int(piece[1])<<"\n";
    // }

    // auto [all_moves, isEmptyForceList] = get_all_moves(test_board, 2);

    // std::vector<std::vector<char*>> parent_list2;
    // char tempPiece[2] = {0, 0};
    // std::vector<char*> tempList;
    // tempList.push_back(tempPiece);
    // parent_list2.push_back(tempList);
    // auto p = eat_max2(0, 0, test_board, parent_list2, 4, 0);

    // for(auto move : all_moves)
    // {
    //     printBoard(move);
    //     std::cout<<"\n";
    // }
    
    // auto p = eat_max2(0, 4, curr_board, parent_list2, 3, 0);

    // // print result
    // for(auto v : p)
    // {
    //     for(auto a : v)
    //         std::cout<<int(a[0])<<" "<<int(a[1])<<"\n";
    //     std::cout<<"\n";
    // }

    // Sleep(10000);

    //fetching TT from  file
    // loadTranspositionTableFromFile(transpositionTable, "transposition_table.txt");


    clock_t begin = clock();
    // for(char i=0; i<1000000; i++)
    //     deepcopy2_char(test2);
    // minimaxResult = minimax_pro2(6, true, test2, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = minimax_pro2_hash(6, true, test2, INT_MIN, INT_MAX, 0, true, false);

    // std::cout<<"nb_of_pieces: "<<nb_of_pieces<<"\n";
    // if(nb_of_pieces < 14)
    //     minimaxResult = minimax_pro2_based(DEPTH + 1, true, curr_board, INT_MIN, INT_MAX, 0, true, false);
    // else
    //     minimaxResult = minimax_pro2_based(DEPTH, true, curr_board, INT_MIN, INT_MAX, 0, true, false);
    // minimaxResult = normal_minimax_based(DEPTH, true, curr_board, INT_MIN, INT_MAX);
    // minimaxResult = normal_minimax(9, true, curr_board, INT_MIN, INT_MAX);

    // save TT to file
    // writeTranspositionTableToFile(transpositionTable, "transposition_table.txt");
    
    // std::vector<std::vector<char*>> parent_list2;
    // char tempPiece[2] = {4, 0};
    // std::vector<char*> tempList;
    // tempList.push_back(tempPiece);
    // parent_list2.push_back(tempList);

    // auto p = eat_max2(4, 0, curr_board, parent_list2, 2, 0);

    // // print result
    // for(auto v : p)
    // {
    //     for(auto a : v)
    //         std::cout<<int(a[0])<<" "<<int(a[1])<<"\n";
    //     std::cout<<"\n";
    // }

    // // auto valid_moves = get_valid_moves(2, 0, 2, 0, minimaxResult.second, 'u').first;

    // auto all_moves = get_all_moves(curr_board, 2).first;
    
    // for(auto move : all_moves)
    // {
    //     printBoard(move);
    //     std::cout<<"\n";
    // }

    // std::cout<<all_moves.size()<<"\n";

    // get game history from file

    std::unordered_map<uint64_t, int> gameHistory;
    readGameHistoryFromFile(gameHistory, "game_history.txt");

    // run iterative deepening
    minimaxResult = iterativeDeepening(curr_board, MAX_DEPTH, AI_IS_WHITE, transpositionTable3, MAX_TIME_SECONDS, gameHistory);


    // save game history to file
    writeGameHistoryToFile(gameHistory, "game_history.txt");

    //start
    clock_t end = clock();
    std::cout<<"Evaluation: "<<minimaxResult.first/100.0<<"\n";
    BitmaskBoard boardResult = minimaxResult.second;
    printBoard(boardResult);
    std::cout<<"Positions Seen: "<<movesSeen<<"\n";

    std::ofstream outfile("minimaxResult.txt");
    if (!outfile.is_open()) {
        std::cout << "Failed to open file" << "\n";
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
        outfile<<"\n";
    }

    // std::vector<char*> pieces;
    // auto force_list = check_for_force(boardResult, 2, &pieces);
    
    // auto all_moves = get_all_moves(minimaxResult.second, 2).first;

    // std::cout<<"\n";
    
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"Positions/sec: "<<movesSeen/time_spent<<"\n";
    std::cout<<"get_all_moves: "<<counter<<"\n";
    std::cout<<"cacheHits: "<<cacheHits<<"\n";
    std::cout<<"Time: "<<time_spent<<"\n"<<"\n";
    //end

    return 0;
}

