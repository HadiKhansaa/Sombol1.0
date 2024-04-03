from copy import deepcopy
import ctypes
import time
import pygame
from numba import njit, jit
import platform
import multiprocessing as mp
from multiprocessing import Pool
from itertools import product
import numpy as np
from ctypes import *
import subprocess
import os

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

game_history = {}
pygame.init()
screen = pygame.display.set_mode((800, 800))
clock = pygame.time.Clock()
turn = 2

piece_to_eat =(-1, -1)

def no_more_moves_black(b):
    for i in b.layout:
        for j in i:
            if j==1 or j==3:
                return False

    return True

def delete_game_history(file_path):
    # Check if file exists
    if os.path.exists(file_path):
        # Delete the file
        os.remove(file_path)

def no_more_moves_white(b):
    for i in b.layout:
        for j in i:
            if j==2 or j==4:
                return False

    return True

def empty_square(row, col):
    pass

def get_valid_moves(row, col, color, streak, valid_moves, board_layout, eat_direction):
    ate_up = (eat_direction == 'u')
    ate_down = (eat_direction == 'd')
    ate_left = (eat_direction == 'l')
    ate_right = (eat_direction == 'r')
    eating_piece = ''
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
                        valid_moves.append((row, col+i+j))
                        eating_piece = 'd'
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
                        valid_moves.append((row, col-i-j))
                        eating_piece = 'd'
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
                        valid_moves.append((row+i+j, col))
                        eating_piece = 'd'
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
                        valid_moves.append((row-i-j, col))
                        eating_piece = 'd'
                    j+=1
            i+=1
        if eating_piece=='':
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
                        valid_moves.append((row, col+i+j))
                        eating_piece = 'd'
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
                        valid_moves.append((row, col-i-j))
                        eating_piece = 'd'
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
                        valid_moves.append((row+i+j, col))
                        eating_piece = 'd'
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
                        valid_moves.append((row-i-j, col))
                        eating_piece = 'd'
                    j+=1
            i+=1
        if eating_piece=='':
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
            eating_piece = 'n'
        if col+2<8 and (board_layout[row][col+1]==2 or board_layout[row][col+1]==4) and board_layout[row][col+2]==0 and streak>=0 and not ate_left:
            streak+=1
            valid_moves.append((row, col+2))
            eating_piece = 'n'
        if col-2>=0 and (board_layout[row][col-1]==2 or board_layout[row][col-1]==4) and board_layout[row][col-2]==0 and streak>=0 and not ate_right:
            streak+=1
            valid_moves.append((row, col-2))
            eating_piece = 'n'
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
            eating_piece = 'n'
        if col+2<8 and (board_layout[row][col+1]==1 or board_layout[row][col+1]==3) and board_layout[row][col+2]==0 and streak>=0 and not ate_left:
            streak+=1
            valid_moves.append((row, col+2))
            eating_piece = 'n'
        if col-2>=0 and (board_layout[row][col-1]==1 or board_layout[row][col-1]==3) and board_layout[row][col-2]==0 and streak>=0 and not ate_right:
            streak+=1
            valid_moves.append((row, col-2))
            eating_piece = 'n'
        if row-1>=0 and board_layout[row-1][col]==0 and streak==0:
            valid_moves.append((row-1, col))
        if col+1<8 and board_layout[row][col+1]==0 and streak ==0:
            valid_moves.append((row, col+1))
        if col-1>=0 and board_layout[row][col-1]==0 and streak ==0:
            valid_moves.append((row, col-1))
    return valid_moves, eating_piece

def check_for_piece_akel(row, col, color, board_layout):

    if color == 1:
        if row+2<8 and (board_layout[row+1][col]==2 or board_layout[row+1][col]==4) and board_layout[row+2][col]==0:
            return 'n'
            
        if col+2<8 and (board_layout[row][col+1]==2 or board_layout[row][col+1]==4) and board_layout[row][col+2]==0:
            return 'n'
            
        if col-2>=0 and (board_layout[row][col-1]==2 or board_layout[row][col-1]==4) and board_layout[row][col-2]==0:
            return 'n'

    elif color ==2:
        if row-2>=0 and (board_layout[row-1][col]==1 or board_layout[row-1][col]==3) and board_layout[row-2][col]==0:
            return 'n'

        if col+2<8 and (board_layout[row][col+1]==1 or board_layout[row][col+1]==3) and board_layout[row][col+2]==0:
            return 'n'

        if col-2>=0 and (board_layout[row][col-1]==1 or board_layout[row][col-1]==3) and board_layout[row][col-2]==0:
            return 'n'

    return ''

def check_for_nom(row, col, color, streak, valid_moves, board_layout):

    eating_piece = get_valid_moves(row, col, color, streak, valid_moves, board_layout, '')[1]

    dama_nom = (eating_piece=='d')
    fff = (eating_piece != '')

    if fff or dama_nom:
        return 1
    else:
        return 0

