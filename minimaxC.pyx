import time
from random import randrange
from copy import deepcopy

def get_valid_moves(int row,int col,int color,int streak,list valid_moves, list board_layout, str eat_direction):

    cdef int ate_up = (eat_direction == 'up')
    cdef int ate_down = (eat_direction == 'down')
    cdef int ate_left = (eat_direction == 'left')
    cdef int ate_right = (eat_direction == 'right')

    cdef str eating_piece = ''
    cdef int i, j
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
                        eating_piece = 'dama'
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
            eating_piece = 'normal'
            

        if col+2<8 and (board_layout[row][col+1]==2 or board_layout[row][col+1]==4) and board_layout[row][col+2]==0 and streak>=0 and not ate_left:
            streak+=1
            valid_moves.append((row, col+2))
            eating_piece = 'normal'
            

        if col-2>=0 and (board_layout[row][col-1]==2 or board_layout[row][col-1]==4) and board_layout[row][col-2]==0 and streak>=0 and not ate_right:
            streak+=1
            valid_moves.append((row, col-2))
            eating_piece = 'normal'
            
        

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
            eating_piece = 'normal'

        if col+2<8 and (board_layout[row][col+1]==1 or board_layout[row][col+1]==3) and board_layout[row][col+2]==0 and streak>=0 and not ate_left:
            streak+=1
            valid_moves.append((row, col+2))
            eating_piece = 'normal'

        if col-2>=0 and (board_layout[row][col-1]==1 or board_layout[row][col-1]==3) and board_layout[row][col-2]==0 and streak>=0 and not ate_right:
            streak+=1
            valid_moves.append((row, col-2))
            eating_piece = 'normal'

        if row-1>=0 and board_layout[row-1][col]==0 and streak==0:
            valid_moves.append((row-1, col))
        if col+1<8 and board_layout[row][col+1]==0 and streak ==0:
            valid_moves.append((row, col+1))
        if col-1>=0 and board_layout[row][col-1]==0 and streak ==0:
            valid_moves.append((row, col-1))
    
    return valid_moves, eating_piece


def eat_max2_not_dama(int row,int col,list board_layout,list parent_list,int color,str eat_direction):
    if parent_list!=[] and len(parent_list[0])>20:
        return parent_list
    
    cdef list valid_moves
    cdef str eating_piece
    cdef list new_list
    cdef list big_dama_list
    cdef tuple move
    cdef tuple parent
    cdef list value
    cdef list aux_list2
    cdef tuple last
    cdef tuple pre_last
    cdef list valid_moves2
    cdef list maxp
    cdef list new_board_layout
    cdef list p2

    valid_moves, eating_piece = get_valid_moves(row, col, color, 0, [], board_layout, eat_direction)

    cdef int fff = (eating_piece!='')

    if fff:
        
        new_list = []
        big_dama_list = []
        for move in valid_moves:
            
            if move[1]==col:
                if move[0]>row:
                    eat_direction = 'down'
                else:
                    eat_direction = 'up'
            else:
                if move[1]>col:
                    eat_direction = 'right'
                else:
                    eat_direction = 'left'

            
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
                        eat_direction = 'down'
                    else:
                        eat_direction = 'up'
                elif pre_last[0] == last[0]:
                    if last[1]>pre_last[1]:
                        eat_direction = 'right'
                    else:
                        eat_direction = 'left'
                
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

                new_board_layout = deepcopy(board_layout)
                new_board_layout, eat_direction = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color)

                p2 = eat_max2_not_dama(move[0], move[1], board_layout, parent_list, color, eat_direction)
                if maxp==[] or len(p2[0]) > len(maxp[0]):
                    maxp = p2
            return maxp
    else:
        return []


