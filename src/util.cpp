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
