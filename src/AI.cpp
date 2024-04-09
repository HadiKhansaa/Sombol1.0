#include "AI.hpp"
#include "constant.hpp"
#include "moveGeneration.hpp"
#include "checking.hpp"
#include "globals.hpp"
#include <bitset>
#include "robin_hood.h"
#include "util.hpp"
#include <omp.h>


void order_moves2(
    BitmaskBoard& board, 
    std::vector<BitmaskBoard>& moves, 
    robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, 
    char previousDepth, bool isMaxPlayer, int maxDepth) 
{

    board.setTurn(isMaxPlayer ? 0 : 1);
    auto itBoard = transpositionTable.find(board);

    // Sorting lambda function
    auto compareMoves = [&transpositionTable, isMaxPlayer, previousDepth, maxDepth, itBoard, &board](BitmaskBoard& a, BitmaskBoard& b) -> bool {

        a.setTurn(isMaxPlayer ? 1 : 0);
        b.setTurn(isMaxPlayer ? 1 : 0);
        auto itA = transpositionTable.find(a);
        auto itB = transpositionTable.find(b);

        // if move was best move in previous depth consider it first
        if(itBoard != transpositionTable.end() && previousDepth > 1) // early depth best moves are not reliable, and other metrics are better
        {
            // if(a == b)
            //     std::cout<<"akalna khara";
            if(itBoard->second.bestMove == a && !(itBoard->second.bestMove == b))
            {
                // if(previousDepth == 4)
                // {
                //     printBoard(board);
                //     std::cout<<"\n";
                //     printBoard(a);
                //     std::cout<<"------------------\n";
                //     Sleep(500);
                    
                // }
                return true;
            }
            if(itBoard->second.bestMove == b && !(itBoard->second.bestMove == a))
            {
                // std::cout<<"Best move found\n";
                return false;
            }
        }
        // If both moves are found, compare their evaluations
        if (itA != transpositionTable.end() && itB != transpositionTable.end()) {
            // Prioritize higher evaluations for simplicity; adjust based on your game's needs
            // heuristic that prefers the move that was found at a deeper depth
            if(isMaxPlayer)
                return itA->second.eval > itB->second.eval;
            else
                return itA->second.eval < itB->second.eval;
        }
        // // If only one move is found, prioritize it
        else if (itA != transpositionTable.end()) {
            return true;
        }
        else if (itB != transpositionTable.end()) {
            return false;
        }

        // bool boardCapture = board.capture_available();
        // bool aCapture = a.capture_available();
        // bool bCapture = b.capture_available();

        // bool boardHasWhiteKing = board.hasWhiteKing();
        // bool boardHasBlackKing = board.hasBlackKing();

        // char turn = isMaxPlayer ? 1 : 2;

        // // if its a kesh proiritize closing it
        // if(board.capture_available(isMaxPlayer) && !a.capture_available(isMaxPlayer) && b.capture_available(isMaxPlayer))
        //     return true;
        
        // if(board.capture_available(isMaxPlayer) && a.capture_available(isMaxPlayer) && !b.capture_available(isMaxPlayer))
        //     return false;

        // // if move can Capture King prioritize it
        // if(dama_is_captured(board, a, turn) && !dama_is_captured(board, b, turn))
        //     return true;
        
        // if(dama_is_captured(board, b, turn) && !dama_is_captured(board, a, turn))
        //     return false;

        // // prioritize promotion
        // if(!isMaxPlayer && !board.hasWhiteKing() && a.hasWhiteKing() && !b.hasWhiteKing())
        //     return true;
        
        // if(isMaxPlayer && !board.hasBlackKing() && a.hasBlackKing() && !b.hasBlackKing())
        //     return true;

        // if(!isMaxPlayer && !board.hasWhiteKing() && !a.hasWhiteKing() && b.hasWhiteKing())
        //     return false;
        
        // if(isMaxPlayer && !board.hasBlackKing() && !a.hasBlackKing() && b.hasBlackKing())
        //     return false;
        
        // // if its a kol move its usually bad
        // if(!board.capture_available(isMaxPlayer) && a.capture_available(isMaxPlayer) && !b.capture_available(isMaxPlayer))
        //     return false;
        
        // if(!board.capture_available(isMaxPlayer) && !a.capture_available(isMaxPlayer) && b.capture_available(isMaxPlayer))
        //     return true;
        
        // // we reached here so both moves arn't kol, so prioritize advancing
        // if(isMaxPlayer) 
        // {
        //     if(a.calculate_advancment_score() > b.calculate_advancment_score())
        //         return true;
        //     else
        //         return false;
        // }
        // else
        // {
        //     if(a.calculate_advancment_score() < b.calculate_advancment_score())
        //         return true;
        //     else
        //         return false;
        // }

        // // prioritize dama movements
        // if(turn == 1 && boardHasBlackKing && kingMoved(board, a, 1) && !kingMoved(board, b, 1))
        //     return true;
        
        // if(turn == 1 && boardHasBlackKing && !kingMoved(board, a, 1) && kingMoved(board, b, 1))
        //     return false;
        
        // if(turn == 2 && boardHasWhiteKing && kingMoved(board, a, 2) && !kingMoved(board, b, 2))
        //     return true;
        
        // if(turn == 2 && boardHasWhiteKing && !kingMoved(board, a, 2) && kingMoved(board, b, 2))
        //     return false;
        
        return false;
    };

    // Sort moves based on their evaluations stored in the transposition table
    std::sort(moves.begin(), moves.end(), compareMoves);
}

