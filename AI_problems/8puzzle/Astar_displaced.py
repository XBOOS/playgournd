from ids_8puzzle import possibleMoves,toInt,swap,showBoard,toList,reachGoal
import sys
import operator 
initial_state = [7,2,4,5,0,6,8,3,1]
goal_state = [0,1,2,3,4,5,6,7,8]
def misplacedCount(board_state):
    return sum([1 for (i,j) in zip(goal_state,board_state) if i!=j and j!=0])

def cost(board_state,layer):
    return layer+misplacedCount(board_state)

def puzzle(parentNode,history_boards):
    global possibleMoves
    nodes = 0
    while(True):
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
    history_boards = {}
    history_boards[toInt(initial_state)]=(cost(initial_state,0),0,0)
    puzzle(initial_state,history_boards)
