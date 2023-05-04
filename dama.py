import copy
from copy import deepcopy
import time
from turtle import position
import pygame
from numba import njit, jit
import multiprocessing as mp
from multiprocessing import Pool
from itertools import product
import numpy as np

from pygame.locals import (
    RLEACCEL,
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    KEYDOWN,
    QUIT,
)

force_list =  []
aux_list = []
game_history = {}
pygame.init()

screen = pygame.display.set_mode((800, 800))
clock = pygame.time.Clock()
fff = False
has_eaten = False
eatable=False
dama_nom = False
turn = 2

ate_up = False
ate_down = False
ate_left = False
ate_right = False

piece_to_eat =(-1, -1)

def no_more_moves_black(b):
    for i in b.layout:
        for j in i:
            if j==1 or j==3:
                return False

    return True

def no_more_moves_white(b):
    for i in b.layout:
        for j in i:
            if j==2 or j==4:
                return False

    return True

def empty_square(row, col):
    pass

def check_for_nom(row, col, color, streak, valid_moves, board_layout):
    global force_list
    global fff
    global dama_nom
    fff = False
    global ate_up
    global ate_down
    global ate_left
    global ate_right
    global piece_to_eat

    old_up = ate_up
    old_down = ate_down
    old_left = ate_left
    old_right = ate_right

    get_valid_moves(row, col, color, streak, valid_moves, board_layout)

    if fff or dama_nom:
        ate_up = old_up
        ate_down = old_down
        ate_left = old_left
        ate_right = old_right
        return 1
    else:
        ate_up = old_up
        ate_down = old_down
        ate_left = old_left
        ate_right = old_right
        return 0
    

def get_valid_moves(row, col, color, streak, valid_moves, board_layout):
    global force_list
    global fff
    global dama_nom
    fff = False

    global ate_up
    global ate_down
    global ate_left
    global ate_right

    global piece_to_eat
    piece_to_eat = (-1, -1)

    dama_nom = False

    
    #black dama
    if color == 3:
        #right
        i=1
        while True and not ate_left:
            if col+i >= 8:
                break
            if col+i<8 and board_layout[row][col+i] == 3 or board_layout[row][col+i] == 1:
                break
            if col+i<8 and board_layout[row][col+i] == 4 or board_layout[row][col+i] == 2:
                j=1
                while True:
                    if col+i+j>=8:
                        i=10
                        break
                    if col+i+j<8 and board_layout[row][col+i+j] == 4 or board_layout[row][col+i+j] == 2 or board_layout[row][col+i+j] == 3 or board_layout[row][col+i+j] == 1:
                        i=10
                        break
                    if col+i+j<8 and board_layout[row][col+i+j] == 0:
                        piece_to_eat = (row, col+i)
                        valid_moves.append((row, col+i+j))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1
            
        #left
        i=1
        while True and not ate_right:
            if col-i < 0:
                break
            if board_layout[row][col-i] == 3 or board_layout[row][col-i] == 1:
                break
            if board_layout[row][col-i] == 4 or board_layout[row][col-i] == 2:
                j=1
                while True:
                    if col-i-j<0:
                        i=10
                        break
                    if board_layout[row][col-i-j] == 4 or board_layout[row][col-i-j] == 2 or board_layout[row][col-i-j] == 3 or board_layout[row][col-i-j] == 1:
                        i=10
                        break
                    if  board_layout[row][col-i-j] == 0:
                        piece_to_eat = (row, col-i)
                        valid_moves.append((row, col-i-j))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1


        #down
        i=1
        while True and not ate_up:
            if row+i >= 8:
                break
            if board_layout[row+i][col] == 3 or board_layout[row+i][col] == 1:
                break
            if board_layout[row+i][col] == 4 or board_layout[row+i][col] == 2:
                j=1
                while True:
                    if row+i+j>=8:
                        i=10
                        break
                    if board_layout[row+i+j][col] == 4 or board_layout[row+i+j][col] == 2 or board_layout[row+i+j][col] == 3 or board_layout[row+i+j][col] == 1:
                        i=10
                        break
                    if board_layout[row+i+j][col] == 0:
                        piece_to_eat = (row+i, col)
                        valid_moves.append((row+i+j, col))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1
            
        #up
        i=1
        while True and not ate_down:
            if row-i < 0:
                break
            if board_layout[row-i][col] == 3 or board_layout[row-i][col] == 1:
                break
            if board_layout[row-i][col] == 4 or board_layout[row-i][col] == 2:
                j=1
                while True:
                    if row-i-j<0:
                        i=10
                        break
                    if board_layout[row-i-j][col] == 4 or board_layout[row-i-j][col] == 2 or board_layout[row-i-j][col] == 3 or board_layout[row-i-j][col] == 1:
                        i=10
                        break
                    if board_layout[row-i-j][col] == 0:
                        piece_to_eat = (row-i, col)
                        valid_moves.append((row-i-j, col))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1
        
        if dama_nom==False:
            i=1
            while col+i<8 and board_layout[row][col+i] == 0:
                valid_moves.append((row, col+i))
                i+=1
            
            i=1
            while col-i>=0 and board_layout[row][col-i] == 0:
                valid_moves.append((row, col-i))
                i+=1

            i=1
            while row+i<8 and board_layout[row+i][col] == 0:
                valid_moves.append((row+i, col))
                i+=1
            
            i=1
            while row-i>=0 and board_layout[row-i][col] == 0:
                valid_moves.append((row-i, col))
                i+=1

    #red dama
      
    if color == 4:
        dama_nom = False
        #right
        i=1
        while True and not ate_left:
            if col+i >= 8:
                break
            if col+i<8 and board_layout[row][col+i] == 4 or board_layout[row][col+i] == 2:
                break
            if col+i<8 and board_layout[row][col+i] == 3 or board_layout[row][col+i] == 1:
                j=1
                while True:
                    if col+i+j>=8:
                        i=10
                        break
                    if col+i+j<8 and board_layout[row][col+i+j] == 4 or board_layout[row][col+i+j] == 2 or board_layout[row][col+i+j] == 3 or board_layout[row][col+i+j] == 1:
                        i=10
                        break
                    if col+i+j<8 and board_layout[row][col+i+j] == 0:
                        piece_to_eat = (row, col+i)
                        valid_moves.append((row, col+i+j))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1
            
        #left
        i=1
        while True and not ate_right:
            if col-i < 0:
                break
            if board_layout[row][col-i] == 4 or board_layout[row][col-i] == 2:
                break
            if board_layout[row][col-i] == 3 or board_layout[row][col-i] == 1:
                j=1
                while True:
                    if col-i-j<0:
                        i=10
                        break
                    if board_layout[row][col-i-j] == 4 or board_layout[row][col-i-j] == 2 or board_layout[row][col-i-j] == 3 or board_layout[row][col-i-j] == 1:
                        i=10
                        break
                    if board_layout[row][col-i-j] == 0:
                        piece_to_eat = (row, col-i)
                        valid_moves.append((row, col-i-j))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1


        #down
        i=1
        while True and not ate_up:
            if row+i >= 8:
                break
            if board_layout[row+i][col] == 4 or board_layout[row+i][col] == 2:
                break
            if board_layout[row+i][col] == 3 or board_layout[row+i][col] == 1:
                j=1
                while True:
                    if row+i+j>=8:
                        i=10
                        break
                    if board_layout[row+i+j][col] == 4 or board_layout[row+i+j][col] == 2 or board_layout[row+i+j][col] == 3 or board_layout[row+i+j][col] == 1:
                        i=10
                        break
                    if  board_layout[row+i+j][col] == 0:
                        piece_to_eat = (row+i, col)
                        valid_moves.append((row+i+j, col))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1
            
        #up
        i=1
        while True and not ate_down:
            if row-i < 0:
                break
            if board_layout[row-i][col] == 4 or board_layout[row-i][col] == 2:
                break
            if board_layout[row-i][col] == 3 or board_layout[row-i][col] == 1:
                j=1
                while True:
                    if row-i-j<0:
                        i=10
                        break
                    if board_layout[row-i-j][col] == 4 or board_layout[row-i-j][col] == 2 or board_layout[row-i-j][col] == 3 or board_layout[row-i-j][col] == 1:
                        i=10
                        break
                    if  board_layout[row-i-j][col] == 0:
                        piece_to_eat = (row-i, col)
                        valid_moves.append((row-i-j, col))
                        dama_nom= True
                        fff=True
                    j+=1
            i+=1
        
        if dama_nom==False:
            i=1
            while col+i<8 and board_layout[row][col+i] == 0:
                valid_moves.append((row, col+i))
                i+=1
            
            i=1
            while col-i>=0 and board_layout[row][col-i] == 0:
                valid_moves.append((row, col-i))
                i+=1

            i=1
            while row+i<8 and board_layout[row+i][col] == 0:
                valid_moves.append((row+i, col))
                i+=1
            
            i=1
            while row-i>=0 and board_layout[row-i][col] == 0:
                valid_moves.append((row-i, col))
                i+=1
    
    if color == 1:
        #streak
        if row+2<8 and (board_layout[row+1][col]==2 or board_layout[row+1][col]==4) and board_layout[row+2][col]==0 and streak>=0:
            streak+=1
            valid_moves.append((row+2, col))
            fff=True
            

        if col+2<8 and (board_layout[row][col+1]==2 or board_layout[row][col+1]==4) and board_layout[row][col+2]==0 and streak>=0 and not ate_left:
            streak+=1
            valid_moves.append((row, col+2))
            fff=True
            

        if col-2>=0 and (board_layout[row][col-1]==2 or board_layout[row][col-1]==4) and board_layout[row][col-2]==0 and streak>=0 and not ate_right:
            streak+=1
            valid_moves.append((row, col-2))
            fff=True
            
        

        if row+1<8 and board_layout[row+1][col]==0 and streak==0:
            valid_moves.append((row+1, col))
        if col+1<8 and board_layout[row][col+1]==0 and streak==0:
            valid_moves.append((row, col+1))
        if col-1>=0 and board_layout[row][col-1]==0 and streak==0:
            valid_moves.append((row, col-1))
    elif color == 2:
        #streak
        
        if row-2>=0 and (board_layout[row-1][col]==1 or board_layout[row-1][col]==3) and board_layout[row-2][col]==0 and streak>=0:
            streak+=1
            valid_moves.append((row-2, col))
            fff=True

        if col+2<8 and (board_layout[row][col+1]==1 or board_layout[row][col+1]==3) and board_layout[row][col+2]==0 and streak>=0 and not ate_left:
            streak+=1
            valid_moves.append((row, col+2))
            fff=True

        if col-2>=0 and (board_layout[row][col-1]==1 or board_layout[row][col-1]==3) and board_layout[row][col-2]==0 and streak>=0 and not ate_right:
            streak+=1
            valid_moves.append((row, col-2))
            fff=True

        if row-1>=0 and board_layout[row-1][col]==0 and streak==0:
            valid_moves.append((row-1, col))
        if col+1<8 and board_layout[row][col+1]==0 and streak ==0:
            valid_moves.append((row, col+1))
        if col-1>=0 and board_layout[row][col-1]==0 and streak ==0:
            valid_moves.append((row, col-1))
    

    return valid_moves

