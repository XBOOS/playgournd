#include<iostream>
#include<string>
#include<deque>
#include<vector>
#include<cstdlib> // rand,srand,system
#include<ctime> //clock,clock_t,CLOCKS_PER_SEC,time

using namespace std;


const double kTurnRate = 0.2;// probability of turning at each step

const double kWaitTime = 0.1; //time to wait in seconds, between frame

const int kMaxFood = 20;

const char kEmptyTile = ' ';
const char kWallTile = '#';
const char kFoodTile = '$';
const char kSnakeTile = '*';

/* The string used to clear the display before printing the game board
 * Windows, "CLS"
 * Mac OS X / linux "clear"
 */
const string kClearCommand = "clear";

/* A struct encoding a point in a two-dimentional grid.
 */

struct pointT
{
    int row,col;
}


/* A struct containing relevant game information
 */
struct gameT
{
    vector<string> world;
    int numRows,numCols; //size of the playing field

    deque<pointT> snake;
    int dx,dy;//the snake direction

    int numEaten; //how much food we have eaten

};

/* Reads a line of text from the user
 */
string GetLine(){

    string result;
    getline(cin,result);
    return result;
}

/* A helper function constructing a point in a given position*/
pointT MakePoint(int row,int col)
{
    pointT result;
    result.row = row;
    result.col = col;
    return result;

}

/*Return true when probability < set probability
 * By scaling down rand() by RAND_MAX+1.0 so that we can have value in [0,1)
 * and returning value the value is less the set probability
 */
bool RandomChance(double probability)
{
    return (rand()/RAND_MAX+1.0) < probability;

}
/*Place a piece of food randomly on the board. Assumes that there is some
 * free space remaining
 */

void PlaceFood(gameT& game)
{
    while(true){
        int row = rand() % game.numRows;
        int col = rand() % game.numCols;

        if(game.world[row][col]==kEmptyTile){
            game.world[row][col] = kFoodTile;
            return;
        }

    }
}

/*Clears the display and prints the game board
 * so that we could see the changing process of the game board
 */
void PrintWorld(gameT& game)
{
    //clear the screen history
    system(kClearCommand.c_str());

    //print each row
    for(int i =0;i< game.numRows;++i){
        cout<<game.world[i]<<endl;
    }

    cout<<"Food eaten:  "<<game.numEaten<<endl;
}


/*Given an ifstream to a file containing CORRECTLY-FORMATTED world data
 * loads in the game world
 *
 * The format used is as follows
 * Line1 : numRows numCOls
 * Line2 : dx dy
 * Rest: World data
 *
 * We assume that the world is correctly sized and there is a single
 * '*' character in the world that's the starting point for the snake.
 */
void LoadWorld(gameT& game,ifstream& input)
{
    input>>game.numRows>>game.numCols;
    game.world.resize(game.numRows);

    input>>game.dx>>game.dy;

    //we are going to use getline() to get in the world date,
    //so we need to make sure we consume the newline character
    //at the end of the line containing the input data.
    //we use getline() to handle this.
    //remember that the getline is unformated function
    string dummy;
    getline(input,dummy);

    for(int row = 0; row<game.numRows;++row){
        //checks to see if the * character is in this line. if so, make the snake
        getline(input,game.world[row]);
        int col = game.world[row].find('*');
        if(col!=string::npos){
            game.snake.push_back(MakePoint(row,col));
        }
    }

    //set the numEaten to 0
    game.numEaten = 0;

}


/* Helper function which returns whether a point is contained in the game
 * grid
 */
bool InWorld(pointT& pt,gameT& game)
{
    return pt.col>=0 &&
           pt.col<game.numCols &&
           pt.row>=0 &&
           pt.row<game.numRows;
}

/*returns whether, if the snake head is at position head,the snake
 * has crashed.
 */
bool Crashed(pointT head,gameT&game)
{
    //crashed if the head is out of bounds,on a wall or snake body
    return !InWorld(head,game)||
            game.world[head.row][head.col] == kWallTile ||
            game.world[head.row][head.col] == kSnakeTile;
}

/*Returns the next postion occupied by the head if the snake
 * is moving in the direction dx,dy.
 */
pointT GetNextPosition(gameT& game,int dx,int dy)
{
    //get the head
    pointT nextSpot = game.snake.front();

    //update the head
    nextSpot.col +=dx;
    nextSpot.row +=dy;

    return nextSpot;
}


