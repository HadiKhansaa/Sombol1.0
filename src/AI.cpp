#include "AI.hpp"
#include "constant.hpp"
#include "moveGeneration.hpp"
#include "globals.hpp"
#include "robin_hood.h"
#include "util.hpp"


void order_moves2(
    BitmaskBoard& board, 
    std::vector<BitmaskBoard>& moves, 
    robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, 
    char previousDepth, bool isMaxPlayer) 
{

    // Sorting lambda function
    auto compareMoves = [&transpositionTable, isMaxPlayer](BitmaskBoard& a, BitmaskBoard& b) -> bool {

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
            // b.setTurn(isMaxPlayer ? 0 : 1);
            // auto itB = transpositionTable.find(b);
            // if(itB != transpositionTable.end())
            // {
            //     if(isMaxPlayer)
            //         return itA->second.eval > itB->second.eval;
            //     else
            //         return itA->second.eval < itB->second.eval;
            // }

            // if not found, compare with the evaluation of the board
            int evalB = b.evaluate_board();
            if(isMaxPlayer)
                return itA->second.eval > evalB;
            else
                return itA->second.eval < evalB;
            
            return true;
        }
        else if (itB != transpositionTable.end()) {
            // try to find a in the opposite turn
            // a.setTurn(isMaxPlayer ? 0 : 1);
            // auto itA = transpositionTable.find(a);
            // if(itA != transpositionTable.end())
            // {
            //     if(isMaxPlayer)
            //         return itA->second.eval > itB->second.eval;
            //     else
            //         return itA->second.eval < itB->second.eval;
            // }

            // if not found, compare with the evaluation of the board
            int evalA = a.evaluate_board();
            if(isMaxPlayer)
                return evalA > itB->second.eval;
            else
                return evalA < itB->second.eval;
            return false;
        }
        
        // If neither move is found, prioritize the one with the higher evaluation
        // int evalA = a.evaluate_board();
        // int evalB = b.evaluate_board();

        // // save in TT
        // TTValue value = {0, evalA};
        // transpositionTable[a] = value;

        // value = {0, evalB};
        // transpositionTable[b] = value;


        // if(isMaxPlayer)
        //     return evalA > evalB;
        // else
        //     return evalA < evalB;


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

    // board_layout.setTurn(0); //arbitrarly
    // gameHistory[board_layout.hash()]++;

    int total_depth = depth;
    // if(akel_depth > 2)
    //     std::cout<<"Depth: "<<int(depth)<<" Akel Depth: "<<int(akel_depth)<<"\n";

    // First, check if this board state is already in the transposition table
    board_layout.setTurn(max_player ? 0 : 1);
    auto it = transpositionTable.find(board_layout);
    if(!(depth == maxDepth && akel_depth == 0) && it != transpositionTable.end()) {
        TTValue& ttValue = it->second;
        // Ensure that the depth stored in the transposition table is at least as deep as the current search depth
        if(ttValue.depth >= total_depth) {
            // If so, use the stored evaluation and potentially best move
            cacheHits++; // Assuming cacheHits is a metric you're tracking

            // board_layout.setTurn(0); //arbitrarly
            // gameHistory[board_layout.hash()]--;
            return ttValue.eval;
        }
    }
    // Base case remains the same
    if(depth <= 0 || board_layout.player_won()) {
        movesSeen++;
        int eval = board_layout.evaluate_board(gameHistory); // Assuming evaluate_board is defined elsewhere
        // TTValue value = {depth, eval};
        // transpositionTable[board_layout] = value;

        // board_layout.setTurn(0); //arbitrarly
        // gameHistory[board_layout.hash()]--;
        return eval;
    }

    auto [moves, isEmptyForceList] = get_all_moves(board_layout, max_player ? 1 : 2);

    // Order moves based on transposition table
    order_moves2(board_layout, moves, transpositionTable, total_depth - 1, max_player);

    int bestEval = max_player ? INT_MIN : INT_MAX;
    BitmaskBoard bestMove;

    for(auto move : moves) {
        int eval;
        // mimic playing the move

        // move.setTurn(0); //arbitrarly
        // gameHistory[move.hash()]++;

        // return instantly if win

        if(board_layout.black_won())
            eval = 10000;
        else if(board_layout.white_won())
            eval = -10000;
        else if(gameHistory[move.hash()] >= 3)
            eval = 0; // draw
        else if (!isEmptyForceList && akel_depth < 10) // if akel don't decrease depth
            eval = search2(depth, !max_player, move, alpha, beta, !max_player, akel_depth+1, true, transpositionTable, bestMove, maxDepth, gameHistory);
        else
        {
            // if(board_layout.draw()) // if no more akels and a single piece each
            //     eval = 0;
            if((akel_player == !max_player) && akel_depth>5) // akling path cut
                // eval = max_player ? INT_MAX : INT_MIN; // throw line
                eval = search2(depth-1, !max_player, move, alpha, beta, !max_player, 100, false, transpositionTable, best_move, maxDepth, gameHistory);
            else
                eval = search2(depth-1, !max_player, move, alpha, beta, !max_player, 0, false, transpositionTable, bestMove, maxDepth, gameHistory);
        }

        if(max_player) {
            if(eval > bestEval) {
                bestEval = eval;
                if((depth == maxDepth) && (akel_depth == 0))
                    bestMove = move;
                if(bestEval == 10000) // if win return instantly
                    break;
            }
            alpha = std::max(alpha, bestEval);
        } else {
            if(eval < bestEval) {
                bestEval = eval;
                if((depth == maxDepth) && (akel_depth == 0))
                    bestMove = move;
                if(bestEval == -10000) // if win return instantly
                    break;
            }
            beta = std::min(beta, bestEval);
        }

        // remove the move from the game history
        // move.setTurn(0); //arbitrarly
        // gameHistory[move.hash()]--;

        if(akel_depth == 0 && beta <= alpha) {
            break; // Alpha-beta pruning
        }
    }

    // Update transposition table if needed
    if(!transpositionTable.count(board_layout) || transpositionTable[board_layout].depth < total_depth) {

        // if equal depth take the higher akel depth
        TTValue value = {total_depth, bestEval};
        transpositionTable[board_layout] = value;
    }

    if((depth == maxDepth) && (akel_depth == 0) && ((AI_IS_WHITE && max_player) || (!AI_IS_WHITE && !max_player)))
        best_move = bestMove;
    
    // board_layout.setTurn(0); //arbitrarly
    // gameHistory[board_layout.hash()]--;

    return bestEval;
}

std::pair<int, BitmaskBoard> iterativeDeepening(BitmaskBoard& initialBoard, char maxDepth, bool isMaxPlayer, robin_hood::unordered_map<BitmaskBoard, TTValue>& transpositionTable, int maxTimeSeconds, robin_hood::unordered_map<uint64_t, int>& gameHistory) {    
    int bestEval = isMaxPlayer ? INT_MIN : INT_MAX;
    BitmaskBoard bestMove = initialBoard;

    // add initial board to game history (white move)

    initialBoard.setTurn(0); //arbitrarly
    uint64_t hashKey = initialBoard.hash();
    gameHistory[hashKey]++;

    // if there is only a single move possible play it instantly
    auto [moves, isEmptyForceList] = get_all_moves(initialBoard, isMaxPlayer ? 1 : 2);

    // std::cout<<"Number of moves: "<<moves.size()<<"\n";
    // for(auto move : moves)
    // {
    //     printBoard(move);
    //     std::cout<<"\n";
    // }

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

        // std::cout<<transpositionTable.size()<<"\n";
        
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