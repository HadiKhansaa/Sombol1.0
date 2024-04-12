#include <iostream>
#include <time.h> 
#include <vector>
#include <cstdlib>   // for rand() and srand()
#include <BitmaskBoard.hpp>
#include <moveGeneration.hpp>
#include <util.hpp>
#include <json.hpp>

using namespace std;
using json = nlohmann::json;

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

std::string boardToString(const BitmaskBoard& board) {
    std::string boardStr;  // Use a std::string to accumulate the board representation
    
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            const char piece = board.get(i, j);  // Assume this returns a char (numeric value)
            
            // Correct way to convert numeric value to character ('0', '1', ..., '9')
            boardStr += (piece + '0');  // Convert the piece to its digit character
            
            boardStr += " ";  // Add a space between pieces
        }
        boardStr.pop_back();  // Remove the trailing space at the end of each row
        boardStr += "\n";  // Add a newline character at the end of each row
    }

    // jsonBoard["board"] = boardStr;  // Assign the constructed string to the "board" key in the JSON object
    return boardStr;  // Return the JSON object
}

int main()
{
    // I want to create unit tests for the move generation functions
    // first create a random board
    BitmaskBoard board = generateRandomBoard();

    // test get_all_moves
    bool isEmptyForceList = false;
    std::vector<BitmaskBoard> movesBlack = get_all_moves(board, 1, isEmptyForceList);

    std::vector<BitmaskBoard> movesWhite = get_all_moves(board, 2, isEmptyForceList);

    // Creating a JSON object to store the data
    json testData;

    for(int i = 0; i<10000; i++) {
        BitmaskBoard board = generateRandomBoard();
        std::vector<BitmaskBoard> movesBlack = get_all_moves(board, 1, isEmptyForceList);
        std::vector<BitmaskBoard> movesWhite = get_all_moves(board, 2, isEmptyForceList);

        testData.push_back({
            {"board", boardToString(board)},
            {"movesBlack", {}},
            {"movesWhite", {}}
        });

        for(const auto& move : movesBlack) {
            testData.back()["movesBlack"].push_back(boardToString(move));
        }

        for(const auto& move : movesWhite) {
            testData.back()["movesWhite"].push_back(boardToString(move));
        }

    }

    // Writing to a file
    std::ofstream file("get_all_moves_normal_board_test.json");
    file << testData.dump(4); // 4 spaces as indentation for pretty printing
    file.close();

    return 0;
}