#include "constant.hpp"
#include "moveGeneration.hpp"
#include "bitmaskBoard.hpp"
#include "hashing.hpp"
#include "util.hpp"
#include "AI.hpp"
#include "globals.hpp"
#include "robin_hood.h"

int main()
{
    //counters
    movesSeen = 0;
    counter = 0;
    cacheHits = 0;

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

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            test2[i][j]-='0';
        

    std::pair<int, BitmaskBoard> minimaxResult;

    BitmaskBoard curr_board;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            curr_board.set(i, j, test2[i][j]);
        }
    }

    clock_t begin = clock();

    // initialize TT
    // std::unordered_map<BitmaskBoard, TTValue> transpositionTable;
    robin_hood::unordered_map<BitmaskBoard, TTValue, BitmaskBoardHash> transpositionTable;


    // read transposition table from file
    // readTranspositionTableFromFile(transpositionTable, "transposition_table.dat");

    // get game history from file
    // std::unordered_map<uint64_t, int> gameHistory;

    robin_hood::unordered_map<uint64_t, int> gameHistory;
    readGameHistoryFromFile(gameHistory, "game_history.txt");

    // run iterative deepening
    minimaxResult = iterativeDeepening(curr_board, MAX_DEPTH, AI_IS_WHITE, transpositionTable, MAX_TIME_SECONDS, gameHistory);


    // test get_all_moves
    // std::vector<BitmaskBoard> moves;
    // bool isEmptyForceList;
    // std::tie(moves, isEmptyForceList) = get_all_moves(minimaxResult.second, 2);

    // for(auto move : moves)
    // {
    //     printBoard(move);
    //     std::cout<<"\n";
    // }

    // save transposition table to file
    // writeTranspositionTableToFile(transpositionTable, "transposition_table.dat");

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

    // BitmaskBoard testBoard;

    // create this board
    // 0 0 0 0 4 0 0 0
    // 1 1 1 1 0 1 1 1 
    // 0 0 0 0 1 0 1 1
    // 1 1 1 0 0 0 0 1
    // 0 0 0 0 0 2 2 2
    // 2 2 2 2 0 0 2 2
    // 2 2 2 2 0 2 0 2
    // 0 0 0 0 0 0 0 0

    // testBoard.set(0, 4, 4);
    // testBoard.set(1, 0, 1);
    // testBoard.set(1, 1, 1);
    // testBoard.set(1, 2, 1);
    // testBoard.set(1, 3, 1);
    // testBoard.set(1, 5, 1);
    // testBoard.set(1, 6, 1);
    // testBoard.set(1, 7, 1);
    // testBoard.set(2, 4, 1);
    // testBoard.set(2, 6, 1);
    // testBoard.set(2, 7, 1);
    // testBoard.set(3, 0, 1);
    // testBoard.set(3, 1, 1);
    // testBoard.set(3, 2, 1);
    // testBoard.set(3, 7, 1);
    // testBoard.set(4, 5, 2);
    // testBoard.set(4, 6, 2);
    // testBoard.set(4, 7, 2);
    // testBoard.set(5, 0, 2);
    // testBoard.set(5, 1, 2);
    // testBoard.set(5, 2, 2);
    // testBoard.set(5, 3, 2);
    // testBoard.set(5, 6, 2);
    // testBoard.set(5, 7, 2);
    // testBoard.set(6, 0, 2);
    // testBoard.set(6, 1, 2);
    // testBoard.set(6, 2, 2);
    // testBoard.set(6, 3, 2);
    // testBoard.set(6, 5, 2);
    // testBoard.set(6, 7, 2);

    // // test get_all_moves
    // std::vector<BitmaskBoard> moves;
    // bool isEmptyForceList;
    // moves = get_all_moves3(testBoard, 2, isEmptyForceList);

    // for(auto move : moves)
    // {
    //     printBoard(move);
    //     std::cout<<"\n";
    // }

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"Positions/sec: "<<movesSeen/time_spent<<"\n";
    std::cout<<"get_all_moves: "<<counter<<"\n";
    std::cout<<"cacheHits: "<<cacheHits<<"\n";
    std::cout<<"Time: "<<time_spent<<"\n"<<"\n";

    return 0;
}