int search2(
    char depth, bool max_player, BitmaskBoard& board_layout,
    int alpha, int beta, char akel_player, char akel_depth, bool akling,
     robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable,
      BitmaskBoard& best_move, char maxDepth, robin_hood::unordered_map<uint64_t, int>& gameHistory)
{  
    int total_depth = depth;

    // Base case remains the same
    board_layout.setTurn(max_player ? 0 : 1);
    if(depth <= 0 || board_layout.player_won()){
        movesSeen++;
        int eval = board_layout.evaluate_board(gameHistory); // Assuming evaluate_board is defined elsewhere
        // TTValue value = {depth, eval};
        // transpositionTable[board_layout] = value;      
        // if((depth == maxDepth) && (akel_depth == 0) && ((AI_IS_WHITE && max_player) || (!AI_IS_WHITE && !max_player)))
        //     best_move = board_layout;
        
        return eval;
    }

    // if draw
    board_layout.setTurn(0); //arbitrarly
    if(board_layout.draw(gameHistory))
    {  
        // save in TT
        // TTValue value = {100, 0, board_layout};
        // transpositionTable[board_layout] = value;

        return 0;
    }

    // First, check if this board state is already in the transposition table
    board_layout.setTurn(max_player ? 0 : 1);
    auto it = transpositionTable.find(board_layout);
    if(!(depth == maxDepth && akel_depth == 0) && it != transpositionTable.end()) {
        TTValue& ttValue = it->second;
        // Ensure that the depth stored in the transposition table is at least as deep as the current search depth
        if(ttValue.depth >= total_depth) {
            // If so, use the stored evaluation and potentially best move
            cacheHits++; // Assuming cacheHits is a metric you're tracking

            // if((depth == maxDepth) && (akel_depth == 0) && ((AI_IS_WHITE && max_player) || (!AI_IS_WHITE && !max_player)))
            //     best_move = ttValue.bestMove;
            
            return ttValue.eval;
        }
    }

    bool isEmptyForceList = false;
    auto moves = get_all_moves(board_layout, max_player ? 1 : 2, isEmptyForceList);

    // Order moves based on transposition table
    order_moves2(board_layout, moves, transpositionTable, total_depth - 1, max_player, maxDepth);

    int bestEval = max_player ? MIN_EVAL : MAX_EVAL;
    BitmaskBoard bestMove;
    for(auto move : moves) {
        // move.setTurn(0);
        int eval;
        // mimic playing the move

        // check instantly if win -> dont search furthur
        // if(board_layout.black_won())
        //     eval = MAX_EVAL;
        // else if(board_layout.white_won())
        //     eval = MIN_EVAL;
        // else if(move.draw(gameHistory))
        //     eval = 0; // draw
        if (!isEmptyForceList && akel_depth < 10) // if akel don't decrease depth
            eval = search2(depth, !max_player, move, alpha, beta, !max_player, akel_depth+1, true, transpositionTable, bestMove, maxDepth, gameHistory);
        else
        {
            // mimic playing the move
            if(isEmptyForceList)
                addMoveToHistory(gameHistory, move, max_player);
            
            // if(move.draw(gameHistory)) // if no more akels and a single piece each
            //     eval = 0;
            if((akel_player == !max_player) && akel_depth>5) // akling path cut
                // eval = max_player ? MAX_EVAL : MIN_EVAL; // throw line
                eval = search2(depth-1, !max_player, move, alpha, beta, !max_player, 100, false, transpositionTable, best_move, maxDepth, gameHistory);
            else
                eval = search2(depth-1, !max_player, move, alpha, beta, !max_player, 0, false, transpositionTable, bestMove, maxDepth, gameHistory);
        }

        // remove the move from the game history
        if(isEmptyForceList)
            removeMoveFromHistory(gameHistory,  move, max_player);

        if(max_player && eval == MAX_EVAL)
        {
            bestMove = move;
            bestEval = eval;
            // save in TT
            TTValue value = {100, eval, bestMove};
            transpositionTable[board_layout] = value;
            break;
        }
        else if(!max_player && eval == MIN_EVAL)
        {
            bestMove = move;
            bestEval = eval;
            // save in TT
            TTValue value = {100, eval, bestMove};
            transpositionTable[board_layout] = value;
            break;
        }
        
        if(max_player) {
            if(eval > bestEval) {
                bestEval = eval;
                // if((depth == maxDepth) && (akel_depth == 0))
                    bestMove = move;
            }
            alpha = std::max(alpha, bestEval);
        } else {
            if(eval < bestEval) {
                bestEval = eval;
                // if((depth == maxDepth) && (akel_depth == 0))
                    bestMove = move;
            }
            beta = std::min(beta, bestEval);
        }
        
        if(akel_depth==0 && beta <= alpha) {
            break; // Alpha-beta pruning
        }
    }

    // Update transposition table if needed
    if(!(bestEval == MAX_EVAL || bestEval == MIN_EVAL) && !transpositionTable.count(board_layout) || transpositionTable[board_layout].depth < total_depth) {
        bestMove.setTurn(max_player ? 1 : 0);
        // if equal depth take the higher akel depth
        TTValue value = {total_depth, bestEval, bestMove};
        transpositionTable[board_layout] = value;
    }

    // store the final best move
    if((depth == maxDepth) && (akel_depth == 0) && !akling && ((AI_IS_WHITE && max_player) || (!AI_IS_WHITE && !max_player)))
    {   
        // std::cout<<int(maxDepth)<<"\n";  
        best_move = bestMove;
    }
    
    return bestEval;
}