def check_for_force(board_layout, turn):
    aktar_list = []
    aktar_flag = False
    force_list = []
    maxLength = 3
    
    for row2 in range(8):
        for col2 in range(8):
            color = board_layout[row2][col2]
            if ((color==2 or color==4) and turn==2) or ((color==1 or color==3) and turn==1):
                fff = False
                parent_list2 = []

                if color == 1 or color == 2:
                    eating_piece = check_for_piece_akel(row2, col2, color, board_layout)
                else:
                    valid_moves, eating_piece = get_valid_moves(row2, col2, color, 0, [], board_layout, '')
                
                dama_nom = (eating_piece == 'd')
                fff = (eating_piece != '')

                if fff==True:
                    valid_moves, eating_piece = get_valid_moves(row2, col2, color, 0, [], board_layout, '')
                    parent_list2 = []
                    for move in valid_moves:
                        parent_list2.append([(row2, col2), move])

                    if color == 1 or color == 2:
                        parent_list = eat_max2_not_dama(row2, col2, board_layout, parent_list2, color, '')
                        if parent_list != []:
                            listSize = len(parent_list[0])
                        if parent_list != None and parent_list != [] and listSize>=maxLength:
                            if listSize>maxLength:
                                aktar_list.clear()
                            maxLength = listSize
                            aktar_flag = True
                            for value in parent_list:
                                aktar_list.append(value[0])

                    elif color == 3 or color == 4:
                        parent_list = eat_max2(row2, col2, board_layout, parent_list2, color, '')
                        if parent_list != []:
                            listSize = len(parent_list[0])
                        if parent_list != None and parent_list != [] and listSize>2 and listSize>=maxLength:
                            if listSize>maxLength:
                                aktar_list.clear()
                            maxLength = listSize
                            for value in parent_list:
                                aktar_list.append(value[0])
                            aktar_flag = True
                    if not aktar_flag:
                        force_list.append((row2, col2))
                    
    if aktar_flag:
        force_list = aktar_list
        #for move in force_list:
            #if move not in aktar_list:
                #force_list.remove(move)
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

    testSurf = pygame.image.load("images/positionBoard.jpeg").convert()
    testSurf = pygame.transform.scale(testSurf, (800, 800))

    #make this board
    # 0 0 0 0 0 0 0 0
    # 1 1 1 0 1 1 1 1
    # 0 0 0 0 1 1 0 0
    # 1 1 1 1 0 1 1 2
    # 0 0 0 0 0 2 0 0
    # 2 2 2 2 2 2 2 2
    # 2 2 2 2 2 0 0 0
    # 0 0 0 0 0 0 0 0

    layout3 = [
             [0, 0, 0, 0, 3, 0, 0, 0]
            ,[1, 1, 2, 0, 0, 1, 1, 1]
            ,[0, 0, 0, 0, 2, 1, 1, 0]
            ,[1, 1, 0, 2, 0, 1, 1, 2]
            ,[0, 0, 0, 2, 0, 2, 2, 0]
            ,[2, 2, 0, 2, 0, 2, 2, 2]
            ,[2, 2, 0, 4, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ]

    #test endgame
    layout8 = [

                # create this board
                # 0 0 0 0 0 0 0 0 
                # 0 0 1 0 0 0 0 1
                # 1 1 0 0 0 0 1 1
                # 1 0 1 0 0 0 1 1
                # 2 0 0 2 0 2 2 0
                # 2 0 2 0 0 2 2 2
                # 0 2 0 0 0 0 0 0
                # 0 0 0 0 0 0 0 0
                
                [0, 0, 0, 0, 0, 0, 0, 0]
                ,[0, 0, 1, 0, 0, 0, 0, 1]
                ,[1, 1, 0, 0, 0, 0, 1, 1]
                ,[1, 0, 1, 0, 0, 0, 1, 1]
                ,[2, 0, 0, 2, 0, 2, 2, 0]
                ,[2, 0, 2, 0, 0, 2, 2, 2]
                ,[0, 2, 0, 0, 0, 0, 0, 0]
                ,[0, 0, 0, 0, 0, 0, 0, 0]
               ]
    
    layout7 = [
                # create this board
                # 0 0 0 0 0 0 0 0
                # 1 0 0 0 0 0 0 0
                # 1 0 0 0 0 0 0 0
                # 0 1 0 0 0 0 0 0
                # 2 0 0 0 0 2 0 0
                # 2 0 2 0 0 0 0 0
                # 0 0 0 0 0 0 0 0
                # 0 0 0 0 3 0 0 0 
        
                [0, 0, 0, 0, 0, 0, 0, 0]
                ,[1, 0, 0, 0, 0, 0, 0, 0]
                ,[1, 0, 0, 0, 0, 0, 0, 0]
                ,[0, 1, 0, 0, 0, 0, 0, 0]
                ,[2, 0, 0, 0, 0, 2, 0, 0]
                ,[2, 0, 2, 0, 0, 0, 0, 0]
                ,[0, 0, 0, 0, 0, 0, 0, 0]
                ,[0, 0, 0, 0, 3, 0, 0, 0]
        

                ]

    
    #bugged 1: fixed
    layout5 = [
            [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 2, 0, 0, 0, 0],
            [0, 0, 0, 0, 3, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0],
            [0, 4, 0, 0, 0, 0, 0, 0]
        ]

    layout10 = [
                # create this board
                # 0 0 0 0 0 0 0 0
                # 1 1 1 1 0 1 1 1
                # 1 1 0 0 1 0 1 1
                # 0 0 0 0 0 0 1 1
                # 2 2 0 0 2 2 2 0
                # 2 0 0 2 2 0 2 2
                # 2 0 0 2 0 0 2 2
                # 0 0 0 0 0 0 0 0 


                [0, 0, 0, 0, 0, 0, 0, 0]
                ,[1, 1, 0, 1, 0, 1, 1, 1]
                ,[1, 1, 1, 0, 1, 0, 1, 1]
                ,[0, 0, 0, 0, 0, 0, 1, 1]
                ,[2, 0, 0, 2, 2, 2, 2, 0]
                ,[2, 0, 0, 2, 2, 0, 2, 2]
                ,[2, 0, 0, 2, 0, 0, 2, 2]
                ,[0, 0, 0, 0, 0, 0, 0, 0]
    ]   
    layout9 = [
                # create this board
                # 0 0 0 0 0 0 0 3
                # 0 1 0 0 1 1 1 0
                # 0 0 0 0 0 1 1 0 
                # 0 1 0 1 0 0 0 0
                # 0 0 0 0 2 2 2 1
                # 0 2 0 2 2 0 0 2
                # 0 2 2 0 0 0 0 2
                # 0 0 0 0 0 0 0 0
        
                [0, 0, 0, 0, 0, 0, 0, 3]
                ,[0, 1, 0, 0, 1, 1, 1, 0]
                ,[0, 0, 0, 0, 0, 1, 1, 0]
                ,[0, 1, 0, 1, 0, 0, 0, 0]
                ,[0, 0, 0, 0, 2, 2, 2, 1]
                ,[0, 2, 0, 2, 2, 0, 0, 2]
                ,[0, 2, 2, 0, 0, 0, 0, 2]
                ,[0, 0, 0, 0, 0, 0, 0, 0]
    ]
    
    #bugged 2: 
    layout6 = [
            #  create this broad
            # 0 0 0 4 0 0 0 0
            # 1 0 0 0 0 0 0 0
            # 1 0 0 0 0 0 0 0
            # 1 0 0 0 0 1 1 1
            # 2 0 0 0 0 0 0 0
            # 2 2 0 0 0 0 0 2
            # 0 0 0 0 0 0 0 2
            # 0 0 0 3 0 0 0 0
        
             [0, 0, 0, 0, 0, 0, 0, 0]
            ,[0, 0, 0, 1, 1, 0, 0, 0]
            ,[1, 0, 0, 1, 1, 1, 0, 1]
            ,[0, 0, 0, 1, 1, 0, 0, 0]
            ,[2, 2, 4, 0, 0, 0, 0, 2]
            ,[0, 0, 2, 2, 2, 0, 2, 2]
            ,[0, 0, 2, 0, 0, 0, 0, 2]
            ,[0, 0, 0, 0, 0, 3, 0, 0]

            # [0, 0, 0, 0, 0, 0, 0, 0]
            # ,[1, 2, 1, 1, 1, 0, 1, 1]
            # ,[0, 0, 0, 1, 1, 1, 1, 1]
            # ,[1, 0, 1, 0, 0, 0, 0, 0]
            # ,[0, 0, 0, 0, 2, 0, 2, 2]
            # ,[2, 0, 0, 0, 0, 0, 0, 0]
            # ,[2, 2, 0, 2, 2, 0, 0, 2]
            # ,[0, 0, 0, 0, 0, 0, 0, 0]

            
        ]
                                            
         
    layout = [
                [0, 0, 0, 0, 0, 0, 0, 0],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [1, 1, 1, 1, 1, 1, 1, 1],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 0, 0, 0, 0, 0, 0],
                [2, 2, 2, 2, 2, 2, 2, 2],
                [2, 2, 2, 2, 2, 2, 2, 2],
                [0, 0, 0, 0, 0, 0, 0, 0]
               ]

    layout_photo = get_layout(testSurf)

    layout2 = [
                # create this board
                # 0 0 0 0 0 0 0 4 
                # 1 1 0 0 0 1 0 0 
                # 1 1 1 0 0 0 0 0 
                # 1 1 1 0 0 0 0 0 
                # 2 0 0 2 0 0 0 0 
                # 2 2 0 0 2 0 2 2 
                # 0 2 0 1 0 0 0 0 
                # 0 0 0 0 0 0 0 0 
                [0, 0, 0, 0, 0, 0, 0, 4]
                ,[1, 1, 0, 0, 0, 1, 0, 0]
                ,[1, 1, 1, 0, 0, 0, 0, 0]
                ,[1, 1, 1, 0, 0, 0, 0, 0]
                ,[2, 0, 0, 2, 0, 0, 0, 0]
                ,[2, 2, 0, 0, 2, 0, 2, 2]
                ,[0, 2, 0, 1, 0, 0, 0, 0]
                ,[0, 0, 0, 0, 0, 0, 0, 0]
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
            if board_layout[1][0] != 1 and board_layout[1][1] != 1 and board_layout[1][0] != 3 and board_layout[1][1] != 3:
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

    parent_list2 = []
    parent_list = []
    valid_moves, eating_piece = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout, '')
    fff = (eating_piece!='')
    if fff:
        for move in valid_moves:
            parent_list2.append ( [(piece[0], piece[1]), move])
        
        if color2 == 1 or color2 == 2:
            parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, '')
            aux_list = []
            if parent_list != [] and parent_list!=None:
                for value in parent_list:
                    aux_list.append(value[-1])
                valid_moves = aux_list

        elif color2==3 or color2 == 4:
            parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, '')
            aux_list = []
            if parent_list != [] and parent_list!=None:
                for value in parent_list:
                    aux_list.append(value[-1])
                valid_moves = aux_list

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
    global game_history

    #draw
    if str(board_layout) in game_history.keys():
        if game_history[str(board_layout)]>=2:
            return 0

    sum = 0
    balance_black = 0
    balance_red = 0
    all_pieces = 0
    red_pieces = 0
    black_pieces = 0

    positions_seen+=1

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
                    sum+=0.7

                #double kesh
                if False and turn==1 and double_kesh(board_layout, i, j, turn):
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
                    sum-=0.7
                
                #double kesh
                if False and turn==2 and double_kesh(board_layout, i, j, turn):
                    sum-=0.3


            elif piece == 3:
                black_pieces+=1
                sum+=3.5
            elif piece == 4:
                red_pieces+=1
                sum-=3.5

    sum = sum - abs(balance_black)/20 + abs(balance_red)/20

    all_pieces = black_pieces + red_pieces

    if red_pieces>black_pieces:
        sum += all_pieces*0.005
    elif black_pieces>red_pieces:
        sum -= all_pieces*0.005

    #wins
    if black_pieces==0:
        sum = float('-inf')
    if red_pieces==0:
        sum = float('inf')

    
    #draw
    if black_pieces==1 and red_pieces==1:
        sum = 0

               
    return sum