def eat_piece():
    pass

def check_for_force(board_layout, turn):
    global fff
    global ate_down
    global ate_up
    global ate_right
    global ate_left

    old_up = ate_up
    old_down = ate_down
    old_right = ate_right
    old_left = ate_left

    ate_up = False
    ate_down = False
    ate_right = False
    ate_left = False
    aktar_list = []
    eaten_pieces = {}
    aktar_flag = False
    force_list = []
    maxLength = 3
    for row2 in range(8):
        for col2 in range(8):
            color = board_layout[row2][col2]
            if ((board_layout[row2][col2]==2 or board_layout[row2][col2]==4) and turn==2) or ((board_layout[row2][col2]==1 or board_layout[row2][col2]==3) and turn==1):
                fff = False
                parent_list2 = []

                ate_up = False
                ate_down = False
                ate_right = False
                ate_left = False

                valid_moves = get_valid_moves(row2, col2, board_layout[row2][col2], 0, [], board_layout)
                if fff==True:
                    parent_list2 = []
                    for move in valid_moves:
                        parent_list2.append ( [(row2, col2), move])

                    if color == 1 or color == 2:
                        t_board_layout = deepcopy(board_layout)
                        parent_list = eat_max2_not_dama(row2, col2, t_board_layout, parent_list2, board_layout[row2][col2])
                        if parent_list != []:
                            listSize = len(parent_list[0])
                        if parent_list != None and parent_list != [] and listSize>2 and listSize>=maxLength:
                            if listSize>maxLength:
                                aktar_list.clear()
                            maxLength = listSize
                            aktar_flag = True
                            for value in parent_list:
                                aktar_list.append(value[0])

                    elif color == 3 or color == 4:
                        t_board_layout = deepcopy(board_layout)
                        parent_list = eat_max2(row2, col2, t_board_layout, parent_list2, board_layout[row2][col2], eaten_pieces)
                        if parent_list != []:
                            listSize = len(parent_list[0])
                        if parent_list != None and parent_list != [] and listSize>2 and listSize>=maxLength:
                            if listSize>maxLength:
                                aktar_list.clear()
                            maxLength = listSize
                            for value in parent_list:
                                aktar_list.append(value[0])
                            aktar_flag = True
                    force_list.append((row2, col2))
    if aktar_flag:
        force_list = aktar_list
        #for move in force_list:
            #if move not in aktar_list:
                #force_list.remove(move)

    ate_up = old_up
    ate_down = old_down
    ate_right = old_right
    ate_left = old_left
    fff = False
    return force_list

                        
