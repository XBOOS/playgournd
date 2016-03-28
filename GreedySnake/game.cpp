#include<iostream>
#include<string>
#include<deque>
#include<vector>

using namespace std;

const int kMaxFood = 20;

const char kEmptyTile = ' ';
const char kWallTile = '#';
const char kFoodTile = '$';
const char kSnakeTile = '*';

struct pointT
{
    int row,col;
}

struct gameT
{
    vector<string> world;
    int numRows,numCols; //size of the playing field

    deque<pointT> snake;
    int dx,dy;//the snake direction

    int numEater; //how much food we have eaten


};

int main(){
    gameT game;

    //two state of game
    InitializeGame(game);
    Runsimulation(game);
    return 0;

}