def evaluate_int(board_layout, turn):
    global game_history

    #draw
    #if str(board_layout) in game_history.keys():
        #if game_history[str(board_layout)]>=2:  
            #return 0

    sum = 0
    balance_black = 0
    balance_red = 0
    all_pieces = 0
    red_pieces = 0
    black_pieces = 0


    #if there's 1 forced move evalute after it being played
    #black
    if False and turn == 1:
        force_list1 = check_for_force(board_layout, 1)

        if len(force_list1) == 1:
            print("hi")
            forced_piece = force_list1[0]
            valid_moves, parent_list = get_valid_moves2(board_layout, forced_piece, board_layout[forced_piece[0]][forced_piece[1]])

            if len(valid_moves) == 1:
                forced_move = valid_moves[0]
                temp_board_layout = board_layout.copy()
                move_piece(forced_piece, forced_move, temp_board_layout, parent_list, board_layout[forced_piece[0]][forced_piece[1]])
                board_layout = temp_board_layout
                turn = 2
        
    #red
    if False and turn == 2:
        force_list1 = check_for_force(board_layout, 2)
        
        if len(force_list1) == 1:
            print("hi")
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
                    sum+=7
                    pass

                black_pieces+=1
                #spreading the pieces
                if j<=3:
                    balance_black+=1
                else:
                    balance_black-=1

                sum+=100 #piece = 1 pnt

                sum += i # the higher the better

                if i == 6: #pre-dama
                    sum+=150
                    if turn == 1:
                        sum+=40
                #important rows
                if i==5: 
                    sum+=40
                if i==4 and turn == 1:
                    sum+=30
                    if j==0 or j==7:
                        sum+=10
                
                
                # 4 important squares
                if j==0 or j==7:
                    if i == 5 and turn == 1:
                        sum+=80
                    if i == 5 and turn == 2:
                        sum+=20
                    if i == 6 and turn == 1:
                        sum+=50
                
                #if on 5th row and passage is clear the piece is dangerous
                if i==4 and turn==1 and passage_is_clear(board_layout, i, j, turn):
                    sum+=70

                #double kesh
                if False and turn==1 and double_kesh(board_layout, i, j, turn):
                    sum+=30
                    
            elif piece == 2:
                #sides are good
                if j==0 or j==7:
                    sum-=7
                    pass
                red_pieces+=1
                #spreading the pieces
                if j<=3:
                    balance_red+=1
                else:
                    balance_red-=1

                sum-=100
                sum -= (7-i)
                if i == 1:
                    sum-=150
                    if turn==2:
                        sum-=40
                if i == 2:
                    sum-=40
                if i == 3 and turn == 2:
                    sum-=30
                    if j==0 or j==7:
                        sum-=10

                # 4 important squares
                if j==0 or j==7:
                    if i == 2 and turn == 2:
                        sum-=80
                    if i == 2 and turn == 1:
                        sum-=20
                    if i == 1 and turn == 2:
                        sum-=50
                
                #if on 5th row and passage is clear the piece is dangerous
                if i==3 and turn==2 and passage_is_clear(board_layout, i, j, turn):
                    sum-=70
                
                #double kesh
                if False and turn==2 and double_kesh(board_layout, i, j, turn):
                    sum-=30


            elif piece == 3:
                black_pieces+=1
                sum+=450
            elif piece == 4:
                red_pieces+=1
                sum-=450

    sum = sum - 100*abs(balance_black)/20 + 100*abs(balance_red)/20

    all_pieces = black_pieces + red_pieces

    if red_pieces>black_pieces:
        sum += all_pieces//2
    elif black_pieces>red_pieces:
        sum -= all_pieces//2

    #wins
    if black_pieces==0:
        sum = float('-inf')
    if red_pieces==0:
        sum = float('inf')

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

def dama_in_board(board_layout):
    for i in board_layout:
        for piece in i:
            if piece==4 or piece ==3:
                return True
    return False

memo = {}

def draw(game_history, board):
    try:
        if game_history[str(board)]>=2:
            return True
        else:
            return False
    except:
        return False
    
def check_for_dama_move(initial_board, move_board, turn):
    for i in range(8):
        for j in range(8):
            if initial_board[i][j]==turn+2 and move_board[i][j]!=turn+2:
                return True
    return False

def dama_in_board2(board_layout, turn):
    for i in board_layout:
        for piece in i:
            if piece == turn + 2:
                return True
    return False

calculations = 0

def minimax(depth, max_player, board_layout, alpha, beta, max_depth, max_calculations):
    global memo
    global calculations
    global positions_seen
    global game_history
    if depth <= 0:
        positions_seen+=1
        if max_player:
            turn = 1
        else:
            turn = 2

        #returnVal =  libCalc.evaluate(
        #board_layout[0][0], board_layout[0][1], board_layout[0][2], board_layout[0][3], board_layout[0][4], board_layout[0][5], board_layout[0][6], board_layout[0][7],
        #board_layout[1][0], board_layout[1][1], board_layout[1][2], board_layout[1][3], board_layout[1][4], board_layout[1][5], board_layout[1][6], board_layout[1][7],
        #board_layout[2][0], board_layout[2][1], board_layout[2][2], board_layout[2][3], board_layout[2][4], board_layout[2][5], board_layout[2][6], board_layout[2][7],
        #board_layout[3][0], board_layout[3][1], board_layout[3][2], board_layout[3][3], board_layout[3][4], board_layout[3][5], board_layout[3][6], board_layout[3][7],
        #board_layout[4][0], board_layout[4][1], board_layout[4][2], board_layout[4][3], board_layout[4][4], board_layout[4][5], board_layout[4][6], board_layout[4][7],
        #board_layout[5][0], board_layout[5][1], board_layout[5][2], board_layout[5][3], board_layout[5][4], board_layout[5][5], board_layout[5][6], board_layout[5][7],
        #board_layout[6][0], board_layout[6][1], board_layout[6][2], board_layout[6][3], board_layout[6][4], board_layout[6][5], board_layout[6][6], board_layout[6][7],
        #board_layout[7][0], board_layout[7][1], board_layout[7][2], board_layout[7][3], board_layout[7][4], board_layout[7][5], board_layout[7][6], board_layout[7][7], turn)

        return evaluate_int(board_layout, turn), board_layout

    if max_player:
        maxEval = -100000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 1)

        #dama_in_board_flag = dama_in_board2(board_layout, 1)

        for move in all_moves:

            #draw_move(screen, move)

            #dama_moved = False
            #if dama_in_board_flag:
                #dama_moved = check_for_dama_move(board_layout, move, 1)

            strMove = str(move)
            drawFlag = False
            if False and  draw(game_history, move):
                evaluation = 0
                drawFlag = True

            inDict = False
            try:
                memo[strMove][1]
                inDict = True
            except:
                inDict = False
            
            if not drawFlag:
                if inDict and memo[strMove][1]==depth and memo[strMove][2]==True:
                    evaluation = memo[strMove][0]
                else:
                    next_force = check_for_force(move, 2)

                    if next_force!=[] and calculations < max_calculations:
                        calculations+=1
                        evaluation, board = minimax(depth, False, move, alpha, beta, max_depth, max_calculations)
                    else:
                        evaluation, board = minimax(depth-1, False, move, alpha, beta, max_depth, max_calculations)
                        memo[str(move)] = (evaluation, depth, True)
            #display(move)
            if evaluation > maxEval:
                maxEval = evaluation
                best_move = move

            if True or depth<max_depth:
                alpha = max(alpha, maxEval)
                if beta<= alpha:
                    break
        return maxEval, best_move
    else:
        minEval = 100000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        if False and force_list!=[]:
            if depth<3:
                depth+=1

        #dama_in_board_flag = dama_in_board2(board_layout, 2)

        for move in all_moves:

            #draw_move(screen, move)

            #dama_moved = False
            #if dama_in_board_flag:
                #dama_moved = check_for_dama_move(board_layout, move, 2)
            
            strMove = str(move)
            drawFlag = False
            if False and draw(game_history, move):
                evaluation = 0
                drawFlag = True
            
            inDict = False
            try:
                memo[strMove][1]
                inDict = True
            except:
                inDict = False
            
            if not drawFlag:
                if inDict and memo[strMove][1]==depth and memo[strMove][2]==False:
                    evaluation = memo[strMove][0]
                else:
                    next_force = check_for_force(move, 1)
                    if next_force != [] and calculations < max_calculations:
                        calculations+=1
                        evaluation, board = minimax(depth, True, move, alpha, beta, max_depth, max_calculations)
                    else:
                        evaluation, board = minimax(depth-1, True, move, alpha, beta, max_depth, max_calculations)
                        memo[str(move)] = (evaluation, depth, False)
            #display(move)
            if evaluation < minEval:
                minEval = evaluation
                best_move = move
            
            if True or depth<max_depth:
                beta = min(beta, minEval)
                if beta<= alpha:
                    break
        return minEval, best_move