running = True
def main_menu():
    global running

    run = True
    while run:
        clock.tick(30)
        screen2 = pygame.display.set_mode((800, 800))
        screen.fill((255, 174, 201))

        startSound = pygame.mixer.Sound('sound/start.wav')
        leaveSound = pygame.mixer.Sound('sound/discord-leave.mp3')

        newGameBottonSurf = pygame.image.load("images/newGameBotton.png")
        skinBottonSurf = pygame.image.load("images/skinBotton.png")
        timerBottonSurf = pygame.image.load("images/timerBotton.png")
        quitBottonSurf = pygame.image.load("images/quitBotton.png")
        piecesBgImageSurf = pygame.image.load("images/piecesBgImage.png")
        titleSurf = pygame.image.load("images/title.png")
        	
        newGameRect = pygame.Rect((550, 200), (164, 72))
        skinRect = pygame.Rect((550, 350), (164, 72))
        timerRect = pygame.Rect((550, 500), (164, 72))
        quitRect = pygame.Rect((550, 650), (164, 72))

        p2 = pygame.mouse.get_pos()
        if newGameRect.collidepoint(p2):
            newGameBottonSurf = pygame.image.load("images/newGameHover.png")
        elif skinRect.collidepoint(p2):
            skinBottonSurf = pygame.image.load("images/skinHover.png")
        elif timerRect.collidepoint(p2):
            timerBottonSurf = pygame.image.load("images/timerHover.png")
        elif quitRect.collidepoint(p2):
            quitBottonSurf = pygame.image.load("images/quitHover.png")

        screen2.blit(newGameBottonSurf, (550, 200))
        screen2.blit(skinBottonSurf, (550, 350))
        screen2.blit(timerBottonSurf, (550, 500))
        screen2.blit(quitBottonSurf, (550, 650))

        screen2.blit(titleSurf, (100, 150))
        screen2.blit(piecesBgImageSurf, (0, 300))
        	
        for  event in pygame.event.get():
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    run = False
                    running = False
                    return ""
            elif event.type == pygame.QUIT:
                run = False
                running = False
                return ""
            if event.type == pygame.MOUSEBUTTONDOWN:
                p = pygame.mouse.get_pos()
                if newGameRect.collidepoint(p):
                    startSound.play()
                    run = False
                    return ""
                elif skinRect.collidepoint(p):
                    startSound.play()
                    run = False
                    jpg = select_skin(screen2)
                    return jpg
                elif timerRect.collidepoint(p):
                    startSound.play()
                    run = False
                    return ""
                elif quitRect.collidepoint(p):
                    leaveSound.play()
                    running = False
                    run = False
                    return ""
        pygame.display.flip()

def select_skin(screen2):
    global running
    surf_select_skin = pygame.image.load("images/select_board.png").convert()
    startSound = pygame.mixer.Sound('sound/start.wav')
    run = True
    while run:
        clock.tick(30)
        surf_select_skin = pygame.transform.scale(surf_select_skin, (800, 800))

        screen2 = pygame.display.set_mode((800, 800))
        screen.fill((255, 174, 201))
        screen2.blit(surf_select_skin, (30, 30))
        for  event in pygame.event.get():
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    run = False
                    running = False
                    return ""
            elif event.type == pygame.QUIT:
                run = False
                running = False
                return ""
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_1:
                    startSound.play()
                    running = True
                    return "images/board.jpg"
                elif event.key == pygame.K_2:
                    running = True
                    startSound.play()
                    return "images/board2.jpg"
                elif event.key == pygame.K_3:
                    running = True
                    startSound.play()
                    return "images/board3.jpg"
                elif event.key == pygame.K_4:
                    running = True
                    startSound.play()
                    return "images/board4.jpg"
                
            if event.type == pygame.MOUSEBUTTONDOWN:
                p = pygame.mouse.get_pos()
                if (p[0]>90 and p[0]<311) and (p[1]>150 and p[1]<376):
                    running = True
                    startSound.play()
                    return "images/board.jpg"
                elif (p[0]>485 and p[0]<710) and (p[1]>150 and p[1]<376):
                    running = True
                    startSound.play()
                    return "images/board2.jpg"
                elif (p[0]>80 and p[0]<305) and (p[1]>450 and p[1]<675):
                    running = True
                    startSound.play()
                    return "images/board3.jpg"
                elif (p[0]>485 and p[0]<710) and (p[1]>450 and p[1]<675):
                    running = True
                    startSound.play()
                    return "images/board4.jpg"
                
        pygame.display.flip()


