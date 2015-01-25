#!/usr/bin/python2
import sys
import string
import random
import time
import getopt

import definition
from definition import ifin, move,iflive,alternative,transfer,printboard,diagnose,check,ifinboard


f=sys.stderr  # for test and report error
count=0
true=0
snake1=[]

OUT = False
mod ="straight_rand_20"
god = False

ifsamearea = True

try:
    opts, args = getopt.getopt(sys.argv[1:], "dm:x:y:g", ["debug", "mod="])
except getopt.GetoptError, err:
    # print help information and exit:
    print str(err) # will print something like "option -a not recognized"
    sys.exit(2)


for o,a in opts:
    if o in ("-d", "--debug"):
        OUT=True
    elif o in("-m", "--mod"):
        mod = a
    elif o in("-x"):
        definition.maxxsize = int(a)
    elif o in("-y"):
        definition.maxysize = int(a)
    elif o in ("-g"):
        god = True
maxxsize=definition.maxxsize
maxysize=definition.maxysize

board=[[0 for j in xrange(maxysize)] for i in xrange(maxxsize)]
#print maxxsize, maxysize
inputaxle = sys.stdin.readline()
while inputaxle[0] == '#' :
    inputaxle = sys.stdin.readline()
initial_state = map(int, inputaxle.split())
axle = []
position = []
snake1=[initial_state]
board[initial_state[0]][initial_state[1]]=1

last_method = 1

space=[[0 for j in xrange(maxysize)] for i in xrange(maxxsize)]
def printboardf(boarda):
    for i in xrange(maxxsize):
        line=''
        for j in xrange(maxysize):
            line += str(boarda[i][j])
        f.write( line+'\n')

def distance(point1,point2) :
    return abs(point1[0]-point2[0])+abs(point1[1]-point2[1])


def avoidhead(array,axle,position):
    if distance(axle,position) == 2 :
        if abs(axle[0]-position[0]) == 1 and abs(axle[1]-position[1]) == 1 :
            if OUT: f.write("diagnose\n")
            newpoint1 = [axle[0],position[1]]
            newpoint2 = [position[0],axle[1]]
            onlychoice = 4
            for i in array:
                if move(position,i) != newpoint1 and move(position,i) != newpoint2:
                    onlychoice = i
            if onlychoice == 4 :
                return array
            else: 
                return [ onlychoice ]
        elif abs(axle[0] - position[0]) == 0 :
            newpoint = [axle[0] , (axle[1] + position[1])/2]
            onlychoice = 4 
            for i in array :
                if move(position,i) != newpoint :
                    onlychoice = i
            if onlychoice ==4:
                return array
            else:
                return [onlychoice ] 
        else :
            newpoint = [(axle[0] + position[0])/2 , axle[1]]
            onlychoice = 4 
            for i in array :
                if move(position,i) != newpoint :
                    onlychoice = i
            if onlychoice ==4:
                return array
            else:
                return [onlychoice ] 
    else: 
        return array


