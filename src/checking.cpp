#include "checking.hpp"
#include "moveGeneration.hpp"
#include "globals.hpp"
#include "robin_hood.h"

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

bool equals(std::vector<char*>& a, std::vector<char*>& b)
{
    for(char i=0; i<(char)a.size(); i++)
        if(a[i][0]!=b[i][0] || a[i][1]!=b[i][1])
            return false;
    return true;
}

bool doesnt_have(std::vector <std::vector<char*>>& a, std::vector<char*>& b) {
    for(auto element:a)
        if(equals(element, b))
            return false;
    return true;
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

std::vector<char*> check_for_force(BitmaskBoard& board_layout, char turn, std::vector<char*> *pieces = { })
{
    auto [pieces2, check] = check_for_possible_capture(board_layout, turn);
    *pieces = pieces2;
    if(check == false)
        return {};
    bool aktar_flag = false;
    std::vector<char*> aktar_list;
    robin_hood::unordered_set<char*> aktar_set;
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

bool dama_is_captured(BitmaskBoard& board_layout, BitmaskBoard& move, char turn) {
    if(turn == 1) {
        if(board_layout.hasWhiteKing() && !move.hasWhiteKing()) {
            return true;
        }
        return false;
    }
    else {
        if(board_layout.hasBlackKing() && !move.hasBlackKing()) {
            return true;
        }
        return false;
    }
}

bool kingMoved(BitmaskBoard& board_layout, BitmaskBoard& move, char turn) {
    if(turn == 1) {
        if(board_layout.getBlackKings() != move.getBlackKings()) {
            return true;
        }
        return false;
    }
    else {
        if(board_layout.getWhiteKings() != move.getWhiteKings()) {
            return true;
        }
        return false;
    }
}