def classic_minimax(depth, max_player, board_layout, alpha, beta):
    global memo
    global calculations
    global positions_seen
    global game_history

    if depth <= 0:
        positions_seen+=1
        
        if max_player:
            turn = 1
        else:
            turn = 2

        return evaluate_int(board_layout, turn), board_layout

    if max_player:
        maxEval = -100000
        best_move = None
        
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            #draw_move(screen, move)

            strMove = str(move)
            drawFlag = False
            if draw(game_history, move):
                evaluation = 0
                drawFlag = True

            inDict = False
            try:
                memo[strMove][1]
                inDict = True
            except:
                inDict = False
            
            if not drawFlag:
                if inDict and memo[strMove][1]==depth and memo[strMove][2]==True:
                    evaluation = memo[strMove][0]
                else:
                    evaluation, board = classic_minimax(depth-1, False, move, alpha, beta)
                    memo[str(move)] = (evaluation, depth, True)

            if evaluation > maxEval:
                maxEval = evaluation
                best_move = move
            
            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break
            

        return maxEval, best_move
    else:
        minEval = 100000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:      
            #draw_move(screen, move)
            strMove = str(move)
            drawFlag = False
            if draw(game_history, move):
                evaluation = 0
                drawFlag = True
            
            inDict = False
            try:
                memo[strMove][1]
                inDict = True
            except:
                inDict = False
            
            if not drawFlag:
                if inDict and memo[strMove][1]==depth and memo[strMove][2]==False:
                    evaluation = memo[strMove][0]
                else:
                    evaluation, board = classic_minimax(depth-1, True, move, alpha, beta)
                    memo[str(move)] = (evaluation, depth, False)

            if evaluation < minEval:
                minEval = evaluation
                best_move = move
            
            beta = min(beta, minEval)
            if beta<= alpha:
                break
        return minEval, best_move

def classic_minimax2(depth, max_player, board_layout, alpha, beta):
    global memo
    global calculations
    global positions_seen
    global game_history

    if depth <= 0:
        if max_player:
            turn = 1
        else:
            turn = 2
        return evaluate_int(board_layout, turn), board_layout

    if max_player:
        maxEval = -100000
        best_move = None
        
        all_moves = get_all_moves(board_layout, 1)[0]

        for move in all_moves:
            positions_seen+=1
            evaluation = classic_minimax2(depth-1, False, move, alpha, beta)[0]

            if evaluation > maxEval:
                maxEval = evaluation
                best_move = move

            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break

        return maxEval, best_move
    else:
        minEval = 100000
        best_move = None
        all_moves = get_all_moves(board_layout, 2)[0]

        for move in all_moves:
            positions_seen+=1
            evaluation = classic_minimax2(depth-1, True, move, alpha, beta)[0]


            if evaluation < minEval:
                minEval = evaluation
                best_move = move
            
            beta = min(beta, minEval)
            if beta<= alpha:
                break

        return minEval, best_move

def minimax_pro(depth, max_player, board_layout, alpha, beta, akel_depth):
    global memo
    global calculations
    global positions_seen
    global game_history

    if depth <= 0:
        positions_seen+=1
        
        if max_player:
            turn = 1
        else:
            turn = 2

        return evaluate_int(board_layout, turn), board_layout

    if max_player:
        maxEval = -100000
        best_move = None
        
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            #draw_move(screen, move)

            strMove = str(move)
            drawFlag = False
            if draw(game_history, move):
                evaluation = 0
                drawFlag = True

            inDict = False
            try:
                memo[strMove][1]
                inDict = True
            except:
                inDict = False
            
            if not drawFlag:
                if inDict and memo[strMove][1]==depth and memo[strMove][2]==True:
                    evaluation = memo[strMove][0]
                else:
                    if force_list!=[] and akel_depth<4:
                        evaluation, board = minimax_pro(depth, False, move, alpha, beta, akel_depth+1)
                    else:
                        if akel_depth>0:
                            evaluation, board = minimax_pro(1, False, move, alpha, beta, 100)
                        else:
                            evaluation, board = minimax_pro(depth-1, False, move, alpha, beta, 0)
                            memo[str(move)] = (evaluation, depth, True)

            if evaluation > maxEval:
                maxEval = evaluation
                best_move = move
            
            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break
            

        return maxEval, best_move
    else:
        minEval = 100000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:      
            #draw_move(screen, move)
            strMove = str(move)
            drawFlag = False
            if draw(game_history, move):
                evaluation = 0
                drawFlag = True
            
            inDict = False
            try:
                memo[strMove][1]
                inDict = True
            except:
                inDict = False
            
            if not drawFlag:
                if inDict and memo[strMove][1]==depth and memo[strMove][2]==False:
                    evaluation = memo[strMove][0]
                else:
                    if force_list!=[] and akel_depth<4:
                        evaluation, board = minimax_pro(depth, True, move, alpha, beta, akel_depth+1)
                    else:
                        if False and akel_depth>0:
                            evaluation, board = minimax_pro(0, True, move, alpha, beta, 100)
                        else:
                            evaluation, board = minimax_pro(depth-1, True, move, alpha, beta, 0)
                            memo[str(move)] = (evaluation, depth, False)

            if evaluation < minEval:
                minEval = evaluation
                best_move = move
            
            beta = min(beta, minEval)
            if beta<= alpha:
                break
        return minEval, best_move

def won_game(board_layout, color):
    for i in range(8):
        for j in range(8):
            if board_layout[i][j]!=color and board_layout[i][j]!=color+2 and board_layout[i][j]!=0:
                return False
    return True

