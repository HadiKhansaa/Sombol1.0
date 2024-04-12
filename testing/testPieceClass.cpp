#include <cstdint>
#include "move.hpp"
#include "robin_hood.h"
#include "moveGeneration.hpp"
#include "BitmaskBoard.hpp"
#include "checking.hpp"
#include "util.hpp"
#include <iostream>

BitmaskBoard generateRandomBoard()
{
    BitmaskBoard board;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(i > 0 && i < 4) { // rows 1 2 3
                if(rand()%2 == 0) {
                    board.set(i, j, 1); //2 for white
                }
                else {
                    board.set(i, j, 0); //0 for empty
                }
            }
            else if(i > 4 && i < 7) { // rows 5 6
                if(rand()%2 == 0) {
                    board.set(i, j, 2); //2 for white
                }
                else {
                    board.set(i, j, 0); //0 for empty
                }
            }
            else if(i==0) { // black damas
                if(rand()%8 == 0) {
                    board.set(i, j, 3); //3 for black dama small chance
                }
                else {
                    board.set(i, j, 0); //0 for empty
                }
            }
            else if(i==7) { // white damas
                if(rand()%8 == 0) {
                    board.set(i, j, 4); //4 for white dama small chance
                }
                else {
                    board.set(i, j, 0); //0 for empty
                }
            }
            else {
                board.set(i, j, 0); //0 for empty
            
            }
        }
    }
    return board;
}

int main() {
    // test get_all_valid_moves
    BitmaskBoard board_layout = generateRandomBoard();

    // printBoard(board_layout);
    // std::cout << std::endl;


    // bool isEmptyForceList = false;
    // std::vector<std::vector<Piece>> main_parent_list;
    // auto all_valid_moves = get_all_valid_moves(board_layout, 1, isEmptyForceList, main_parent_list);
    // auto moves = get_all_moves3(all_valid_moves, isEmptyForceList, main_parent_list, board_layout, 1);

    // for(auto move : moves) {
    //     printBoard(move);
    //     std::cout << std::endl;
    // }    

    // time 100000 get_all moves vs get_all_moves3
    clock_t begin = clock();
    bool isEmptyForceList = false;
    for(int i = 0; i < 1000000; i++) {
        auto moves = get_all_moves3(board_layout, 1, isEmptyForceList);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time spent for get_all_moves3: " << time_spent << " seconds\n";

    begin = clock();
    isEmptyForceList = false;
    for(int i = 0; i < 1000000; i++) {
        auto moves = get_all_moves(board_layout, 1, isEmptyForceList);
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time spent for get_all_moves: " << time_spent << " seconds\n";

    return 0;
}