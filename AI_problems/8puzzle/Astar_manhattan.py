from Astar_displaced import possibleMoves,toInt,swap,showBoard,toList,reachGoal
import sys
import operator 
initial_state = [7,2,4,5,0,6,8,3,1]
goal_state = [0,1,2,3,4,5,6,7,8]
history_boards = {}

def manhattan_distance(board_state):
    distance = 0
    for (i,j) in [(goal_state.index(item),board_state.index(item)) for item in board_state if item in goal_state]:
        distance +=tuple_distance(conversion(i),conversion(j))
    return distance

def conversion(index):
    if index==0:
        return (0,0)
    elif index==1:
        return (0,1)
    elif index==2:
        return (0,2)
    elif index==3:
        return (1,0)
    elif index==4:
        return (1,1)
    elif index==5:
        return (1,2)
    elif index==6:
        return (2,0)
    elif index==7:
        return (2,1)
    elif index==8:
        return (2,2)
    else:
        print "Dont accept this input"
        return False

def tuple_distance(t1,t2):
    return abs(t1[0]-t2[0])+abs(t1[1]-t2[1])


def cost(board_state,layer):
    return layer+manhattan_distance(board_state)

def puzzle(parentNode,history_boards):
    global possibleMoves
    nodes = 0
    while(True):
#        print parentNode
        if reachGoal(parentNode,goal_state):
            print "Congradulations! You reach goal after "+str(history_boards[toInt(parentNode)][2])+" moves"
            print "Total nodes expanded: ",nodes
            print "\n\nThe move process replay blow: \n"
            num = toInt(parentNode)
            while True: 
                print "\nAfter "+str(history_boards[num][2])+" moves you got: "
                print showBoard(num)
                num = history_boards[num][1]
                if history_boards[num][2] == 0:
                    print "\nAfter "+str(history_boards[num][2])+" moves you got: "
                    print showBoard(num)
                    return 

        else:
            possible_moves = possibleMoves(parentNode)
            for move_tuple in possible_moves:
                tmp = parentNode[:]
                sonNode = swap(tmp,move_tuple)
                if toInt(sonNode) not in history_boards:
                    nodes +=1
                    layer = history_boards[toInt(parentNode)][2]
                    history_boards[toInt(sonNode)] = (cost(sonNode,layer+1),toInt(parentNode),layer+1)
            history_boards[toInt(parentNode)] = (sys.maxint,history_boards[toInt(parentNode)][1],history_boards[toInt(parentNode)][2])
            parentNode = toList(min(history_boards.iteritems(),key=operator.itemgetter(1))[0])
            

if __name__ == "__main__":
    history_boards[toInt(initial_state)]=(cost(initial_state,0),0,0)
    puzzle(initial_state,history_boards)