def minimax_pro2(depth, max_player, board_layout, alpha, beta, akel_depth, akel_player, akling, memo_pro):
    #global memo
    global calculations
    global positions_seen
    global game_history

    if akling and depth==0 and akel_depth<5:
        depth=1

    if depth <= 0:
        positions_seen+=1
        
        if max_player:
            turn = 1
        else:
            turn = 2

        return evaluate_int(board_layout, turn), board_layout

    if max_player:
        maxEval = float('-inf')
        best_move = None
        calculations+=1
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            #draw_move(screen, move)

            #strMove = str(move)
            drawFlag = False
            if False and draw(game_history, move):
                evaluation = 0
                drawFlag = True

            #inDict = False
            if False:
                try:
                    memo_pro[strMove][1]
                    inDict = True
                except:
                    inDict = False
                
            if not drawFlag:
                if False and inDict and memo_pro[strMove][1]==depth and memo_pro[strMove][2]==True:
                    evaluation = memo_pro[strMove][0]
                else:
                    if force_list!=[] and akel_depth<5:
                        evaluation, board = minimax_pro2(depth, False, move, alpha, beta, akel_depth+1, False, True, memo_pro)
                    else:
                        if (akel_player == False) and akel_depth>2:
                            evaluation, board = minimax_pro2(0, False, move, alpha, beta, 100, False, False, memo_pro)
                        else:
                            evaluation, board = minimax_pro2(depth-1, False, move, alpha, beta, 0, False, False, memo_pro)
                            #memo_pro[str(move)] = (evaluation, depth, True)

            if evaluation > maxEval:
                maxEval = evaluation
                best_move = move
            
            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break
            

        return maxEval, best_move
    else:
        minEval = float('inf')
        best_move = None
        calculations+=1
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:      
            #draw_move(screen, move)
            #strMove = str(move)
            drawFlag = False
            if False and draw(game_history, move):
                evaluation = 0
                drawFlag = True
            
            #inDict = False
            if False:
                try:
                    memo_pro[strMove][1]
                    inDict = True
                except:
                    inDict = False
            
            if not drawFlag:
                if False and inDict and memo_pro[strMove][1]==depth and memo_pro[strMove][2]==False:
                    evaluation = memo_pro[strMove][0]
                else:
                    if force_list!=[] and akel_depth<5:
                        evaluation, board = minimax_pro2(depth, True, move, alpha, beta, akel_depth+1, True, True, memo_pro)
                    else:
                        if (akel_player == True) and akel_depth>2:
                            evaluation, board = minimax_pro2(0, True, move, alpha, beta, 100, True, False, memo_pro)
                        else:
                            evaluation, board = minimax_pro2(depth-1, True, move, alpha, beta, 0, True, False, memo_pro)
                            #memo_pro[str(move)] = (evaluation, depth, False)

            if evaluation < minEval:
                minEval = evaluation
                best_move = move
            
            beta = min(beta, minEval)
            if beta<= alpha:
                break
        return minEval, best_move

def minimax_depth1(screen, max_player, board_layout, akel_depth, alpha, beta, memo2):
    global positions_seen
    if (max_player == False) and (check_for_force(board_layout, 2)!=[]):

        all_moves, force_list = get_all_moves(board_layout, 2)
        no_more_akel = False
        minEval = 10000
        best_move = None

        for move in all_moves:
            positions_seen+=1
            evaluation = evaluate_int(move, 2)
            next_force = check_for_force(move, 1)
            if next_force == []:
                no_more_akel = True

            if evaluation<minEval:
                    minEval = evaluation
                    best_move = move

        if no_more_akel:
            return minEval, best_move

    if False and akel_depth >= 5:
        return evaluate_int(board_layout, 2), board_layout

    if max_player:
        maxEval = -10000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            positions_seen+=1
            #draw_move(screen, move)
            evaluation = minimax_depth1(screen, False, move, akel_depth, alpha, beta, memo2)[0]

            if evaluation>maxEval:
                maxEval = evaluation
                best_move = move

            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break
    
        return maxEval, best_move
    else:
        minEval = 10000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:
            next_force = check_for_force(move, 1)
            strMove = str(move)
            if (next_force!=[] or force_list!=[]) and akel_depth<5:
                #draw_move(screen, move)
                if strMove in memo2.keys() and memo2[strMove][1]==akel_depth and memo2[strMove][2]==False:
                    evaluation = evaluation = memo2[strMove][0]
                else:
                    evaluation = minimax_depth1(screen, True, move, akel_depth+1, alpha, beta, memo2)[0]
                    memo2[strMove] = (evaluation, akel_depth, False)
            
            try:
                if evaluation<minEval:
                    minEval = evaluation
                    best_move = move

            except:
                pass

            beta = min(beta, minEval)
            if beta<= alpha:
                break

        return minEval, best_move

def minimax_depth1_black(screen, max_player, board_layout, akel_depth, alpha, beta, memo2):

    if (max_player == False) and (check_for_force(board_layout, 1)!=[]):

        all_moves, force_list = get_all_moves(board_layout, 1)
        no_more_akel = False
        maxEval = -10000
        best_move = None

        for move in all_moves:
            evaluation = evaluate_int(move, 1)
            next_force = check_for_force(move, 2)
            if next_force == []:
                no_more_akel = True

            if evaluation>maxEval:
                maxEval = evaluation
                best_move = move

        if no_more_akel:
            return maxEval, best_move

    if False and akel_depth >= 5:
        return evaluate_int(board_layout, 2), board_layout

    if max_player:
        minEval = 10000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:

            #draw_move(screen, move)
            evaluation = minimax_depth1_black(screen, False, move, akel_depth, alpha, beta, memo2)[0]

            if evaluation<minEval:
                minEval = evaluation
                best_move = move

            beta = min(beta, minEval)
            if beta<= alpha:
                break
    
        return minEval, best_move
    else:
        maxEval = -10000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            next_force = check_for_force(move, 2)
            strMove = str(move)
            if (next_force!=[] or force_list!=[]) and akel_depth<5:
                #draw_move(screen, move)
                if strMove in memo2.keys() and memo2[strMove][1]==akel_depth and memo2[strMove][2]==True:
                    evaluation = evaluation = memo2[strMove][0]
                else:
                    evaluation = minimax_depth1_black(screen, True, move, akel_depth+1, alpha, beta, memo2)[0]
                    memo2[strMove] = (evaluation, akel_depth, True)
            
            try:
                if evaluation>maxEval:
                    maxEval = evaluation
                    best_move = move

            except:
                pass

            alpha = max(alpha, maxEval)
            if beta<= alpha:
                break

        return maxEval, best_move

def move_piece(piece, move, board_layout, parent_list, color):
    row = piece[0]
    col = piece[1]

    r = move[0]
    c = move[1]

    value_length = 0

    if parent_list!=[] and parent_list!=None:
        for value in parent_list:
            if (r,c) == value[-1]:
                value_length = len(value)
                z=0
                for z in range(0, value_length):
                    if z+1 >= value_length:
                        break
                    old_row = value[z][0]
                    old_col = value[z][1]
                    new_row = value[z+1][0]
                    new_col = value[z+1][1]
                    board_layout = eat_piece_if_possible(board_layout, old_row, old_col, new_row, new_col, color)[0]
                #new
                break
        parent_list.remove(value)
        if value_length==0:
            return board_layout
    else:
        board_layout = eat_piece_if_possible(board_layout, row, col, r, c, color)[0]
        

    aux = board_layout[row][col]
    board_layout[row][col]=board_layout[r][c]
    board_layout[r][c]=aux

    #check if dama

    if r == 0 and color==2:
        board_layout[r][c]=4
    if r == 7 and color==1:
        board_layout[r][c]=3

    return board_layout