def get_layout(board_surf):
    layout = [
             [0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
         ]
    for i in range(8):
        for j in range(8):
            color = board_surf.get_at((i*100 + 50, j*100 + 50))[:3]
            x = color[0]
            y = color[1]
            z = color[2]

            if 240<x<257 and 240<y<257 and 240<z<257:
                layout[j][i] = 2
            elif -1<x<20 and -1<y<20 and -1<z<20:
                layout[j][i] = 1
            elif -1<x<2 and -1<y<2 and 240<z<257:
                layout[j][i] = 3
            elif 240<x<257 and -1<y<2 and -1<z<2:
                layout[j][i] = 4
            else:
                layout[j][i] = 0
    return layout


class blackPiece:
    surf = pygame.image.load("images/black.png")
    x = 0
    y = 0
    isDama = False
    moves = []
    Alive = True

class whitePiece:
    surf = pygame.image.load("images/red.png")
    x = 0
    y = 0
    isDama = False
    moves = []
    Alive = True

class Board:
    surf = pygame.image.load("images/board2.jpg").convert()
    surf = pygame.transform.scale(surf, (800, 800))

    testSurf = pygame.image.load("positionBoard.jpeg").convert()
    testSurf = pygame.transform.scale(testSurf, (800, 800)) 

    layout2 = np.array(
                        [
                        [0, 0, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 1, 1, 1, 1],
                        [1, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0],
                        [2, 2, 2, 2, 2, 2, 2, 2],
                        [2, 2, 2, 2, 2, 2, 2, 2],
                        [0, 0, 0, 0, 0, 0, 0, 0]
                        
                        ]
                        
                        )
              
    layout = [
                [3, 0, 0, 0, 0, 0, 0, 0],
                [1, 0, 1, 0, 0, 2, 4, 0],
                [0, 0, 1, 0, 0, 0, 0, 1],
                [1, 0, 0, 0, 0, 1, 0, 0],
                [0, 2, 0, 0, 0, 0, 0, 0],
                [2, 2, 2, 0, 0, 0, 0, 0],
                [2, 0, 0, 2, 2, 0, 0, 2],
                [0, 0, 0, 0, 0, 0, 0, 0]
               ]

    layout3 = get_layout(testSurf)

    layout4 = [
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [1, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 1, 0, 0, 0, 0],
                [0, 0, 0, 0, 4, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0]

                ]



###################################################### ai stuff ##########################################################

def get_all_pieces(board_layout, color):
    pieces = []
    for i in range(0, 8):
        for j in range(0, 8):
            if (color == 1 and (board_layout[i][j] == 1 or board_layout[i][j] == 3)) or (color ==2 and (board_layout[i][j] == 2 or board_layout[i][j] == 4)):
                pieces.append((i, j))
    return pieces

def passage_is_clear(board_layout, row, col, turn):
    if turn == 1:
        if col == 7:
            if board_layout[6][7] != 2 and board_layout[6][6] != 2  and board_layout[6][7] != 4 and board_layout[6][6] != 4:
                return True
            return False

        if col == 0:
            if board_layout[6][0] != 2 and board_layout[6][1] != 2 and board_layout[6][0] != 4 and board_layout[6][1] != 4:
                return True
            return False
        
        if (board_layout[row+1][col+1] == 2 or board_layout[row+1][col+1] == 4) ^ (board_layout[row+1][col-1] == 2 or board_layout[row+1][col-1] == 4):
            return False

        for j in range(-2, 3):
            if col+j < 0:
                continue
            if col+j > 7:
                break
            if board_layout[row+2][col+j] == 2 or board_layout[row+2][col+j] == 4:
                return False
        return True

    if turn == 2:
        if col == 7:
            if board_layout[1][7] != 1 and board_layout[1][6] != 1 and board_layout[1][7] != 3 and board_layout[1][6] != 3:
                return True
            return False

        if col == 0:
            if board_layout[0][1] != 1 and board_layout[1][1] != 1 and board_layout[0][1] != 3 and board_layout[1][1] != 3:
                return True
            return False
        
        if (board_layout[row-1][col+1] == 1 or board_layout[row-1][col+1] == 3) ^ (board_layout[row-1][col-1] == 1 or board_layout[row-1][col-1] == 3):
            return False

        for j in range(-2, 3):
            if col+j < 0:
                continue
            if col+j > 7:
                break
            if board_layout[row-2][col+j] == 1 or board_layout[row-2][col+j]==3:
                return False
        return True

def get_valid_moves2(board_layout, piece, color2):
    global ate_down
    global ate_up
    global ate_left
    global ate_right

    old_up = ate_up
    old_down = ate_down
    old_left = ate_left
    old_right = ate_right

    parent_list2 = []
    parent_list = []
    valid_moves = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout)
    if fff:
        for move in valid_moves:
            parent_list2.append ( [(piece[0], piece[1]), move])
        
        if color2 == 1 or color2 == 2:
            temporary_board_layout = deepcopy(board_layout)
            parent_list = eat_max2_not_dama(piece[0], piece[1], temporary_board_layout, parent_list2, color2)
            aux_list = []
            if parent_list != [] and parent_list!=None:
                for value in parent_list:
                    aux_list.append(value[-1])
                valid_moves = aux_list

        elif color2==3 or color2 == 4:
            temporary_board_layout = deepcopy(board_layout)
            parent_list = eat_max2(piece[0], piece[1], temporary_board_layout, parent_list2, color2, {})
            aux_list = []
            if parent_list != [] and parent_list!=None:
                for value in parent_list:
                    aux_list.append(value[-1])
                valid_moves = aux_list
    
    ate_up = old_up
    ate_down = old_down
    ate_left = old_left
    ate_right = old_right

    return valid_moves, parent_list

def double_kesh(board_layout, row, col, turn):
    if turn == 1:
        if row+2 > 7 or col+1 > 7 or col-1 < 0:
            return False
        if (board_layout[row+1][col+1] == 2 or board_layout[row+1][col+1] == 4) and (board_layout[row+1][col-1] == 2 or board_layout[row+1][col-1] == 4) and board_layout[row+2][col]==0: 
            return True
    if turn == 2:
        if row-2 <0 or col+1 > 7 or col-1 < 0:
            return False
        if (board_layout[row-1][col+1] == 1 or board_layout[row-1][col+1] == 3) ^ (board_layout[row-1][col-1] == 1 or board_layout[row-1][col-1] == 3) and board_layout[row-2][col]==0:
            return True
    return False

positions_seen = 0
def evaluate(board_layout, turn):
    global positions_seen
    positions_seen+=1
    sum = 0
    balance_black = 0
    balance_red = 0
    all_pieces = 0
    red_pieces = 0
    black_pieces = 0

    #if there's 1 forced move evalute after it being played
    #black
    if turn == 1:
        force_list1 = check_for_force(board_layout, 1)

        if len(force_list1) == 1:
            forced_piece = force_list1[0]
            valid_moves, parent_list = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]])

            if len(valid_moves) == 1:
                forced_move = valid_moves[0]
                temp_board_layout = board_layout.copy()
                move_piece(forced_piece, forced_move, temp_board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]])
                board_layout = temp_board_layout
                turn = 2
        
    #red
    if turn == 2:
        force_list1 = check_for_force(board_layout, 2)
        
        if len(force_list1) == 1:
            forced_piece = force_list1[0]
            valid_moves, parent_list = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]])

            if len(valid_moves) == 1:
                forced_move = valid_moves[0]    
                temp_board_layout = board_layout.copy()
                move_piece(forced_piece, forced_move, temp_board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]])
                board_layout = temp_board_layout
                turn = 1
    


    for i in range(8):
        for j in range(8):

            piece = board_layout[i][j]
            if piece == 1:
                #sides are good
                if j==0 or j==7:
                    sum+=0.07

                black_pieces+=1
                #spreading the pieces
                if j<=3:
                    balance_black+=1
                else:
                    balance_black-=1

                sum+=1 #piece = 1 pnt

                sum += i*0.01 # the higher the better

                if i == 6: #pre-dama
                    sum+=1.5
                #important rows
                if i==5: 
                    sum+=0.4
                if i==4 and turn == 1:
                    sum+=0.3
                
                # 4 important squares
                if j==0 or j==7:
                    if i == 5 and turn == 1:
                        sum+=0.8
                    if i == 5 and turn == 2:
                        sum+=0.2
                    if i == 6 and turn == 1:
                        sum+=0.5
                
                #if on 5th row and passage is clear the piece is dangerous
                if i==4 and turn==1 and passage_is_clear(board_layout, i, j, turn):
                    sum+=0.8

                #double kesh
                if turn==1 and double_kesh(board_layout, i, j, turn):
                    sum+=0.3
                    
            elif piece == 2:
                #sides are good
                if j==0 or j==7:
                    sum-=0.07
                red_pieces+=1
                #spreading the pieces
                if j<=3:
                    balance_red+=1
                else:
                    balance_red-=1

                sum-=1
                sum -= (7-i)*0.01
                if i == 1:
                    sum-=1.5
                if i == 2:
                    sum-=0.4
                if i == 3 and turn == 2:
                    sum-=0.3

                # 4 important squares
                if j==0 or j==7:
                    if i == 2 and turn == 2:
                        sum-=0.8
                    if i == 2 and turn == 1:
                        sum-=0.2
                    if i == 1 and turn == 2:
                        sum-=0.5
                
                #if on 5th row and passage is clear the piece is dangerous
                if i==3 and turn==2 and passage_is_clear(board_layout, i, j, turn):
                    sum-=0.8
                
                #double kesh
                if turn==2 and double_kesh(board_layout, i, j, turn):
                    sum-=0.3


            elif piece == 3:
                black_pieces+=1
                sum+=3.5
            elif piece == 4:
                red_pieces+=1
                sum-=3.5

    sum = sum - abs(balance_black)/10 + abs(balance_red)/10

    all_pieces = black_pieces + red_pieces

    if red_pieces>black_pieces:
        sum += all_pieces*0.005
    elif black_pieces>red_pieces:
        sum -= all_pieces*0.005

    #draw
    if str(board_layout) in game_history.keys():
        if game_history[str(board_layout)]>=2:
            sum = 0
    if black_pieces==1 and red_pieces==1:
        sum = 0
               
    return sum


