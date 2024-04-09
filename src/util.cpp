#include "util.hpp"

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

void addMoveToHistory(robin_hood::unordered_map<uint64_t, int>& gameHistory, BitmaskBoard& move, bool isMaxPlayer)
{
    move.setTurn(0); //arbitrarly
    gameHistory[move.hash()]++;
    move.setTurn(isMaxPlayer ? 1 : 0);
}

void removeMoveFromHistory(robin_hood::unordered_map<uint64_t, int>& gameHistory, BitmaskBoard& move, bool isMaxPlayer)
{
    move.setTurn(0); //arbitrarly
    if(gameHistory.count(move.hash()))
        gameHistory[move.hash()]--;
    move.setTurn(isMaxPlayer ? 1 : 0);
}