iter_count = 0
def get_all_moves(board_layout, color):
    global iter_count
    moves = []

    #create force list
    valid_moves = []
    parent_list = []
    force_list = []
    force_list = check_for_force(board_layout, color)

    if force_list!=[]:
        pieces = force_list
    else:
        pieces = get_all_pieces(board_layout, color)

    for piece in pieces:
        
        color2 = board_layout[piece[0]][piece[1]]
        
        
        if color2 == 1 or color2 == 2:

            valid_moves, eating_piece = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout, '')
            fff = (eating_piece!='')

            if fff:
                parent_list2 = []
                for move in valid_moves:
                    parent_list2.append( [(piece[0], piece[1]), move])
                
                parent_list = eat_max2_not_dama(piece[0], piece[1], board_layout, parent_list2, color2, '')
                aux_list = []
                if parent_list != [] and parent_list!=None:
                    for value in parent_list:
                        aux_list.append(value[-1])
                    valid_moves = aux_list
                    
        if color2 == 3 or color2 == 4:
            iter_count+=1
            valid_moves, eating_piece = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout, '')
            dama_nom = (eating_piece=='d')

            if dama_nom:
                parent_list2 = []
                for move in valid_moves:
                    parent_list2.append ( [(piece[0], piece[1]), move])
                parent_list = eat_max2(piece[0], piece[1], board_layout, parent_list2, color2, '')
                aux_list = []
                if parent_list != [] and parent_list!=None:
                    for value in parent_list:
                        aux_list.append(value[-1])
                    valid_moves = aux_list
        
        for move in valid_moves:
            iter_count+=1
            temp_board_layout = deepcopy2(board_layout)
            temp_board_layout = move_piece(piece, move, temp_board_layout, parent_list, color2)

            #test: checking multiple same move
            if parent_list!=[]:
                while check_end_of_parent_list(parent_list, move):
                    temp_board_layout2 = deepcopy2(board_layout)
                    temp_board_layout2 = move_piece(piece, move, temp_board_layout2, parent_list, color2)
                    moves.append(temp_board_layout2)

            if False:
                if color == 1:
                    Turn = 2
                else:
                    Turn = 1
                
                f_list = check_for_force(temp_board_layout, Turn)

                if f_list!=[]:
                    moves.append(temp_board_layout)
                else:
                    moves.insert(0, temp_board_layout)
            moves.append(temp_board_layout)

    return moves, force_list

def check_end_of_parent_list(parent_list, move):
    for l in parent_list:
        if l[-1] == move:
            return True
    return False

def deepcopy2(board_layout):
    new_board_layout = [row[:] for row in board_layout]

    #new_board_layout = [[0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    [0, 0, 0, 0, 0, 0 ,0, 0],
    #                    ]



    #for i in range(8):
        #for j in range(8):
            #new_board_layout[i][j] = board_layout[i][j]
    
    return new_board_layout

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

    eat_direction = ''

    if color == 1 or color == 2:
        sumRowOver2 = (row + r)//2
        sumColOver2 = (col+c)//2
        if c==col:
            if r>row:
                eat_direction = 'd'
            else:
                eat_direction = 'u'
        else:
            if c>col:
                eat_direction = 'r'
            else:
                eat_direction = 'l'

    if color == 1:
        if board_layout[sumRowOver2][col] == 2 or board_layout[sumRowOver2][col] == 4:
            board_layout[sumRowOver2][col]=0
            #eatSound.play()
            return board_layout, eat_direction
        elif board_layout[row][sumColOver2] == 2 or board_layout[row][sumColOver2] == 4:
            board_layout[row][sumColOver2]=0
            #eatSound.play()
            return board_layout, eat_direction
    elif color == 2:
        if board_layout[sumRowOver2][col] == 1 or board_layout[sumRowOver2][col] == 3:
            board_layout[sumRowOver2][col]=0
            #eatSound.play()
            return board_layout, eat_direction
        elif board_layout[row][sumColOver2] == 1 or board_layout[row][sumColOver2]==3 :
            board_layout[row][sumColOver2]=0
            #eatSound.play()
            return board_layout, eat_direction
    elif color ==3:
        if c==col:
            if r>row:
                big = r
                small = row
                eat_direction = 'd'
            else:
                big = row
                small = r
                eat_direction = 'u'
            for k in range(small,big):
                if board_layout[k][c] == 2 or board_layout[k][c] == 4:
                    board_layout[k][c]=0
                    #eatSound.play()
                    return board_layout, eat_direction
                    break

        elif row ==r:
            if c>col:
                big = c
                small = col
                eat_direction = 'r'
            else:
                big = col
                small = c
                eat_direction = 'l'
            for k in range(small,big):
                if board_layout[r][k] == 2 or board_layout[r][k] == 4:
                    board_layout[r][k]=0
                    #eatSound.play()
                    return board_layout, eat_direction
                    break
                    
    elif color == 4:
        if c==col:
            if r>row:
                big = r
                small = row
                eat_direction = 'd'
            else:
                big = row
                small = r
                eat_direction = 'u'
            for k in range(small,big):
                if board_layout[k][c] == 1 or board_layout[k][c] == 3:
                    board_layout[k][c]=0
                    #eatSound.play()
                    return board_layout, eat_direction
                    break

        elif row ==r:
            if c>col:
                big = c
                small = col
                eat_direction = 'r'
            else:
                big = col
                small = c
                eat_direction = 'l'
            for k in range(small,big):
                if board_layout[r][k] == 1 or board_layout[r][k] == 3:
                    board_layout[r][k]=0
                    #eatSound.play()
                    return board_layout, eat_direction
                    break

    return board_layout, eat_direction

def eat_max2_not_dama(row, col, board_layout, parent_list,  color, eat_direction):
    if parent_list!=[] and len(parent_list[0])>20:
        return parent_list
    

    valid_moves, eating_piece = get_valid_moves(row, col, color, 0, [], board_layout, eat_direction)

    fff = (eating_piece!='')

    if fff:
        new_list = []
        big_dama_list = []
        for move in valid_moves:
            
            if move[1]==col:
                if move[0]>row:
                    eat_direction = 'd'
                else:
                    eat_direction = 'u'
            else:
                if move[1]>col:
                    eat_direction = 'r'
                else:
                    eat_direction = 'l'

            
            eating_piece = get_valid_moves(move[0], move[1], color, 0, [], board_layout, eat_direction)[1]
            fff = (eating_piece!='')

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
                  
                if last[1]==pre_last[1]:
                    if last[0]>pre_last[0]:
                        eat_direction = 'd'
                    else:
                        eat_direction = 'u'
                elif pre_last[0] == last[0]:
                    if last[1]>pre_last[1]:
                        eat_direction = 'r'
                    else:
                        eat_direction = 'l'

                valid_moves2, eating_piece = get_valid_moves(last[0], last[1], color, 0, [], board_layout, eat_direction)
                fff = (eating_piece!='')

                if fff:
                    for move in valid_moves2:
                        aux_list2.append(value + [move])
            parent_list = aux_list2

            return parent_list
        else:
            maxp = []
            valid_moves =  new_list
            for move in valid_moves:

                new_board_layout = deepcopy2(board_layout)
                new_board_layout, eat_direction = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color)

                p2 = eat_max2_not_dama(move[0], move[1], board_layout, parent_list, color, eat_direction)
                if maxp==[] or len(p2[0]) > len(maxp[0]):
                    maxp = p2
            return maxp
    else:
        return []
        
def eat_max2(row, col, board_layout, parent_list,  color, eat_direction):
    
    valid_moves, eating_piece = get_valid_moves(row, col, color, 0, [], board_layout, eat_direction)

    dama_nom = (eating_piece=='d')

    if dama_nom:
        new_list = []
        big_dama_list = []

        for move in valid_moves:

            dama_nom = False
            
            if move[1]==col:
                if move[0]>row:
                    eat_direction = 'd'
                else:
                    eat_direction = 'u'
            else:
                if move[1]>col:
                    eat_direction = 'r'
                else:
                    eat_direction = 'l'
    
            eating_piece = get_valid_moves(move[0], move[1], color, 0, [], board_layout, eat_direction)[1]

            dama_nom = (eating_piece=='d')
    
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

                
                if last[1]==pre_last[1]:
                    if last[0]>pre_last[0]:
                        eat_direction = 'd'
                    else:
                        eat_direction = 'u'
                else:
                    if last[1]>pre_last[1]:
                        eat_direction = 'r'
                    else:
                        eat_direction = 'l'

                valid_moves2, eating_piece = get_valid_moves(last[0], last[1], color, 0, [], board_layout, eat_direction)

                dama_nom = (eating_piece=='d')
                
                if dama_nom:
                    for move in valid_moves2:
                        aux_list2.append(value + [move])
                parent_list = aux_list2

            return parent_list
        else:
            valid_moves =  new_list
            maxp=[]
            for move in valid_moves:

                new_board_layout = deepcopy2(board_layout)
                new_board_layout , eat_direction = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color)

                p2 = eat_max2(move[0], move[1], new_board_layout, parent_list, color, eat_direction)
                
                if maxp==[]:
                    maxp = p2
                else:
                    if len(p2[0])>len(maxp[0]):
                        maxp = p2
                    elif len(p2[0])==len(maxp[0]):
                        for li in p2:
                            if li not in maxp:
                                maxp.append(li)      
            return maxp
    else:
        return []

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

