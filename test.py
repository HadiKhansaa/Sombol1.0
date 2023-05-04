g = 0
import time
from random import randrange


for t in range(100):
    boards = []
    board = [
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 1, 0, 1, 0, 0],
                [1, 1, 0, 1, 1, 1, 1, 0],
                [1, 0, 0, 1, 1, 0, 1, 1],
                [2, 1, 2, 2, 2, 2, 2, 0],
                [2, 0, 0, 2, 2, 2, 2, 2],
                [0, 2, 0, 2, 0, 0, 2, 0],
                [0, 0, 0, 0, 0, 0, 0, 0]
               ]

    for i in range(8):
        for j in range(8):
            board[i][j] = randrange(2)
    
    boards.append(board)


def dummy_fun():
    g=0
    g+=1

start = time.time()

for i in range(100000):
    dummy_fun()

print(g)

end = time.time()

print("time: ", end - start)