def difai(array,last_method,mod):
    array1 = avoidhead(array,axle,position)
    if ifsamearea == False and not god :
        if (last_method - 1) % 4 in array1 :
            return (last_method-1) % 4
        elif last_method in array1 :
            return last_method
        elif (last_method + 1) % 4 in array1 :
            return (last_method + 1) % 4
    else:
        if mod == "rand":
            return random.choice(array1)
        elif len(array1) == 0: return []
        elif mod == "0":
            return array1[0]
        elif mod == "1":
            return array1[-1]
        elif mod == "straight_0":
            if last_method in array1:
                return last_method
            else:
                return array1[0]
        elif mod == "straight_1":
            if last_method in array1:
                return last_method
            else:
                return array1[-1]
        elif mod == "rand_0":
            if random.randint(0,20)!= 0:
                return array1[0]
            else:
                return random.choice(array1)
 
        elif mod == "rand_1":
            if random.randint(0,20)!= 0:
                return array1[-1]
            else:
                return random.choice(array1)


        elif mod == "straight_rand_30":
            if last_method in array1 and random.randint(0,30)!=0:
                return last_method
            else:
                return random.choice(array1)

        elif mod == "straight_rand_20":
            if last_method in array1 and random.randint(0,20)!=0:
                return last_method
            else:
                return random.choice(array1)
 
        elif mod == "straight_rand_10":
            if last_method in array1 and random.randint(0,20)!=0:
                return last_method
            else:
                return random.choice(array1)
 

        elif mod == "left":
            if (last_method - 1) % 4 in array1:
                return (last_method-1) % 4
            elif last_method in array1:
                return last_method
            elif (last_method + 1) % 4 in array1:
                return (last_method + 1) % 4
        elif mod == "left_rand":
            if random.randint(0,20)!= 0:
                if (last_method - 1) % 4 in array1:
                    return (last_method-1) % 4
                elif last_method in array1:
                    return last_method
                elif (last_method + 1) % 4 in array1:
                    return (last_method + 1) % 4
            else:
                return random.choice(array1)
        elif mod == "right":
            if (last_method + 1) % 4 in array1:
                return (last_method+1) % 4
            elif last_method in array1:
                return last_method
            elif (last_method - 1) % 4 in array1:
                return (last_method - 1) % 4
        elif mod == "right_rand":
            if random.randint(0,20)!= 0:
                if (last_method + 1) % 4 in array1:
                    return (last_method+1) % 4
                elif last_method in array1:
                    return last_method
                elif (last_method - 1) % 4 in array1:
                    return (last_method - 1) % 4
            else:
                return random.choice(array1)
            
        
        


def searchmethod(position,possible):
    for i in xrange(4):
        if move(position,i) == possible:
            if OUT: f.write(str(i)+ "     last method \n")
            return i

def ifinspace(point,space):
    if point[0] < 0 or point[0] >= maxxsize:
        return True
    elif point[1] < 0 or point[1] >= maxysize:
        return True
    else: 
        return ifinboard(point,space)