/* Performs AI logic to control the snake.
 * The behavior is as follows:
 * 1. If we are going to crash, we try to turn.
 * 2. Independently, we have a percent chance to turn at each step
 * 3. If we do have to turn, we always turn in a safe direction, and
 * if there are multiple options, we pick on randomly.
 */
void PerformAI(gameT& game)
{
    //see where wer are going in the next step
    pointT nextSpot = GetNextPosition(game,game.dx,game.dy);

    //if this crashes us or we just feel like turning. turn!
    if(Crashed(nextSpot,game) || RandomChance(kTurnRate))
    {
    /* Compute what direction we'd be facing if we turned left or
         * right.  From linear algebra we have the following:
         *
         * For a left turn:
         * |x'|   |0 -1||x| --> x' = -y
         * |y'| = |1  0||y| --> y' =  x
         *
         * For a right turn:
         * |x'|   |0  1||x| --> x' =  y
         * |y'| = |-1 0||y| --> y' = -x
         */

        int leftDx = -game.dy;
        int leftDy = game.dx;

        int rightDx = game.dy;
        int rightDy = -game.dx;

        //check if turning left or right will cause us to crash
        bool canLeft = !Crashed(GetNextPosition(game,leftDx,leftDy),game);
        bool canRight = !Crashed(GetNextPosition(game,rightDx,rightDy),game);

        //Now determine whether to turn left or turn right
        bool willTurnLeft;
        if(!canLeft && !canRight)
            return;
        else if(canLeft && !canRight)
            willTurnLeft = true;
        else if(!canLeft && canRight)
            willTurnLeft = false;
        else
            willTurnLeft = RandomChance(0.5);

        //based on the direction, turn appropriatedly
        game.dx = willTurnLeft? leftDx :rightDx;
        game.dy = willTurnLeft? leftDy : rightDy;
    }
}
/* Moves the snake one step in its current direction
 * and handles collicions and eating food.
 * Returns true if we didnt crash and false if we did.
 */
bool MoveSnake(gameT& game)
{
    pointT nextSpot = GetNextPosition(game,game.dx,game.dy);

    if(Crashed(nextSpot,game)){
        return false;
    }

    bool isFood = (game.world[nextSpot.row][nextSpot.col]) == kFoodTile;

    //update the display
    game.world[nextSpot.row][nextSpot.col] = kSnakeTile;

    //push the head
    game.snake.push_front(nextSpot);

    //if we got food,pick a new spot and dont move the tail.This will cause us
    //to extend by one spot. otherwise clear the tail and remove it from the board
    if(isFood){

        PlaceFood(game);
        ++game.numEaten;

    }
    else{
        game.world[game.snake.back().row][game.snake.back().col] = kEmptyTile;
        game.snake.pop_back();
    }

    return true;
}

//Pauses for a few milliseconds so we can see what happing.
//This is implemented using a busy loop, which is less-optimal but
//does not require platform specific features.
//
void Pause()
{
    clock_t start = clock();
    while(static_cast<double>(clock()-start)/CLOCKS_PER_SEC < kWaitTime);
}

void DisplayResult(gameT& game)
{
    PrintWorld(game);
    if(game.numEaten == kMaxFood)
        cout<< "Yay!  The snake won!" << endl;
    else
        cout<<"Oh no!  The snake crashed!" <<endl;
}

/* Prompts the user for a filename, then opens the specific file
 */
void OpenUserFile(ifstream& input)
{
    while(true){
        cout<<"Enter level file:  ";
        input.open(GetLine().c_str());

        if(!input.fail()) return;
        cout<<"Sorry, I cant open that file. "<<endl;
        input.clear();
    }
}

/*Initialize the game and loads the level file.
 */
void InitializeGame(gameT& game)
{
    //seed the randomizer
    srand(static_cast<int>(time(NULL)));

    ifstream input;
    OpenUserFile(input);
    LoadWorld(game,input);
}

void Runsimulation(gameT& game)
{
    while(game.numEaten<kMaxFood)
    {
        PrintWorld(game);
        PerformAI(game);

        if(!MoveSnake(game))
            break;
        Pause();
    }

    DisplayResult(game);
}

/* The main program
 * Initializes the world,
 * then runs the simulation.
 */
    int main(){
    gameT game;

    //two state of game
    InitializeGame(game);
    Runsimulation(game);
    return 0;

}
