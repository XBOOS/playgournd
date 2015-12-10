
#BFS method to solve the 8 puzzle problem.
initial_state = [7,2,4,5,0,6,8,3,1]
#initial_state = [1,4,2,0,5,8,3,6,7]
goal_state = [0,1,2,3,4,5,6,7,8]
history_boards = {}
current_layer_states1 =[initial_state]
current_layer_states2 =[]
layer_markerSwitch =True#True for 1 and False for 2
layer = 0 # to record how many moves you've got
def possibleMoves(board_state):
# 2 possible moves
    blank = board_state.index(0)
    if blank==0:
        return[(0,1),(0,3)]
    elif blank==2:
        return[(2,1),(2,5)]
    elif blank==6:
        return[(6,3),(6,7)]
    elif blank==8:
        return[(8,5),(8,7)]
# 3 possible moves
    elif blank==1:
        return[(1,0),(1,2),(1,4)]
    elif blank==3:
        return[(3,0),(3,4),(3,6)]
    elif blank==5:
        return[(5,2),(5,4),(5,8)]
    elif blank==7:
        return[(7,6),(7,4),(7,8)]
# 4 possible moves
    elif blank==4:
        return[(4,1),(4,3),(4,5),(4,7)]
    else:
        print("Invalid input")
        exit(0)
        

#moves = possibleMoves(initial_state);
#print moves
def move(board_state,possibleMoves,current_layer):
    for move_tuple in possibleMoves:
        current_board_state = board_state[:] 
        tmp = swap(current_board_state,move_tuple)
        if not toInt(tmp) in history_boards:
            current_layer.append(tmp)
            history_boards[toInt(tmp)] = toInt(board_state)
    return         
def toInt(board_state):
    return int(''.join(map(str, board_state)))

def swap(board_state,move_tuple):
    board_state[move_tuple[0]],board_state[move_tuple[1]] =  board_state[move_tuple[1]],board_state[move_tuple[0]] 
    return board_state

def reachGoal(board_state):
    return board_state == goal_state
def showBoard(num):
    num = str(num)
    if len(num)<9:
        num = '0'+num
    return ' '.join(num[:3])+'\n'+' '.join(num[3:6])+'\n'+' '.join(num[6:])

    
    
def puzzle(pre_layer_states,new_layer_states):
    
    global layer
    layer =layer + 1        
    for board_state in pre_layer_states:
        if reachGoal(board_state):
            print "Congradulations! You won after "+str(layer-1)+" moves.\n\n"
            print "Number of nodes",len(history_boards)
            print "\n\nThe move process replay blow: \n"
            num = toInt(board_state)
            while layer>0:
                print "\nAfter "+str(layer-1)+" moves you got: "
                print showBoard(num)
                num = history_boards[num]
                layer -=1
                
            return False
        else:
            move(board_state,possibleMoves(board_state),new_layer_states)
    return True

if __name__ == "__main__":
    while True:
        if(layer_markerSwitch):
            if puzzle(current_layer_states1,current_layer_states2):
                layer_markerSwitch = not layer_markerSwitch
                print "The current move is layer ",layer
#                print "Number of nodes",len(history_boards)
                del current_layer_states1[:]
            else:
                break
        else:
            if puzzle(current_layer_states2,current_layer_states1):
                layer_markerSwitch = not layer_markerSwitch
                print "The current move is layer ",layer
#                print "Number of nodes ",len(history_boards)
                del current_layer_states2[:]
            else:
                break