def eat_max2(int row,int col,list board_layout,list parent_list,int  color,str eat_direction):
    
    cdef list valid_moves
    cdef str eating_piece
    cdef list new_list
    cdef list big_dama_list
    cdef tuple move
    cdef tuple parent
    cdef list value
    cdef list aux_list2
    cdef tuple last
    cdef tuple pre_last
    cdef list valid_moves2
    cdef list maxp
    cdef list new_board_layout
    cdef list p2

    valid_moves, eating_piece = get_valid_moves(row, col, color, 0, [], board_layout, eat_direction)

    cdef int dama_nom = (eating_piece=='dama')

    if dama_nom:
        new_list = []
        big_dama_list = []
        for move in valid_moves:

            dama_nom = False
            
            if move[1]==col:
                if move[0]>row:
                    eat_direction = 'down'
                else:
                    eat_direction = 'up'
            else:
                if move[1]>col:
                    eat_direction = 'right'
                else:
                    eat_direction = 'left'
    
            eating_piece = get_valid_moves(move[0], move[1], color, 0, [], board_layout, eat_direction)[1]

            dama_nom = (eating_piece=='dama')
    
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
                        eat_direction = 'down'
                    else:
                        eat_direction = 'up'
                else:
                    if last[1]>pre_last[1]:
                        eat_direction = 'right'
                    else:
                        eat_direction = 'left'

                valid_moves2, eating_piece = get_valid_moves(last[0], last[1], color, 0, [], board_layout, eat_direction)

                dama_nom = (eating_piece=='dama')
                
                if dama_nom:
                    for move in valid_moves2:
                        aux_list2.append(value + [move])
                parent_list = aux_list2

            return parent_list
        else:
            valid_moves =  new_list
            maxp=[]
            for move in valid_moves:

                new_board_layout = deepcopy(board_layout)
                new_board_layout , eat_direction = eat_piece_if_possible(new_board_layout, row, col, move[0], move[1], color)

                p2 = eat_max2(move[0], move[1], new_board_layout, parent_list, color, eat_direction)
                if maxp==[] or len(p2[0]) > len(maxp[0]):
                    maxp = p2
            return maxp
    else:
        return []

def check_for_force(list board_layout,int turn):

    cdef list aktar_list = []
    cdef int aktar_flag = False
    cdef list force_list = []
    cdef int maxLength = 3
    cdef int row2, col2
    cdef int color
    cdef int fff
    cdef list parent_list2
    cdef list valid_moves
    cdef str eating_piece
    cdef int dama_nom
    cdef tuple move
    cdef list t_board_layout
    cdef int listSize
    cdef list value
    
    for row2 in range(8):
        for col2 in range(8):
            color = board_layout[row2][col2]
            if ((board_layout[row2][col2]==2 or board_layout[row2][col2]==4) and turn==2) or ((board_layout[row2][col2]==1 or board_layout[row2][col2]==3) and turn==1):
                fff = False
                parent_list2 = []

                valid_moves, eating_piece = get_valid_moves(row2, col2, board_layout[row2][col2], 0, [], board_layout, '')
                
                dama_nom = (eating_piece=='dama')
                fff = (eating_piece != '')


                if fff==True:
                    parent_list2 = []
                    
                    for move in valid_moves:
                        parent_list2.append ( [(row2, col2), move])

                    if color == 1 or color == 2:
                        
                        t_board_layout = deepcopy(board_layout)
                        parent_list = eat_max2_not_dama(row2, col2, t_board_layout, parent_list2, board_layout[row2][col2], '')
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
                        parent_list = eat_max2(row2, col2, t_board_layout, parent_list2, board_layout[row2][col2], '')
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
        
    return force_list

##############################################################ai###############################################

def get_all_pieces(list board_layout,int color):
    cdef list pieces = []
    cdef int i, j
    for i in range(0, 8):
        for j in range(0, 8):
            if (color == 1 and (board_layout[i][j] == 1 or board_layout[i][j] == 3)) or (color ==2 and (board_layout[i][j] == 2 or board_layout[i][j] == 4)):
                pieces.append((i, j))
    return pieces

def passage_is_clear(list board_layout,int row,int col,int turn):
    cdef int j
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

def get_valid_moves2(list board_layout,tuple piece,int color2):

    cdef list parent_list2 = []
    cdef list parent_list = []
    cdef list valid_moves
    cdef str eating_piece
    cdef int fff
    cdef tuple move
    cdef list temporary_board_layout
    cdef list aux_list

    valid_moves, eating_piece = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout, '')

    fff = (eating_piece!='')

    if fff:
        
        for move in valid_moves:
            parent_list2.append ( [(piece[0], piece[1]), move])
        
        if color2 == 1 or color2 == 2:
            temporary_board_layout = deepcopy(board_layout)
            parent_list = eat_max2_not_dama(piece[0], piece[1], temporary_board_layout, parent_list2, color2, '')
            aux_list = []
            if parent_list != [] and parent_list!=None:
                for value in parent_list:
                    aux_list.append(value[-1])
                valid_moves = aux_list

        elif color2==3 or color2 == 4:
            temporary_board_layout = deepcopy(board_layout)
            parent_list = eat_max2(piece[0], piece[1], temporary_board_layout, parent_list2, color2, '')
            aux_list = []
            if parent_list != [] and parent_list!=None:
                for value in parent_list:
                    aux_list.append(value[-1])
                valid_moves = aux_list

    return valid_moves, parent_list