def win(board_layout):
    for i in board_layout:
        for piece in i:
            if piece!=0:
                return False
    return True

def draw_move(screen, move):

    screen.fill((255, 255, 255))
    screen.blit(board.surf,(0, 0))

    for i in range(0, 8):
        for j in range(0, 8):
            if move[i][j]==1:
                screen.blit(blackpiece.surf, (10+j*100, 10 + i*100))
            elif move[i][j]==2:
                screen.blit(whitepiece.surf, (10+j*100, 10 + i*100))
            elif move[i][j]==3:
                screen.blit(blackDama, (10+j*100, 10 + i*100))
            elif move[i][j]==4:
                screen.blit(redDama, (10+j*100, 10 + i*100))
    
    pygame.display.update()
    time.sleep(0.05)

def display(move):
    for i in move:
        for j in i:
            print(j, end = " ")
        print()
    print()
    print()
    print()

def get_max_eval(depth, all_moves, alpha, beta):
    for move in all_moves:
        evaluation = minimax(screen, depth-1, False, move, alpha, beta, {})[0]

        if evaluation >= maxEval:
            maxEval = evaluation
            best_move = move

        if depth < 3:
            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break
        return maxEval, best_move

def get_min_eval(depth, all_moves, alpha, beta):
    for move in all_moves:
        strMove = str(move)
        if strMove in memo.keys() and memo[strMove][1]==depth:
            evaluation = strMove
        else:    
            evaluation = minimax(screen, depth-1, True, move, alpha, beta, {})[0]
            memo[str(move)] = (evaluation, depth)
        if evaluation <= minEval:
            minEval = evaluation
            best_move = move
        
        if depth < 3:
            beta = min(beta, minEval)
            if beta<= alpha:
                break
        return minEval, best_move

def pool_max(screen, depth, all_moves, board_layout, alpha, beta):
    p = Pool(4)
    args = [screen, depth, all_moves, board_layout, alpha, beta]
    results = p.starmap(get_max_eval, product(args, repeat=2))
    return results

def pool_min(screen, depth, all_moves, board_layout, alpha, beta):
    p = Pool(4)
    args = [screen, depth, all_moves, board_layout, alpha, beta]
    results = p.starmap(get_min_eval, product(args, repeat=2))
    return results

def dama_in_board(board_layout):
    for i in board_layout:
        for piece in i:
            if piece==3 or piece==4:
                return True
    return False

memo = {}
def normal_minimax(screen, depth, max_player, board_layout, alpha, beta):
    global memo
    if depth <= 0:
        return evaluate(board_layout, max_player), board_layout

    if max_player:
        maxEval = float('-inf')
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:

            strMove = str(move)
            if strMove in memo.keys() and memo[strMove][1]==depth and memo[strMove][2]==True:
                evaluation = memo[strMove][0]
            else:
                evaluation = normal_minimax(screen, depth-1, False, move, alpha, beta)[0]
                memo[str(move)] = (evaluation, depth, True)
            
            if evaluation >= maxEval:
                maxEval = evaluation
                best_move = move

            if depth<4:
                alpha = max(alpha, maxEval)
                if beta<= alpha:
                    break
        return maxEval, best_move
    else:
        minEval = float('inf')
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:

            strMove = str(move)
            if strMove in memo.keys() and memo[strMove][1]==depth and memo[strMove][2]==False:
                evaluation = memo[strMove][0]
            else:
                evaluation = normal_minimax(screen, depth-1, True, move, alpha, beta)[0]
                memo[str(move)] = (evaluation, depth, False)

            if evaluation <= minEval:
                minEval = evaluation
                best_move = move
            
            if depth<4:
                beta = min(beta, minEval)
                if beta<= alpha:
                    break
        return minEval, best_move


def minimax( depth, max_player, board_layout, alpha, beta):
    global memo
    global calculations
    if depth <= 0:
        if max_player:
            turn = 1
        else:
            turn = 2

        return evaluate(board_layout, turn), board_layout

    if max_player:
        maxEval = -1000000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 1)

        if False and force_list!=[]:
            if depth<3:
                depth+=1

        for move in all_moves:

            #draw_move(screen, move)

            strMove = str(move)
            if strMove in game_history.keys() and game_history[strMove]>=2:
                if strMove in memo.keys() and memo[strMove][1]==depth and memo[strMove][2]==True:
                    del memo[strMove]
            if strMove in memo.keys() and memo[strMove][1]==depth and memo[strMove][2]==True:
                evaluation = memo[strMove][0]
            else:
                next_force = check_for_force(move, 2)
                if next_force!=[] and calculations < 20000:
                    calculations+=1
                    evaluation = minimax(depth, False, move, alpha, beta)[0]
                else:
                    evaluation = minimax(depth-1, False, move, alpha, beta)[0]
                    memo[str(move)] = (evaluation, depth, True)
            #display(move)
            if evaluation >= maxEval:
                maxEval = evaluation
                best_move = move

            if depth<4:
                alpha = max(alpha, maxEval)
                if beta<= alpha:
                    break
        return maxEval, best_move
    else:
        minEval = 1000000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        if False and force_list!=[]:
            if depth<3:
                depth+=1

        for move in all_moves:
            #draw_move(screen, move)

            strMove = str(move)
            if strMove in game_history.keys() and game_history[strMove]>=2:
                if strMove in memo.keys() and memo[strMove][1]==depth and memo[strMove][2]==True:
                    del memo[strMove]
            if strMove in memo.keys() and memo[strMove][1]==depth and memo[strMove][2]==False:
                evaluation = memo[strMove][0]
            else:
                next_force = check_for_force(move, 1)
                if next_force != [] and calculations < 20000:
                    calculations+=1
                    evaluation = minimax( depth, True, move, alpha, beta)[0]
                else:
                    evaluation = minimax( depth-1, True, move, alpha, beta)[0]
                    memo[str(move)] = (evaluation, depth, False)
            #display(move)
            if evaluation <= minEval:
                minEval = evaluation
                best_move = move
            
            if depth<4:
                beta = min(beta, minEval)
                if beta<= alpha:
                    break
        return minEval, best_move