def readFromFile(board_layout):
    with open('minimaxResult.txt', 'r') as f:
        # Iterate over each line in the file
        for line in f:
            # Split the line into a list of integers
            row = list(map(int, line.split()))
            # Append the row to the board_layout array
            board_layout.append(row)
    return board_layout

def writeToFile(board_layout):
    with open('minimaxResult.txt', 'w') as f:
        # Iterate over each row in the board_layout array
        for row in board_layout:
            # Convert the row to a string of integers separated by spaces
            line = ' '.join(map(str, row))
            # Write the line to the file
            f.write(line + '\n')

#function to get board from sombolBoard and display it on the screen
def printBoardOnScreen(screen):
    board_layout = []
    with open('minimaxResult.txt', 'r') as f:
        # Iterate over each line in the file
        for line in f:
            # Split the line into a list of integers
            row = list(map(int, line.split()))
            # Append the row to the board_layout array
            board_layout.append(row)
    screen.fill((255, 255, 255))
    screen.blit(board.surf,(0, 0))
    for i in range(0, 8):
        for j in range(0, 8):
            if board_layout[i][j]==1:
                screen.blit(blackpiece.surf, (10+j*100, 10 + i*100))
            elif board_layout[i][j]==2:
                screen.blit(whitepiece.surf, (10+j*100, 10 + i*100))
            elif board_layout[i][j]==3:
                screen.blit(blackDama, (10+j*100, 10 + i*100))
            elif board_layout[i][j]==4:
                screen.blit(redDama, (10+j*100, 10 + i*100))
    pygame.display.flip()

def AI_VS_AI():
    printBoardOnScreen(screen)
    writeToFile(board.layout)
    delete_game_history("game_history.txt")
    turn = 2

    while True:

        if turn == 2:
            # get board from minimaxResult.txt 
            board.layout = readFromFile(board.layout)
            
            #execute sombol1.exe
            subprocess.run(['sombol1.exe'], check=True)

            # print the board on the screen
            printBoardOnScreen(screen)
            pygame.display.flip()

            # change the turn
            turn = 1
        else:
            # get board from minimaxResult.txt 
            board.layout = readFromFile(board.layout)
            
            #execute sombol2.exe
            subprocess.run(['sombol2.exe'], check=True)

            # print the board on the screen
            printBoardOnScreen(screen)
            pygame.display.flip()

            # change the turn
            turn = 2
        
        # check if the game is over
        if count_pieces(board.layout, 1) == 0 or count_pieces(board.layout, 2) == 0:
            exit()


#AI vs AI
# AI_VS_AI()
            
#CHANGE Starting turn and AI turn
turn = 2
AI_TURN = 1

# printBoardOnScreen(screen)
#time test 