def evaluate(list board_layout, int turn, dict game_history):

    cdef double sum = 0.0
    cdef int balance_black = 0
    cdef int balance_red = 0
    cdef int all_pieces = 0
    cdef int red_pieces = 0
    cdef int black_pieces = 0
    cdef int i, j
    cdef list force_list1
    cdef tuple forced_piece
    cdef list valid_moves, parent_list
    cdef tuple forced_move
    cdef list temp_board_layout

    #draw
    if str(board_layout) in game_history.keys():
        if game_history[str(board_layout)]>=2:
            return 0
    
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

            if board_layout[i][j] == 1:
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

            elif board_layout[i][j] == 2:
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

            elif board_layout[i][j] == 3:
                black_pieces+=1
                sum+=3.5
            elif board_layout[i][j] == 4:
                red_pieces+=1
                sum-=3.5

    sum = sum - abs(balance_black)/20 + abs(balance_red)/20

    all_pieces = black_pieces + red_pieces

    if red_pieces>black_pieces:
        sum += all_pieces*0.005
    elif black_pieces>red_pieces:
        sum -= all_pieces*0.005

    if black_pieces==1 and red_pieces==1:
        sum = 0
      
    return sum

def draw(dict game_history,list board):
    try:
        if game_history[str(board)]>=2:
            return True
        else:
            return False
    except:
        return False

def minimax(int depth,int max_player,list board_layout,float alpha,float beta,int max_depth,int max_calculations, dict game_history, int calculations):

    cdef float maxEval = -100000
    cdef list best_move = None
    cdef list all_moves
    cdef list force_list
    cdef list move
    cdef float evaluation
    cdef str strMove
    cdef int drawFlag
    cdef int inDict
    cdef list next_force
    cdef list board
    cdef float minEval

    if depth <= 0:
        if max_player:
            turn = 1
        else:
            turn = 2

        return evaluate(board_layout, turn, game_history), board_layout

    if max_player:
        
        all_moves, force_list = get_all_moves(board_layout, 1)

        for move in all_moves:
            if draw(game_history, move):
                evaluation = 0
            
            next_force = check_for_force(move, 2)

            if next_force!=[] and calculations < max_calculations:
                calculations+=1
                
                evaluation, board = minimax(depth, False, move, alpha, beta, max_depth, max_calculations, game_history, calculations)
                if draw(game_history, board):
                    depth = 0
                    evaluation = 0
            else:
                evaluation, board = minimax(depth-1, False, move, alpha, beta, max_depth, max_calculations, game_history, calculations)

            #display(move)
            if evaluation >= maxEval:
                maxEval = evaluation
                best_move = move

            if depth<max_depth:
                alpha = max(alpha, maxEval)
                if beta<= alpha:
                    break
        return maxEval, best_move
    else:
        minEval = 100000
        best_move = None
        all_moves, force_list = get_all_moves(board_layout, 2)

        for move in all_moves:

            if draw(game_history, move):
                evaluation = 0
            
            next_force = check_for_force(move, 1)
            if next_force != [] and calculations < max_calculations:
                calculations+=1
                evaluation, board = minimax( depth, True, move, alpha, beta, max_depth, max_calculations, game_history, calculations)
            else:
                evaluation, board = minimax( depth-1, True, move, alpha, beta, max_depth, max_calculations, game_history, calculations)

            if evaluation <= minEval:
                minEval = evaluation
                best_move = move
            
            if depth<max_depth:
                beta = min(beta, minEval)
                if beta<= alpha:
                    break
        return minEval, best_move

