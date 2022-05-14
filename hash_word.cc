#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include <ctime> 
#include "wordhunt.h"
#include "hash_word.cc"

using namespace std;
using namespace std::chrono;

string current_word = "";
map<string,int> m = createMap();

/*
reads the tiles from standard input, one character per line
*/
string readTiles(struct Board b){

    ifstream tile_file("input.txt");
    string returnMe = "", temp;

    for(int i=0; i<(b.height * b.width); i++){
        tile_file >> temp;

        //ensure there is exactly one letter per line
        if(temp.length() != 1){
            cerr << "ERROR: expecting one letter." << endl;
            exit(0);
        }

        //ensure the letter is uppercase
        temp.at(0) -= ((int)temp.at(0) <= 90) ? 0 : 32;

        //ensure the character is a letter
        if((int)temp.at(0) < 65 || (int)temp.at(0) > 90){
            cerr << "ERROR: expecting a letter." << endl;
            exit(0);
        }

        returnMe += temp;
    }
    return returnMe;
}

/*
places each character into the proper tile on the Board
*/
void populateBoard(struct Board &b, string input){
    for(int y=0; y<b.height; y++){
        for(int x=0; x<b.width; x++){
            b.board[x][y] = input.at(4*y+x);
        }
    }
}

/*
resets availability to true
*/
void resetAvailable(struct Board &b){
    for(int i=0; i<b.height; i++){
        for(int j=0; j<b.width; j++){
            b.board_available[i][j] = true;
        }
    }
}

/*
searches the english_words.txt file for the word
*/
int searchDictionary(string word){
    return m[word];
}

/*
frees a tile if it is within range
*/
void freeIt(struct Board &b, int x, int y){
    current_word = current_word.substr(0, current_word.length()-1);
    b.board_available[x][y] = true;
}

/*
checks if the selected tile is a valid candidate
*/
bool inBounds(struct Board b, int x, int y){
    if(x < 0 || x >= b.width || y < 0 || y >= b.height || b.board_available[x][y] == false){
        return false;
    }
    return true;
}

/*
in between step for snake() calling itself that does the bounds checks
*/
void snakeRecursion(struct Board b, int x, int y){
    if(current_word.length() == b.height * b.width){
        return;
    }
    if(inBounds(b, x+1, y+1)){
        snake(b, x+1, y+1);
        freeIt(b, x+1, y+1);
    }
    if(inBounds(b, x+1, y)){
        snake(b, x+1, y);
        freeIt(b, x+1, y);
    }
    if(inBounds(b, x+1, y-1)){
        snake(b, x+1, y-1);
        freeIt(b, x+1, y-1);
    }
    if(inBounds(b, x, y+1)){
        snake(b, x, y+1);
        freeIt(b, x, y+1);
    }
    if(inBounds(b, x, y-1)){
        snake(b, x, y-1);
        freeIt(b, x, y-1);
    }
    if(inBounds(b, x-1, y+1)){
        snake(b, x-1, y+1);
        freeIt(b, x-1, y+1);
    }
    if(inBounds(b, x-1, y)){
        snake(b, x-1, y);
        freeIt(b, x-1, y);
    }
    if(inBounds(b, x-1, y-1)){
        snake(b, x-1, y-1);
        freeIt(b, x-1, y-1);
    }
}

/*
finds every word combination given a starting tile
*/
void snake(struct Board &b, int x, int y){

    //check if the given tile is within bounds
    if(x < 0 || y < 0 || x >= b.width || y >= b.height){
        //out of bounds
        return;
    }

    //check if the tile is free
    if(!b.board_available[x][y]){
        //not available
        return;
    }

    b.board_available[x][y] = false;

    current_word += b.board[x][y];
    
    if(current_word.length() >= 3){    //look for the current word
        switch(searchDictionary(current_word)){
            //non word and will never be a word
            case 0:
                return;
            break;
            //is a word
            case 1:
                cout << current_word << endl;
            break;
            //not a word, could lead to a word
            case 2:
            break;
            //should be impossible
            default:
                cerr << "Something went wrong." << endl;
                exit(0);
            break;
        }
    }
    
    snakeRecursion(b, x, y);

}

/*
finds and outputs all valid words
*/
void findWords(struct Board b){
    for(int y=0; y<b.height; y++){
        for(int x=0; x<b.width; x++){
            //reset availability of each cell
            resetAvailable(b);

            current_word = "";
            snake(b, x, y);
        }
    }
}

/*
prints the board to the terminal
*/
void printBoard(struct Board b){
    for(int x=0; x<b.width; x++){
        for(int y=0; y<b.height; y++){
            cerr << b.board[y][x];
            if(y+1 == b.height){
                cerr << endl;
            }
            else{
                cerr << " ";
            }
        }
    }
}

int main(){

    Board b;

    string tiles = readTiles(b);

    populateBoard(b, tiles);

    printBoard(b);

    findWords(b);

    return 0;

}