def minimax_depth1(screen, depth, max_player, board_layout, alpha, beta):
    global calculations
    global memo

    if depth<=0:
        if max_player:
            turn = 1
        else:
            turn = 2

        return evaluate(board_layout, turn), board_layout

    if max_player:
        maxEval = evaluation = float('-inf')
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            #draw_move(screen, move)
            evaluation = float('-inf')
            if force_list!=[]:
                evaluation = minimax_depth1(screen, depth-1, False, move, alpha, beta)[0]
            else:
                print("hi")
            if evaluation >= maxEval:
                maxEval = evaluation
                best_move = move

        return maxEval, best_move

    else:
        minEval = evaluation = float('inf')
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:
            
            next_force = check_for_force(move, 1)
            evaluation = float('inf')
            if next_force != []:
                calculations+=1
                evaluation = minimax_depth1(screen, depth-1, True, move, alpha, beta)[0]
            if force_list != []:
                #draw_move(screen, move)
                evaluation = minimax_depth1(screen, 0, True, move, alpha, beta)[0]
            if evaluation <= minEval:
                minEval = evaluation
                best_move = move

        return minEval, best_move

def move_piece(piece, move, board_layout, parent_list, color):
    row = piece[0]
    col = piece[1]

    r = move[0]
    c = move[1]

    if parent_list!=[] and parent_list!=None:
        for value in parent_list:
            if (r,c) == value[-1]:
                z=0
                for z in range(0, len(value)):
                    if z+1 >= len(value):
                        break
                    old_row = value[z][0]
                    old_col = value[z][1]
                    new_row = value[z+1][0]
                    new_col = value[z+1][1]                                
                    board_layout = eat_piece_if_possible(board_layout, old_row, old_col, new_row, new_col, color)
    else:
        board_layout = eat_piece_if_possible(board_layout, row, col, r, c, color)
        

    aux = board_layout[row][col]
    board_layout[row][col]=board_layout[r][c]
    board_layout[r][c]=aux

    #check if dama

    if r == 0 and color==2:
        board_layout[r][c]=4
    if r == 7 and color==1:
        board_layout[r][c]=3

    return board_layout

def get_all_moves(board_layout, color):
    global fff
    global dama_nom
    global ate_up
    global ate_down
    global ate_right
    global ate_left

    moves = []

    #create force list
    valid_moves = []
    pieces = get_all_pieces(board_layout, color)
    force_list = []
    force_list = check_for_force(board_layout, color)

    if force_list!=[]:
        pieces = force_list
    for piece in pieces:
        color2 = board_layout[piece[0]][piece[1]]
        
        parent_list2 = []
        parent_list = []
        if color2 == 1 or color2 == 2:

            ate_up = False
            ate_down = False
            ate_right = False
            ate_left = False
            fff = False

            valid_moves = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout)
            if fff:
                for move in valid_moves:
                    parent_list2.append ( [(piece[0], piece[1]), move])
                temporary_board_layout = deepcopy(board_layout)
                parent_list = eat_max2_not_dama(piece[0], piece[1], temporary_board_layout, parent_list2, color2)
                aux_list = []
                if parent_list != [] and parent_list!=None:
                    for value in parent_list:
                        aux_list.append(value[-1])
                    valid_moves = aux_list
                    
        if color2 == 3 or color2 == 4:

            ate_up = False
            ate_down = False
            ate_right = False
            ate_left = False
            dama_nom = False

            valid_moves = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout)
            if dama_nom:
                for move in valid_moves:
                    parent_list2.append ( [(piece[0], piece[1]), move])
                temporary_board_layout = deepcopy(board_layout)
                parent_list = eat_max2(piece[0], piece[1], temporary_board_layout, parent_list2, color2, {})
                aux_list = []
                if parent_list != [] and parent_list!=None:
                    for value in parent_list:
                        aux_list.append(value[-1])
                    valid_moves = aux_list
        for move in valid_moves:
            temp_board_layout = deepcopy(board_layout)
            temp_board_layout = deepcopy(move_piece(piece, move, temp_board_layout, parent_list, color2))

            if False:
                if color ==1:
                    Turn = 2
                else:
                    Turn = 1
                
                f_list = chceck_for_force(temp_board_layout, Turn)

                if f_list!=[]:
                    moves.append(temp_board_layout)
                else:
                    moves.insert(0, temp_board_layout)
            
            moves.append(temp_board_layout)

    return moves, force_list

###################################################### ai stuff ##########################################################


board = Board()
jpg = main_menu()
if jpg!="":
    board.surf = pygame.image.load(jpg).convert()
blackpiece = blackPiece()
whitepiece = whitePiece()
board.surf = pygame.transform.scale(board.surf, (800, 800))
blackpiece.surf = pygame.transform.scale(blackpiece.surf, (80, 80))
whitepiece.surf = pygame.transform.scale(whitepiece.surf, (80, 80))
surf = pygame.image.load("images/bcircle.png")
surf = pygame.transform.scale(surf, (20, 20))

redDama = pygame.image.load("images/redDama.png")
redDama = pygame.transform.scale(redDama, (80, 80))
blackDama = pygame.image.load("images/blackDama.png")
blackDama = pygame.transform.scale(blackDama, (80, 80))

clickSound = pygame.mixer.Sound('sound/click.wav')
errorSound = pygame.mixer.Sound('sound/error.aiff')
eatSound = pygame.mixer.Sound('sound/eat.wav')
startSound = pygame.mixer.Sound('sound/start.wav')

valid_moves=[]
selected = False

