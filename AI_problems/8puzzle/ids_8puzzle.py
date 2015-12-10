# using the interactive deepening search to solve the 8 puzzle problem
from bfs_8puzzle import swap,possibleMoves,showBoard,toInt
initial_state = [7,2,4,5,0,6,8,3,1]
goal_state = [0,1,2,3,4,5,6,7,8]
history_boards = {}
moves = 0
level = 1
layer = 0
def toList(intNum):
    string = str(intNum)
    if len(string)<9:
        string = '0'+string
    return map(int,list(string))   



def reachGoal(board_state,goal_state):
    return board_state ==goal_state 

#every move go deeper one layer and hash the memory
# sonNode => parentNode,parent_other_possibleMoves
# so that you could easily use hash to go back to parent and go down other sibling,remove the possbleMoves you have already consumed
def move(parentNode,possible_moves):
    global moves
    global layer
    while possible_moves: #not None and not empty
        move_tuple = possible_moves.pop()
        sonNode = parentNode[:]
        sonNode =  swap(sonNode,move_tuple)
        possible_moves_copy = possible_moves
        if toInt(sonNode) not in history_boards and toInt(sonNode) not in (x[0]for x in history_boards.values()):
            layer +=1
            history_boards[toInt(sonNode)] = [toInt(parentNode),possible_moves_copy]
            return sonNode
    return None

def puzzle(initial_state,level):
    global moves
    global possibleMoves
    global layer
    if level<=0:
        print "Invalid level input, no moves avaliable!"
        exit(1)
    parentNode =initial_state
    possible_moves= possibleMoves(parentNode)
    while(layer<=level-1):
        sonNode= move(parentNode,possible_moves)
        if sonNode:
            if reachGoal(sonNode,goal_state):
                print "Congraduations! You reach goal after "+str(layer)+" moves with level = ",level
                print "Number of nodes",len(history_boards)
                print "\n\nThe move process replay blow: \n"
                num = toInt(sonNode)
                while layer>0:
                    print "\nAfter "+str(layer)+" moves you got: "
                    print showBoard(num)
                    num = history_boards[num][0]
                    layer -=1
                return True
            
            parentNode = sonNode
            possible_moves = possibleMoves(parentNode)
            if(layer ==level):
                parentNode = toList(history_boards[toInt(sonNode)][0])
                possible_moves= history_boards[toInt(sonNode)][1]
                del history_boards[toInt(sonNode)]
                layer -=1
            continue
        else:
            if layer==0:
                
                print "You didn't find the goal with level = ",level
                return False
            possible_moves= history_boards[toInt(parentNode)][1]
            tmp = parentNode
            parentNode = toList(history_boards[toInt(parentNode)][0])
            del history_boards[toInt(tmp)]
            
            layer -=1
            continue


if __name__ =="__main__":
    level = 1
    while True:
        if puzzle(initial_state,level):

            break
        layer = 0
        level +=1
