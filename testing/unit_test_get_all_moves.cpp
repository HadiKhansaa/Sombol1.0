#include <iostream>
#include <time.h> 
#include <vector>
#include <cstdlib>   // for rand() and srand()
#include "BitmaskBoard.hpp"
#include "moveGeneration.hpp"
#include "util.hpp"
#include <json.hpp>

using namespace std;
using json = nlohmann::json;


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

BitmaskBoard jsonToBoard(const std::string& boardStr) {
    BitmaskBoard board;
    int i = 0;
    int j = 0;
    for (const char& c : boardStr) {
        if (c == '0') {
            board.set(i, j, 0);
        } else if (c == '1') {
            board.set(i, j, 1);
        } else if (c == '2') {
            board.set(i, j, 2);
        } else if (c == '3') {
            board.set(i, j, 3);
        } else if (c == '4') {
            board.set(i, j, 4);
        }
        else
            continue;
        j++;
        if (j == 8) {
            j = 0;
            i++;
        }
    }
    return board;
}

bool compareMovesSets(const std::vector<BitmaskBoard>& generatedMoves, const json& expectedMovesJson) {
    // we don't care here about size, if contents are the same, then it's ok, contvert vector<string> to set<string>
    // Convert all generated moves to their string representations
    std::vector<std::string> generatedMovesStr;
    for (const auto& move : generatedMoves) {
        generatedMovesStr.push_back(boardToString(move));
    }
    
    // Sort both lists to ensure they are in the same order for comparison
    std::sort(generatedMovesStr.begin(), generatedMovesStr.end());
    
    std::vector<std::string> expectedMovesStr;
    for (const auto& move : expectedMovesJson) {
        expectedMovesStr.push_back(move.get<std::string>());
    }
    std::sort(expectedMovesStr.begin(), expectedMovesStr.end());

    std::set<std::string> generatedMovesSet(generatedMovesStr.begin(), generatedMovesStr.end());
    std::set<std::string> expectedMovesSet(expectedMovesStr.begin(), expectedMovesStr.end());

    return generatedMovesSet == expectedMovesSet;
}

bool compareMoves(const std::vector<BitmaskBoard>& generatedMoves, const json& expectedMovesJson) {
    if (generatedMoves.size() != expectedMovesJson.size()) {
        std::cout << "The number of moves doesn't match" << "\n";
        std::cout<<"Generated moves size: "<<generatedMoves.size()<<"\n";
        std::cout<<"Expected moves size: "<<expectedMovesJson.size()<<"\n";
        return false; // The number of moves doesn't match
    }
    
    // Convert all generated moves to their string representations
    std::vector<std::string> generatedMovesStr;
    for (const auto& move : generatedMoves) {
        generatedMovesStr.push_back(boardToString(move));
    }
    
    // Sort both lists to ensure they are in the same order for comparison
    std::sort(generatedMovesStr.begin(), generatedMovesStr.end());
    
    std::vector<std::string> expectedMovesStr;
    for (const auto& move : expectedMovesJson) {
        expectedMovesStr.push_back(move.get<std::string>());
    }
    std::sort(expectedMovesStr.begin(), expectedMovesStr.end());
    
    // Compare the sorted strings
    return generatedMovesStr == expectedMovesStr;
}

// Global variable to keep track of the test number
static int testCount = 0;

void printTestResult(bool passed, const std::string& moveType) {
    testCount++; // Increment the test count for each call

    // ANSI escape codes for colors
    const std::string RED = "\033[31m"; // Red for failed tests
    const std::string GREEN = "\033[32m"; // Green for passed tests
    const std::string RESET = "\033[0m"; // Reset to default terminal color

    // Determine the color based on whether the test passed
    std::string color = passed ? GREEN : RED;
    std::string result = passed ? "passed" : "failed";

    // Output the colored result
    std::cout << "Test " << testCount << " for " << moveType << " moves: " << color << result << RESET << std::endl;
}

void testGetAllMoves(const std::string& jsonFileName, std::vector<BitmaskBoard> (*get_all_moves)(BitmaskBoard&, char, bool&)) {
    std::ifstream file(jsonFileName);
    json j;
    file >> j; // Load the JSON file

    int passedTests = 0, allTests = 0;
    for (const auto& item : j) {
        BitmaskBoard board = jsonToBoard(item["board"].get<std::string>());
        bool isEmptyForceList = false;

        // Test for black
        auto movesBlack = get_all_moves(board, 1, isEmptyForceList);
        // Test for white
        auto movesWhite = get_all_moves(board, 2, isEmptyForceList);

        // Compare the moves with expected moves (you will need to implement comparison logic)
        bool blackMatch = compareMoves(movesBlack, item["movesBlack"]);
        bool whiteMatch = compareMoves(movesWhite, item["movesWhite"]);

        // Output the result
        printTestResult(blackMatch, "Black");
        printTestResult(whiteMatch, "White");

        // count falied tests
        if (blackMatch) {
            passedTests++;
        }
        // else {
        //     std::cout<<"Board:\n";
        //     printBoard(board);

        //     //print generated moves
        //     std::cout << "Generated moves for black:\n";
        //     for (auto& move : movesBlack) {
        //         printBoard(move);
        //         std::cout << "\n";
        //     }
        //     std::cout<<"\n";
        //     break;
        // }
        if (whiteMatch) {
            passedTests++;
        }
        // else {
        //     std::cout<<"Board:\n";
        //     printBoard(board);
        //     std::cout<<"\n";

        //     //print generated moves
        //     std::cout << "Generated moves for white:\n";
        //     for (auto& move : movesWhite) {
        //         printBoard(move);
        //         std::cout << "\n";
        //     }
        //     break;
        // }
        allTests += 2;
    }
    std::cout<<"Passed tests: "<<passedTests<<"/"<<allTests<<std::endl;
}

int main()
{
    testGetAllMoves("C:/Users/USER/Desktop/PROJECTS/dama/testing/get_all_moves_normal_board_test.json", get_all_moves);
    return 0;
}