start2 = time.time()
test = [
             [0, 0, 0, 0, 3, 0, 0, 0]
            ,[1, 1, 0, 0, 0, 1, 1, 1]
            ,[0, 0, 2, 0, 2, 1, 1, 0]
            ,[1, 1, 0, 2, 0, 1, 1, 2]
            ,[0, 0, 0, 4, 0, 2, 2, 0]
            ,[2, 2, 0, 2, 0, 2, 2, 2]
            ,[2, 2, 0, 2, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ]

test2 = [
             [0, 0, 0, 0, 3, 0, 0, 0]
            ,[0, 2, 0, 0, 0, 0, 0, 1]
            ,[0, 0, 2, 0, 2, 1, 2, 0]
            ,[1, 1, 0, 2, 0, 2, 0, 2]
            ,[0, 0, 0, 4, 0, 2, 2, 0]
            ,[2, 0, 0, 2, 0, 2, 2, 2]
            ,[2, 2, 0, 2, 0, 0, 0, 0]
            ,[0, 0, 0, 0, 0, 0, 0, 0]
            ]


test2 = []
sum = 0.1
moves2 = []
for i in range(1):
    # print(get_valid_moves_gpt(5, 0, 2, 0, [], test, ''))
    # deepcopy2(test)
    # get_all_moves(test, 1)
    #strmove = str(test)
    #move_piece((2, 0), (3, 0), test, [], 1)
    #eat_piece_if_possible(test, 7, 0, 3, 0, 3)
    #check_for_force(test, 1)
    #check_for_piece_akel(3, 0, 1, test)
    #moves2.append(test)
    #evaluate_int(test, 2)
    # print(eat_max2(0, 4, test, [[(0, 4), (4, 3)], [(0, 4), (4, 4)], [(0, 4), (4, 5)], [(0, 4), (4, 6)]], 3, ''))
    pass
end2 = time.time()

# print("The elapsed time python: ", end2-start2)

#main run loop
force_list=np.array([])

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
pygame.display.flip()

#display waiting for connection
#color, board, id = server.init_game("INIT")
#if color == 1: opponent = 2 else: opponent = 1

#inloop:
#if turn == opponent:
#   board = server.get_move()
#else:
#   #play your move
#   server.send_move(board, id)
#turn switch
#

#CHANGEDDDD
CPPFILE = 'dama_bitTest.cpp'
# CPPFILE = 'dama22.cpp'
# Compile the C++ file

# delete game history of previous game
delete_game_history("game_history.txt")
# compile_command = ['g++','-Ofast','-faggressive-loop-optimizations', '-fstack-arrays', CPPFILE, '-o', 'sombol1.0']
# subprocess.run(compile_command, check=True)

try:
    # Call 'make' command
    compile_command = ['make','-j']
    subprocess.run(compile_command, check=True)
except subprocess.CalledProcessError as e:
    print(f"Error during make: {e}")

# write initial board to minimaxResult.txt
writeToFile(board.layout)

while running:
    clock.tick(30)
    screen.fill((255, 255, 255))
    screen.blit(board.surf,(0, 0))
    
    #black ai move 
    if turn == AI_TURN:
        current_board_layout = deepcopy(board.layout)
        
        # Execute the compiled binary

        #check if os is Linux or Windows
        if platform.system() == 'Windows':
            execute_command = ['sombol1.exe']
        else:
            execute_command = ['./sombol1']
            
        subprocess.run(execute_command)

        board.layout = []
        with open('minimaxResult.txt', 'r') as file:
            for line in file:
                row = [int(num) for num in line.strip().split()]
                board.layout.append(row)
        # current_board_layout = deepcopy(board.layout)
        # calculations = 0
        # positions_seen = 0

        # try:
        #     game_history[str(current_board_layout)]+=1
        # except:
        #     game_history[str(current_board_layout)]=1


        # ai_played = False
        # #1 forced move
        # force_list1 = check_for_force(board.layout, 1)

        # if len(force_list1) == 1:
        #     forced_piece = force_list1[0]
        #     valid_moves2, parent_list3 = get_valid_moves2(board.layout, forced_piece, board.layout[forced_piece[0]][forced_piece[1]])

        #     if len(valid_moves2) == 1:
        #         forced_move = valid_moves2[0]
        #         new_board_layout = board.layout.copy()
        #         move_piece(forced_piece, forced_move, new_board_layout, parent_list3, board.layout[forced_piece[0]][forced_piece[1]])
        #         board.layout = deepcopy2(new_board_layout)
        #         turn = 2
        #         time.sleep(0.5)
        #         ai_played = True
        #         value = 690

        # start = time.time()

        # if not ai_played:

        #     if (count_pieces(current_board_layout, 1) + count_pieces(current_board_layout, 2)<7):
        #         value, new_board_layout = deepcopy(minimax_pro2(6, True, current_board_layout, float('-inf'), float('inf'), 0, True, True, {}))
        #     else:
        #         #value, new_board_layout = deepcopy(minimax(5, True, current_board_layout, float('-inf'), float('inf'), 5, 50000))
        #         #value, new_board_layout = deepcopy(classic_minimax(6, True, current_board_layout, float('-inf'), float('inf')))
        #         value, new_board_layout = deepcopy(minimax_pro2(5, True, current_board_layout, float('-inf'), float('inf'), 0, True, True, {}))
            
        #     if False:
        #         value2 = -1000000
        #         current_board_layout2 = deepcopy(board.layout)
        #         value2, new_board_layout2 = deepcopy(minimax_depth1_black(screen, False, current_board_layout2, 0, float('-inf'), float('inf'), {}))
        #         print("value2: ", value2)

                
        #         if value2>value and abs(value2-value)>99:
        #             value = value2
        #             new_board_layout = new_board_layout2
        #     board.layout = deepcopy(new_board_layout)

        clickSound.play()
        # end = time.time()
        # try:
        #     game_history[str(new_board_layout)]+=1
        # except:
        #     game_history[str(new_board_layout)]=1

        try:
             row_save, col_save = check_for_move(current_board_layout, board.layout, 1)
        except:
            pass

        # print(f"evaluation: {value/100 : .2f}")
        # print(f"positions seen: {positions_seen}")
        # print(f"time: {end - start : .2f} sec")
        # if end-start != 0:
        #     print(f"positions/sec: {positions_seen/(end-start) : .0f}")
        # print("calculations: ", calculations)

        # print()
        # print()
        turn = 3 - turn
        # flip turn
        # turn -= 1
        # if(turn == 0):
        #     turn = 2


    
    #red ai move
    if False and turn == 2:

        current_board_layout = deepcopy(board.layout)
        
        # Execute the compiled binary

        #check if os is Linux or Windows
        if platform.system() == 'Windows':
            execute_command = ['sombol2.exe']
        else:
            execute_command = ['./sombol2']
            
        subprocess.run(execute_command)

        board.layout = []
        with open('minimaxResult.txt', 'r') as file:
            for line in file:
                row = [int(num) for num in line.strip().split()]
                board.layout.append(row)
        
        turn = 3 - turn
        printBoardOnScreen(screen)
        pygame.display.flip()

        # current_board_layout = deepcopy(board.layout)
        # calculations = 0
        # positions_seen = 0

        # try:
        #     game_history[str(current_board_layout)]+=1
        # except:
        #     game_history[str(current_board_layout)]=1
        
        # start = time.time()

        # if False and not dama_in_board(current_board_layout) and (count_pieces(current_board_layout, 1) + count_pieces(current_board_layout, 2)<10):
        #     print("hi")
        #     value, new_board_layout = deepcopy(minimax(6, False, current_board_layout, float('-inf'), float('inf'), 6, 0))
        # else:
        #     #value, new_board_layout = deepcopy(classic_minimax(5, False, current_board_layout, float('-inf'), float('inf')))
        #     #value, new_board_layout = minimax(4, False, current_board_layout, float('-inf'), float('inf'), 4, 50000)
        #     #value, new_board_layout = minimax_depth1(screen, False, current_board_layout, 0, float('-inf'), float('inf'), {})
        #     value, new_board_layout = deepcopy(minimax_pro2(5, False, current_board_layout, float('-inf'), float('inf'), 0, False, False, {}))

        # if False:
        #     value2, new_board_layout2 = minimax_depth1(screen, False, current_board_layout, 0, float('-inf'), float('inf'), {})

        #     if value2<value and abs(value - value2)>99:
        #         value = value2
        #         new_board_layout = new_board_layout2
        #         print("hi")

        # end = time.time()
        # try:
        #     game_history[str(new_board_layout)]+=1
        # except:
        #     game_history[str(new_board_layout)]=0

        # try:
        #     row_save, col_save = check_for_move(board.layout, new_board_layout, 2)
        # except:
        #     pass
        clickSound.play()

        #test time

        # print("calculations: ", calculations)
        # print("evaluation: ", value)
        # print("positions seen: ", positions_seen)
        # print("time: ", end - start)
        # print()
        # print()
        # board.layout = deepcopy(new_board_layout)
        
        turn = 3 - turn
        continue
        

    for  event in pygame.event.get():
        if event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False
        elif event.type == pygame.QUIT:
            running = False

        if event.type == pygame.MOUSEBUTTONDOWN and selected == True:
            pos = pygame.mouse.get_pos()
            c = pos[0]//100
            r = pos[1]//100

            aux_list = []
            savedPiece = board.layout[row][col]
            
            if (r, c) in valid_moves:
                clickSound.play()
                
                row_save = row
                col_save = col
                if parent_list!=[] and parent_list!=None:
                    for value in parent_list:
                        if (r,c) in value:
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
                                
                                eating_direction = eat_piece_if_possible(board.layout, old_row, old_col, new_row, new_col, color)[1]
                                
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
                                
                                if value[z+1] == (r, c):
                                    break
                                
                                pygame.display.update()
                                time.sleep(0.3)
                            break
                else:
                    eating_direction = eat_piece_if_possible(board.layout, row, col, r, c, color)[1]
                
                board.layout[r][c] = savedPiece
                board.layout[row][col] = 0

                #check if dama
                if r == 0 and color==2:
                    board.layout[r][c]=4
                if r == 7 and color==1:
                    board.layout[r][c]=3
                selected=False
                valid_moves=[]
                #place board in minixResult.txt
                with open('minimaxResult.txt', 'w') as file:
                    for row in board.layout:
                        line = ' '.join(map(str, row)) + '\n'
                        file.write(line)

                played_extra_turn = False
                if (r,c) in valid_moves_end or parent_list==[]:
                    if turn==1:
                        turn = 2
                    else:
                        turn = 1
                else:
                    played_extra_turn = True
                
            else:
                if event.type == pygame.MOUSEBUTTONDOWN:
                    selected = False
                    #valid_moves=[]
        
        if event.type == pygame.MOUSEBUTTONDOWN and selected == False:
            surf4=None
            valid_moves=[]
            valid_moves_end = []
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
                            valid_moves = get_valid_moves(row, col, board.layout[row][col], 0, [], board.layout, '')[0]
                            selected=True
                    else:
                        clickSound.play()
                        valid_moves = get_valid_moves(row, col, board.layout[row][col], 0, [], board.layout, '')[0]
                        selected=True
                    #valid_moves = get_valid_moves(row, col, board.layout[row][col], 0, [])
                    
                    #akel aal aktar
                    parent_list2 = []
                    for move in valid_moves:
                        parent_list2.append ( [(row, col), move])

                    parent_list = []

                    try:
                        if not played_extra_turn:
                            eating_direction = ''
                    except:
                        eating_direction = ''
                    

                    if color == 3 or color == 4:
                        temp_board_layout = deepcopy(board.layout)
                        parent_list = eat_max2(row, col, temp_board_layout, parent_list2, board.layout[row][col], eating_direction)
                    elif color == 1 or color == 2:
                        parent_list = eat_max2_not_dama(row, col, board.layout, parent_list2, board.layout[row][col], eating_direction)
                    aux_list = []

                    
                    if parent_list != [] and parent_list!=None:
                        for value in parent_list:
                            aux_list.append(value[-1])
                        valid_moves_end = deepcopy(aux_list) 

                    # if two akels exist
                    if len(valid_moves_end) != len(set(valid_moves_end)):
                        aux_list = []
                        if parent_list != [] and parent_list!=None:
                            for value in parent_list:
                                for move in value:
                                    if move != value[0]:
                                        aux_list.append(move)
                            valid_moves = deepcopy(aux_list)
                    else:
                        if parent_list != [] and parent_list!=None:
                            valid_moves = valid_moves_end
                    
                else:
                    selected = False
            #continue

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
    
    for val1, val2 in force_list:
        if surf4 != None:
            screen.blit(surf4, (val2*100, val1*100))

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
    if count_pieces(board.layout, 1) == 1 and count_pieces(board.layout, 2) == 1 and check_for_force(board.layout, 1)  == [] and check_for_force(board.layout, 2)  == []:
        print("draw")
        time.sleep(5)
        break