std::pair<int, BitmaskBoard> iterativeDeepening(BitmaskBoard& initialBoard, char maxDepth, bool isMaxPlayer, robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, int maxTimeSeconds, robin_hood::unordered_map<uint64_t, int>& gameHistory) {    
    int bestEval = isMaxPlayer ? MIN_EVAL : MAX_EVAL;
    BitmaskBoard bestMove;

    // add initial board to game history (white move)

    initialBoard.setTurn(0); //arbitrarly
    uint64_t hashKey = initialBoard.hash();
    gameHistory[hashKey]++;

    // if there is only a single move possible play it instantly
    bool isEmptyForceList = false;
    auto moves = get_all_moves(initialBoard, isMaxPlayer ? 1 : 2, isEmptyForceList);

    if (moves.size() == 1) {
        // add best move to game history (white move)
        hashKey = moves[0].hash();
        gameHistory[hashKey]++;
        return std::make_pair(0, moves[0]);
    }


    double time_spent = 0.0;
    for (char depth = 2; depth <= maxDepth; ++depth) {
        auto begin = clock();

        // BitmaskBoard bestMove;

        // Call minimax for the current depth
        initialBoard.setTurn(isMaxPlayer ? 0 : 1);
        auto eval = search2(depth, isMaxPlayer, initialBoard, MIN_EVAL, MAX_EVAL, isMaxPlayer, 0, false, transpositionTable, bestMove, depth, gameHistory);
        auto end = clock();

        // printBoard(bestMove);
        // std::cout<<"\nhi";
        
        bestMove.setTurn(isMaxPlayer ? 1 : 0);
        transpositionTable[initialBoard] = {depth, eval, bestMove}; // update depth in TT
        
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

        bestEval = eval;
        
        // check for time constraints here
        if (time_spent > 0.1*maxTimeSeconds) {
            std::cout<<"Depth reached: "<<int(depth)<<"\n\n";
            break;
        }
    }

    // add best move to game history (black move)
    bestMove.setTurn(0); //arbitrarly
    hashKey = bestMove.hash();
    gameHistory[hashKey]++;

    return std::make_pair(bestEval, bestMove);
}


std::pair<int, BitmaskBoard> iterativeDeepening2(BitmaskBoard& initialBoard, char maxDepth, bool isMaxPlayer, robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, int maxTimeSeconds, robin_hood::unordered_map<uint64_t, int>& gameHistory) {
    int windowSize = 50; // Example window size
    int alpha = -10000;
    int beta = 10000;
    int bestEval = isMaxPlayer ? -10000 : 10000;
    BitmaskBoard bestMove = initialBoard;
    bool researchNeeded = false;
    double time_spent = 0.0;

    // Iterate through increasing depths
    for (char depth = 2; depth <= maxDepth; ++depth) {
        // Adjust aspiration window based on previous iteration's score
        if (depth > 2 && !researchNeeded) {
            alpha = bestEval - windowSize;
            beta = bestEval + windowSize;
        }

        // Start the timer
        auto begin = clock();

        // Search at current depth within the aspiration window
        auto eval = search2(depth, isMaxPlayer, initialBoard, alpha, beta, isMaxPlayer, 0, false, transpositionTable, bestMove, depth, gameHistory);

        // Check if the search fell outside the aspiration window
        if (eval <= alpha || eval >= beta) {
            // Widen the window and set the flag to re-search
            alpha = -10000;
            beta = 10000;
            researchNeeded = true;
            depth--; // Repeat this depth with a wider window
            continue;
        }

        // Update best evaluation and move
        bestEval = eval;
        researchNeeded = false;

        // Stop the timer
        auto end = clock();
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        
        // Check if time is up
        if (time_spent > 0.1*maxTimeSeconds) {
            std::cout << "Time's up. Depth reached: " << static_cast<int>(depth) << "\n";
            break;
        }
    }

    // Return the best evaluation and corresponding move
    return std::make_pair(bestEval, bestMove);
}