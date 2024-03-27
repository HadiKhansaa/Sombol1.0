#include "AI.hpp"
#include "constant.hpp"
#include "moveGeneration.hpp"
#include "globals.hpp"

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
    int total_depth = depth;

    // First, check if this board state is already in the transposition table
    board_layout.setTurn(max_player ? 0 : 1);
    auto it = transpositionTable.find(board_layout);
    if(akel_depth==0 && !(depth == maxDepth && akel_depth == 0) && it != transpositionTable.end()) {
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
        // mimic playing the move
        move.setTurn(max_player ? 1 : 0);
        gameHistory[move.hash()]++;

        if(gameHistory[move.hash()] >= 3)
            eval = 0; // threefold draw
        // Recursively call minimax on each move
        else if (!isEmptyForceList && akel_depth<10)
            eval = search2(depth, !max_player, move, alpha, beta, !max_player, akel_depth+1, true, transpositionTable, bestMove, maxDepth, gameHistory);
        else
        {
            if((akel_player == !max_player) && akel_depth>2)
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
    if(akel_depth==0 && !transpositionTable.count(board_layout) || transpositionTable[board_layout].depth < total_depth) {
        TTValue value = {total_depth, bestEval};
        transpositionTable[board_layout] = value;
    }

    if((depth == maxDepth) && (akel_depth == 0) && ((AI_IS_WHITE && max_player) || (!AI_IS_WHITE && !max_player)))
        best_move = bestMove;
    return bestEval;
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
        moves[0].setTurn(isMaxPlayer? 1 : 0);
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
    bestMove.setTurn(isMaxPlayer? 1 : 0);
    hashKey = bestMove.hash();
    gameHistory[hashKey]++;

    return std::make_pair(bestEval, bestMove);
}