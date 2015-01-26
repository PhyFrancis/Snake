#!/usr/bin/python2
import sys

maxxsize=40
maxysize=40

def ifin(x,array):
    for i in array:
        if x==i:
            return True    
    return False

def ifinboard(x,boardb):
    if boardb[x[0]][x[1]] == 1:
        return True
    else:
        return False


def move(array,option):
    posi=[]
    posi.append(array[0])
    posi.append(array[1])
    if option==0:
        posi[0]-=1
    elif option==1:
        posi[1]-=1
    elif option==2:
        posi[0]+=1
    elif option==3:
        posi[1]+=1
    return posi    


def iflive(onemore,board):
    if onemore[0]>=maxxsize: return False
    if onemore[1]>=maxysize: return False
    if onemore[0]<0: return False
    if onemore[1]<0: return False
    if board[onemore[0]][onemore[1]] != 0: return False
    return True


def alternative(position,board):  # all possible method
    method=[]
    for i in range(4):
        onemore=move(position,i)
        if iflive(onemore,board):
            method.append(i)
    return method        

def alternative1(position,board,board1):
    method=[]
    for i in range(4):
        onemore=move(position,i)
        if iflive(onemore,board) and iflive(onemore,board1):
            method.append(i)
    return method        


def transfer(boardarray):   # transfer the board into normal form
    board=[[0 for j in xrange(maxysize)] for i in xrange(maxxsize)]
    for k in boardarray:
        board[k[0]][k[1]]=1
    return board

def printboard(boarda):   # print out board in normal form 
    for i in xrange(maxxsize):
        line=''
        for j in xrange(maxysize):
            line += str(boarda[i][j])
        print line

def diagnose(location,point1,point2):  # get the square point by 3 of them
    another=[0,0]
    for i in xrange(2):
        another[i]=point1[i]+point2[i]-location[i]
    return another


def check(board,point):  #check the connected area by point
    queue=[]
    space=[[0 for j in xrange(maxysize)] for i in xrange(maxxsize)]
    size = 0
    queue.append(point)
    if ifinboard(point,board):
        return []
    while len(queue) != 0:
        position=queue[0]
        queue=queue[1:]
        size += 1
        method=alternative1(position,board,space)
        for i in method:
            queue.append(move(position,i))
            space[move(position,i)[0]][move(position,i)[1]]=1
    return size, space