def eat_piece_if_possible(list board_layout,int row,int col,int r,int c,int color):

    cdef str eat_direction = ''
    cdef int big
    cdef int small
    cdef int k

    if color == 1 or color == 2:
        if c==col:
            if r>row:
                eat_direction = 'down'
            else:
                eat_direction = 'up'
        else:
            if c>col:
                eat_direction = 'right'
            else:
                eat_direction = 'left'

    if color == 1:
        if board_layout[(row + r)//2][col] == 2 or board_layout[(row + r)//2][col] == 4:
            board_layout[(row + r)//2][col]=0
            #eatSound.play()
        elif board_layout[row][(col+c)//2] == 2 or board_layout[row][(col+c)//2] == 4:
            board_layout[row][(col+c)//2]=0
            #eatSound.play()
    elif color == 2:
        if board_layout[(row + r)//2][col] == 1 or board_layout[(row + r)//2][col] == 3:
            board_layout[(row + r)//2][col]=0
            #eatSound.play()
        elif board_layout[row][(col+c)//2] == 1 or board_layout[row][(col+c)//2]==3 :
            board_layout[row][(col+c)//2]=0
            #eatSound.play()
    elif color ==3:
        if c==col:
            if r>row:
                big = r
                small = row
                eat_direction = 'down'
            else:
                big = row
                small = r
                eat_direction = 'up'
            for k in range(small,big):
                if board_layout[k][c] == 2 or board_layout[k][c] == 4:
                    board_layout[k][c]=0
                    #eatSound.play()

        elif row ==r:
            if c>col:
                big = c
                small = col
                eat_direction = 'right'
            else:
                big = col
                small = c
                eat_direction = 'left'
            
            for k in range(small,big):
                if board_layout[r][k] == 2 or board_layout[r][k] == 4:
                    board_layout[r][k]=0
                    #eatSound.play()
                    
    elif color == 4:
        if c==col:
            if r>row:
                big = r
                small = row
                eat_direction = 'down'
            else:
                big = row
                small = r
                eat_direction = 'up'
            for k in range(small,big):
                if board_layout[k][c] == 1 or board_layout[k][c] == 3:
                    board_layout[k][c]=0
                    #eatSound.play()

        elif row ==r:
            if c>col:
                big = c
                small = col
                eat_direction = 'right'
            else:
                big = col
                small = c
                eat_direction = 'left'
            for k in range(small,big):
                if board_layout[r][k] == 1 or board_layout[r][k] == 3:
                    board_layout[r][k]=0
                    #eatSound.play()

    return board_layout, eat_direction

def move_piece(tuple piece,tuple move,list board_layout,list parent_list,int color):
    cdef int row = piece[0]
    cdef int col = piece[1]
    cdef int r = move[0]
    cdef int c = move[1]
    cdef list value
    cdef int z=0
    cdef int old_row
    cdef int old_col
    cdef int new_row
    cdef int new_col
    cdef int aux

    if parent_list!=[] and parent_list!=None:
        
        for value in parent_list:
            if (r,c) == value[-1]:
                
                for z in range(0, len(value)):
                    if z+1 >= len(value):
                        break
                    old_row = value[z][0]
                    old_col = value[z][1]
                    new_row = value[z+1][0]
                    new_col = value[z+1][1]                                
                    board_layout = eat_piece_if_possible(board_layout, old_row, old_col, new_row, new_col, color)[0]
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


def get_all_moves(list board_layout,int color):
    cdef list moves = []

    #create force list
    cdef list valid_moves = []
    cdef list pieces = get_all_pieces(board_layout, color)
    cdef list force_list = []
    force_list = check_for_force(board_layout, color)
    cdef int color2
    cdef list parent_list2
    cdef list parent_list
    cdef str eating_piece
    cdef int fff
    cdef tuple move
    cdef list temporary_board_layout
    cdef list aux_list
    cdef list value
    cdef int dama_nom
    cdef list temp_board_layout

    if force_list!=[]:
        pieces = force_list
    for piece in pieces:
        color2 = board_layout[piece[0]][piece[1]]
        parent_list2 = []
        parent_list = []
        if color2 == 1 or color2 == 2:
            valid_moves, eating_piece = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout, '')
            fff = (eating_piece!='')

            if fff:
                for move in valid_moves:
                    parent_list2.append ( [(piece[0], piece[1]), move])
                temporary_board_layout = deepcopy(board_layout)
                parent_list = eat_max2_not_dama(piece[0], piece[1], temporary_board_layout, parent_list2, color2, '')
                aux_list = []
                if parent_list != [] and parent_list!=None:
                    for value in parent_list:
                        aux_list.append(value[-1])
                    valid_moves = aux_list
                    
        if color2 == 3 or color2 == 4:

            valid_moves, eating_piece = get_valid_moves(piece[0], piece[1], color2, 0, [], board_layout, '')
            dama_nom = (eating_piece=='dama')

            if dama_nom:
                for move in valid_moves:
                    parent_list2.append ( [(piece[0], piece[1]), move])
                temporary_board_layout = deepcopy(board_layout)
                parent_list = eat_max2(piece[0], piece[1], temporary_board_layout, parent_list2, color2, '')
                aux_list = []
                if parent_list != [] and parent_list!=None:
                    for value in parent_list:
                        aux_list.append(value[-1])
                    valid_moves = aux_list
        for move in valid_moves:
            temp_board_layout = deepcopy(board_layout)
            temp_board_layout = deepcopy(move_piece(piece, move, temp_board_layout, parent_list, color2))
            
            moves.append(temp_board_layout)

    return moves, force_list

