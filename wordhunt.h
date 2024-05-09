#ifndef WORDHUNT
#define WORDHUNT

#include <string>

using namespace std;

const int _width = 4, _height = 4;

struct Board{
    int width = _width;
    int height = _height;
    char board[_width][_height];
    bool board_available[_width][_height];
};

/*
reads the tiles from standard input, one character per line
*/
string readTiles(struct Board b);

/*
places each character into the proper tile on the Board
*/
void populateBoard(struct Board &b, string input);

/*
prints the board to the terminal
*/
void printBoard(struct Board b);

/*
searches the english_words.txt file for the word
*/
int searchDictionary(string word);

/*
finds and outputs all valid words
*/
void findWords(struct Board b);

/*
finds every word combination given a starting tile
*/
void snake(struct Board &b, int x, int y);

/*
frees a tile if it is within range
*/
void freeIt(struct Board &b, int x, int y);

/*
checks if the selected tile is a valid candidate
*/
bool inBounds(struct Board b, int x, int y);

/*
in between step for snake() calling itself that does the bounds checks
*/
void snakeRecursion(struct Board b, int x, int y);

/*
creates a returns a hashmap of valid an potentially valid words
*/
map<string,int> createMap();

/*
input: infile name and outfile name.
output: nothing
prints all words and almost-words to the file, formatted to be in a hashmap named "m".
*/
void populatePairs(string infile, string outfile);

#endif //WORDHUNT