def eat_piece_if_possible(board_layout, row, col, r, c, color):

    global has_eaten
    global ate_down
    global ate_up
    global ate_right
    global ate_left

    ate_down = False
    ate_up = False
    ate_left = False
    ate_right = False

    if color == 1 or color == 2:
        if c==col:
            if r>row:
                ate_down = True
            else:
                ate_up = True
        else:
            if c>col:
                ate_right = True
            else:
                ate_left = True

    if color == 1:
        if board_layout[(row + r)//2][col] == 2 or board_layout[(row + r)//2][col] == 4:
            board_layout[(row + r)//2][col]=0
            has_eaten = True
            #eatSound.play()
        elif board_layout[row][(col+c)//2] == 2 or board_layout[row][(col+c)//2] == 4:
            board_layout[row][(col+c)//2]=0
            has_eaten = True
            #eatSound.play()
    elif color == 2:
        if board_layout[(row + r)//2][col] == 1 or board_layout[(row + r)//2][col] == 3:
            board_layout[(row + r)//2][col]=0
            has_eaten = True
            #eatSound.play()
        elif board_layout[row][(col+c)//2] == 1 or board_layout[row][(col+c)//2]==3 :
            board_layout[row][(col+c)//2]=0
            has_eaten = True
            #eatSound.play()
    elif color ==3:
        if c==col:
            if r>row:
                big = r
                small = row
                ate_down = True
            else:
                big = row
                small = r
                ate_up = True
            for k in range(small,big):
                if board_layout[k][c] == 2 or board_layout[k][c] == 4:
                    board_layout[k][c]=0
                    has_eaten=True
                    #eatSound.play()

        elif row ==r:
            if c>col:
                big = c
                small = col
                ate_right=True
            else:
                big = col
                small = c
                ate_left=True
            for k in range(small,big):
                if board_layout[r][k] == 2 or board_layout[r][k] == 4:
                    board_layout[r][k]=0
                    has_eaten=True
                    #eatSound.play()
                    
    elif color == 4:
        if c==col:
            if r>row:
                big = r
                small = row
                ate_down = True
            else:
                big = row
                small = r
                ate_up = True
            for k in range(small,big):
                if board_layout[k][c] == 1 or board_layout[k][c] == 3:
                    board_layout[k][c]=0
                    has_eaten=True
                    #eatSound.play()

        elif row ==r:
            if c>col:
                big = c
                small = col
                ate_right = True
            else:
                big = col
                small = c
                ate_left = True
            for k in range(small,big):
                if board_layout[r][k] == 1 or board_layout[r][k] == 3:
                    board_layout[r][k]=0
                    has_eaten=True
                    #eatSound.play()

    return board_layout


def draw(screen, board, i, j):
    if board.layout[i][j]==1:
        screen.blit(blackpiece.surf, (10+j*100, 10 + i*100))
    elif board.layout[i][j]==2:
        screen.blit(whitepiece.surf, (10+j*100, 10 + i*100))
    elif board.layout[i][j]==3:
        screen.blit(blackDama, (10+j*100, 10 + i*100))
    elif board.layout[i][j]==4:
        screen.blit(redDama, (10+j*100, 10 + i*100))

def eat_max2_not_dama(row, col, board_layout, parent_list,  color):
    if parent_list!=[] and len(parent_list[0])>20:
        return parent_list
    global ate_right
    global ate_down
    global ate_left
    global ate_up
    global piece_to_eat
    global fff
    fff = False
    valid_moves = get_valid_moves(row, col, color, 0, [], board_layout)
    if fff:
        new_list = []
        big_dama_list = []
        for move in valid_moves:
            ate_right = False
            ate_down = False
            ate_left = False
            ate_up = False
            
            if move[1]==col:
                if move[0]>row:
                    ate_down = True
                else:
                    ate_up = True
            else:
                if move[1]>col:
                    ate_right = True
                else:
                    ate_left = True

            fff = False
            get_valid_moves(move[0], move[1], color, 0, [], board_layout)

            if fff:
                parent = (row, col)
                new_list.append(move)
                for value in parent_list:
                    if parent == value[-1]:
                        big_dama_list.append(value + [move])

        if big_dama_list!=[]:
            parent_list = big_dama_list        
        if new_list == []:
            aux_list2 = []
            for  value in parent_list:
                last = value[-1]
                pre_last = value[-2]

                ate_right = False
                ate_down = False
                ate_left = False
                ate_up = False
                dama_nom = False
                
                
                if last[1]==pre_last[1]:
                    if last[0]>pre_last[0]:
                        ate_down = True
                    else:
                        ate_up = True
                elif pre_last[0] == last[0]:
                    if last[1]>pre_last[1]:
                        ate_right = True
                    else:
                        ate_left = True

                valid_moves2 = get_valid_moves(last[0], last[1], color, 0, [], board_layout)
                if fff:
                    for move in valid_moves2:
                        aux_list2.append(value + [move])
            parent_list = aux_list2

            return parent_list
        else:
            maxp = []
            valid_moves =  new_list
            for move in valid_moves:
                ate_down = False
                ate_left = False
                ate_right = False
                ate_up = False

                new_board_layout = deepcopy(board_layout)
                new_board_layout = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color)

                p2 = eat_max2_not_dama(move[0], move[1], board_layout, parent_list, color)
                if maxp==[] or len(p2[0]) > len(maxp[0]):
                    maxp = p2
            return maxp
    else:
        return []
        

def eat_max2(row, col, board_layout, parent_list,  color, eaten_pieces):
    
    global ate_right
    global ate_down
    global ate_left
    global ate_up
    global dama_nom
    global piece_to_eat

    dama_nom = False
    valid_moves = get_valid_moves(row, col, color, 0, [], board_layout)

    if dama_nom:
        new_list = []
        big_dama_list = []

        for move in valid_moves:
            ate_right = False
            ate_down = False
            ate_left = False
            ate_up = False
            dama_nom = False
            
            if move[1]==col:
                if move[0]>row:
                    ate_down = True
                else:
                    ate_up = True
            else:
                if move[1]>col:
                    ate_right = True
                else:
                    ate_left = True
    
            get_valid_moves(move[0], move[1], color, 0, [], board_layout)
    
            if dama_nom:
                parent = (row, col)
                new_list.append(move)
                for value in parent_list:
                    if parent == value[-1]:
                        big_dama_list.append(value + [move])


        if big_dama_list!=[]:
            parent_list = big_dama_list        
        if new_list == []:
            aux_list2 = []
            for  value in parent_list:
                last = value[-1]
                pre_last = value[-2]

                ate_right = False
                ate_down = False
                ate_left = False
                ate_up = False
                dama_nom = False
                
                if last[1]==pre_last[1]:
                    if last[0]>pre_last[0]:
                        ate_down = True
                    else:
                        ate_up = True
                else:
                    if last[1]>pre_last[1]:
                        ate_right = True
                    else:
                        ate_left = True

                valid_moves2 = get_valid_moves(last[0], last[1], color, 0, [], board_layout)
                
                if dama_nom:
                    for move in valid_moves2:
                        aux_list2.append(value + [move])
                parent_list = aux_list2

            return parent_list
        else:
            valid_moves =  new_list
            maxp=[]
            for move in valid_moves:
                
                ate_down = False
                ate_left = False
                ate_right = False
                ate_up = False

                new_board_layout = deepcopy(board_layout)
                new_board_layout = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color)

                p2 = eat_max2(move[0], move[1], new_board_layout, parent_list, color, eaten_pieces)
                if maxp==[] or len(p2[0]) > len(maxp[0]):
                    maxp = p2
            return maxp
    else:
        return []


def pool_func(all_moves):
    p = Pool(4)
    p.map

def check_for_move(b1, b2, color):
    i=0
    while i<8:
        j=0
        while j<8:
            if b2[i][j] == 0:
                if b1[i][j]==color or b1[i][j]==color+2:
                    return (i, j)
            j+=1
        i+=1
    return(-1, -1)

def count_pieces(board_layout, color):
    count = 0
    for i in range(8):
        for j in range(8):
            if board_layout[i][j] == color or board_layout[i][j] == color+2:
                count+=1
    return count
#main run loop

while running:
    clock.tick(30)
    screen.fill((255, 255, 255))
    screen.blit(board.surf,(0, 0))
    
    #black ai move 
    if False and turn == 1:
        current_board_layout = deepcopy(board.layout)
        calculations = 0
        
        value, new_board_layout = deepcopy(minimax( 4, True, current_board_layout, float('-inf'), float('inf')))
        value2 = -1000000
        if False and  calculations<1000:
            current_board_layout2 = deepcopy(board.layout)
            value2, new_board_layout2 = deepcopy(normal_minimax(screen, 4, True, current_board_layout2, float('-inf'), float('inf')))
            print(value2)

        if False and value2>value:
            print("hi")
            value = value2
            new_board_layout = new_board_layout2

        try:
            game_history[str(new_board_layout)]+=1
        except:
            game_history[str(new_board_layout)]=0

        row_save, col_save = check_for_move(board.layout, new_board_layout, 1)
        print(calculations)
        print("evaluation: ", value)
        board.layout = deepcopy(new_board_layout)
        turn = 2

    
    #red ai move
    if turn == 2:
        current_board_layout = deepcopy(board.layout)
        calculations = 0
        positions_seen = 0
        value, new_board_layout = deepcopy(minimax( 4, False, current_board_layout, float('-inf'), float('inf')))
        print("seen position: ", positions_seen)
        #value, new_board_layout = deepcopy(minimax_depth1(screen, 7, False, current_board_layout, float('-inf'), float('inf')))

        try:
            game_history[str(new_board_layout)]+=1
        except:
            game_history[str(new_board_layout)]=0

        row_save, col_save = check_for_move(board.layout, new_board_layout, 2)
        print(calculations)
        print("evaluation: ", value)
        board.layout = deepcopy(new_board_layout)
        turn = 1
        

    for val1, val2 in force_list:
        if surf4 != None:
            screen.blit(surf4, (val2*100, val1*100))

    for  event in pygame.event.get():
        if event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False
        elif event.type == pygame.QUIT:
            running = False
        
        if event.type == pygame.MOUSEBUTTONDOWN and selected == False:
            ate_up = False
            ate_down = False
            ate_left = False
            ate_right = False
            valid_moves=[]
            pos = pygame.mouse.get_pos()
            col = pos[0]//100
            row = pos[1]//100

            if board.layout[row][col] != 0:
                color = board.layout[row][col]
                if (board.layout[row][col] == turn) or (board.layout[row][col]==3 and turn==1) or (board.layout[row][col]==4 and turn==2):
                    selected=True 
                    force_list = check_for_force(board.layout, turn)
                    if force_list!=[]:
                        surf4 = pygame.image.load("images/frame.png")
                        surf4 = pygame.transform.scale(surf4, (100, 100))
                        if (row,col) not in force_list:
                            valid_moves=[]
                            selected=False
                            errorSound.play()
                            continue
                        else:
                            clickSound.play()
                            valid_moves = get_valid_moves(row, col, board.layout[row][col], 0, [], board.layout)
                            selected=True
                    else:
                        clickSound.play()
                        valid_moves = get_valid_moves(row, col, board.layout[row][col], 0, [], board.layout)
                        selected=True
                    #valid_moves = get_valid_moves(row, col, board.layout[row][col], 0, [])
                    
                    #akel aal aktar
                    parent_list2 = []
                    for move in valid_moves:
                        parent_list2.append ( [(row, col), move])
                    eaten_pieces = {}
                    if color == 3 or color == 4:
                        temp_board_layout = deepcopy(board.layout)
                        parent_list = eat_max2(row, col, temp_board_layout, parent_list2, board.layout[row][col], eaten_pieces)
                    elif color == 1 or color == 2:
                        parent_list = eat_max2_not_dama(row, col, board.layout, parent_list2, board.layout[row][col])     
                    aux_list = []
                    if parent_list != [] and parent_list!=None:
                        for value in parent_list:
                            aux_list.append(value[-1])
                        valid_moves = deepcopy(aux_list)

            continue
 
        if event.type == pygame.MOUSEBUTTONDOWN and selected == True:
            surf4=None
            clickSound.play()
            has_eaten=False
            pos = pygame.mouse.get_pos()
            c = pos[0]//100
            r = pos[1]//100
            ate_up = False
            ate_down = False
            ate_left = False
            ate_right = False
            aux_list = []
            savedPiece = board.layout[row][col]
            
            if (r, c) in valid_moves:
                row_save = row
                col_save = col
                if parent_list!=[] and parent_list!=None:
                    for value in parent_list:
                        if (r,c) == value[-1]:
                            z=0
                            for z in range(0, len(value)):
                                if z+1 >= len(value):
                                    break
                                old_row = value[z][0]
                                old_col = value[z][1]
                                new_row = value[z+1][0]
                                new_col = value[z+1][1]

                                aux = board.layout[old_row][old_col]
                                board.layout[old_row][old_col]=board.layout[new_row][new_col]
                                board.layout[new_row][new_col]=aux
                                
                                eat_piece_if_possible(board.layout, old_row, old_col, new_row, new_col, color)
                                
                                screen.fill((255, 255, 255))
                                screen.blit(board.surf,(0, 0))

                                for i in range(0, 8):
                                    for j in range(0, 8):
                                        if board.layout[i][j]==1:
                                            screen.blit(blackpiece.surf, (10+j*100, 10 + i*100))
                                        elif board.layout[i][j]==2:
                                            screen.blit(whitepiece.surf, (10+j*100, 10 + i*100))
                                        elif board.layout[i][j]==3:
                                            screen.blit(blackDama, (10+j*100, 10 + i*100))
                                        elif board.layout[i][j]==4:
                                            screen.blit(redDama, (10+j*100, 10 + i*100))
                                
                                
                                pygame.display.update()
                                time.sleep(0.3)
                            break
                else:
                    eat_piece_if_possible(board.layout, row, col, r, c, color)
                
                board.layout[r][c] = savedPiece
                board.layout[row][col] = 0

                #check if dama
                if r == 0 and color==2:
                    board.layout[r][c]=4
                if r == 7 and color==1:
                    board.layout[r][c]=3
                selected=False
                valid_moves=[]

                if turn==1:
                    turn = 2
                else:
                    turn = 1

                try:
                    game_history[str(board.layout)]+=1
                except:
                    game_history[str(board.layout)]=0
                    
            else:
                selected = False
                valid_moves=[]    
    
    try:
        pygame.draw.rect(screen, (0, 255, 0), pygame.Rect(col_save*100, row_save*100, 100, 100),  2)
    except:
        pass

    for i in range(0, 8):
        for j in range(0, 8):
            if board.layout[i][j]==1:
                screen.blit(blackpiece.surf, (10+j*100, 10 + i*100))
            elif board.layout[i][j]==2:
                screen.blit(whitepiece.surf, (10+j*100, 10 + i*100))
            elif board.layout[i][j]==3:
                screen.blit(blackDama, (10+j*100, 10 + i*100))
            elif board.layout[i][j]==4:
                screen.blit(redDama, (10+j*100, 10 + i*100))

    for move in valid_moves:
        ro, co = move    
        screen.blit(surf,(40+100*co, 40+100*ro))

    pygame.display.flip()

    #the end of the game
    if no_more_moves_black(board):
        print("RED WINS!!!!")
        time.sleep(5)
        break
    if no_more_moves_white(board):
        print("BLACK WINS!!!!")
        time.sleep(5)
        break
    if str(board.layout) in game_history.keys() and game_history[str(board.layout)]==3:
        print("draw")
        time.sleep(5)
        break
    if count_pieces(board.layout, 1) == 1 and count_pieces(board.layout, 2):
        print("draw")
        time.sleep(5)
        break
    