while inputaxle:
    count+=1
    inputaxle = sys.stdin.readline()
    while inputaxle[0] == '#' :
        inputaxle = sys.stdin.readline()
    if inputaxle == "die\n" :
        exit()
    axlestring=string.split(inputaxle)
    axle=map(int,axlestring)
    board[axle[0]][axle[1]]=1

    position=snake1[-1]
    method=alternative(position,board)
    if len(method)==0 or axle == snake1[-1]:
        print "die"
        sys.stdout.flush()
        exit()

    if OUT: f.write("ifsamearea   "+str(ifsamearea)+"\n")

    onemore1=move(position,method[0])
    another=[0,0,0] 
    possible = map(lambda x:move(position,x),method)
    Jiezhi = False      # if to optimize     
    if len(method) == 4:
        Jiezhi=True
    elif len(method) == 3:  
        another[0] = diagnose(position,possible[0],possible[1])
        another[1] = diagnose(position,possible[0],possible[2])
        another[2] = diagnose(position,possible[1],possible[2])
        if ifinboard(another[0],board)+ifinboard(another[1],board)+ifinboard(another[2],board) == 1:
            Jiezhi = True    # the two diagnose point are available then no need to check
    elif len(method) == 2:
        another[0] = diagnose(position,possible[0],possible[1])
        if not ifinboard(another[0],board): 
            # onely two way to go, then if one diagnose point is available, no need to check
            Jiezhi = True
    elif len(method) == 1:
        Jiezhi = True
    if Jiezhi == True:
        true+=1
        onemore=move(position,difai(method,last_method,mod))
        last_method = searchmethod(position,onemore)

        if OUT:
            f.write("lenmethod: "+str(method)+'\n')
            f.write(str(position[0])+' '+str(position[1])+'\n')
        if true % 7 == 0 and ifsamearea:
            lenspace,space = check(board,onemore)
            if not ifinspace(move(axle,0),space) and not ifinspace(move(axle,1),space) and not ifinspace(move(axle,2),space) and not ifinspace(move(axle,3),space):
                ifsamearea=False



    else:
        lenspace1=0
        lenspace2=0
        lenspace3=0
        lenspace1,space1=check(board,onemore1)
        if OUT: f.write(str(lenspace1)+' lenspace1\n')
        space2=[]
        space3=[]
        methodall=[[],[],[]]  # to catagorize the method by its connection into 3 or less groups
        another=[0,0,0]
        if len(method) == 2:
            methodall[0].append(method[0]) 
            if not ifinboard(move(position,method[1]),space1):
                ele=method[1]
                onemore2=move(position,ele) 
                lenspace2,space2=check(board,onemore2)
                methodall[1].append(ele)
            else:
                methodall[0].append(method[1])
        elif len(method) == 3: 
            methodall[0].append(method[0]) 
            sp2=False
            if not ifinboard(move(position,method[1]),space1):
                ele=method[1]
                onemore2=move(position,ele) 
                lenspace2,space2=check(board,onemore2)
                methodall[1].append(ele)
                sp2=True
                if OUT:
                    f.write(str(sp2)+'  space2 created by 2rd method\n')
            else:
                methodall[0].append(method[1])
        
            if ifinboard(move(position,method[2]),space1):
                methodall[0].append(method[2]) 
                if OUT: 
                    f.write('all3 in same space1')
            elif not sp2:
                methodall[1].append(method[2])
                onemore2=move(position,method[2])
                lenspace2,space2=check(board,onemore2)
                if OUT:
                    f.write(str(sp2)+'  space2 created by 3rd method\n')
            elif  (sp2 and ifinboard(move(position,method[2]),space2)):
                methodall[1].append(method[2])
                if OUT:
                    f.write('space2 has two now\n')
                
            elif sp2 and( not ifinboard(move(position,method[2]),space2)):
                methodall[2].append(method[2])
                lenspace3,space3=check(board,move(position,method[2]))
                if OUT:
                    f.write('space3 created\n')


        choose = 0
        choice=max(lenspace1,lenspace2,lenspace3)
        if choice == lenspace1:
            choose = 1
            onemore=move(position,difai(methodall[0],last_method,mod))
            last_method = searchmethod(position,onemore)
            if OUT:
                f.write("space1 choose\n")
                f.write(str(onemore[0])+' '+str(onemore[1])+'\n')
        elif choice == lenspace2:
            choose = 2
            onemore=move(position,difai(methodall[1],last_method,mod))
            last_method = searchmethod(position,onemore)
            if OUT:
                f.write("space2 choose\n")
                f.write(str(onemore[0])+' '+str(onemore[1])+'\n')
        else:
            choose = 3
            onemore=move(position,difai(methodall[2],last_method,mod))
            last_method = searchmethod(position,onemore)
           
        if choose == 1: space = space1
        elif choose == 2: space =space2
        elif choose ==3 : space = space3
       
        if not ifinspace(move(axle,0),space) and not ifinspace(move(axle,1),space) and not ifinspace(move(axle,2),space) and not ifinspace(move(axle,3),space):
            ifsamearea=False

                     
        if OUT:
                f.write("space3 choose\n")
                f.write(str(onemore[0])+' '+str(onemore[1])+'\n')
               
        if OUT:
            f.write('1: '+str(lenspace1) + ',  2: '+str(lenspace2) + ' ,3:'+str(lenspace3)+'  \n')    
            printboardf(board)
            f.write(str(count)+'\n')
            f.write(str(position[0])+' '+str(position[1])+'\n')
            f.write(str(onemore[0])+' '+str(onemore[1])+'\n')

    snake1[-1]=onemore
    board[onemore[0]][onemore[1]]=1
    sys.stdout.write("%d %d\n" %(onemore[0],onemore[1]))
    sys.stdout.flush()

