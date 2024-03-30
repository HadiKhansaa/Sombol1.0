#include "../header/constant.hpp"
#include "../header/hashing.hpp"
#include "../header/BitmaskBoard.hpp"

int main () {
    BitmaskBoard board;
    std::unordered_map<BitmaskBoard, TTValue> transpositionTable;

    std::unordered_map<uint64_t, TTValue> transpositionTable2;

    // test speed of inserting into table1 vs table2
    clock_t begin = clock();
    for (int i = 0; i < 1000000; i++) {
        board.set(0, 0, i%4);
        transpositionTable[board] = {i, i};

        transpositionTable.find(board);
    }
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Time spent for unordered_map<BitmaskBoard, TTValue>: " << time_spent << " seconds\n";

    begin = clock();
    for (int i = 0; i < 1000000; i++) {
        transpositionTable2[i] = TTValue();

        transpositionTable2.find(i);
    }
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Time spent for unordered_map<uint64_t, TTValue>: " << time_spent << " seconds\n";



}