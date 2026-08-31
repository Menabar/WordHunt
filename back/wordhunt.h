#ifndef WORDHUNT
#define WORDHUNT

#include <string>
#include <map>
#include <vector>

using namespace std;

struct Position {
    int x;
    int y;
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

struct WordSolution {
    string word;
    vector<Position> path;
    WordSolution(string w = "", vector<Position> p = vector<Position>()) : word(w), path(p) {}
};

/*
board dimensions; the solver is written against these rather than literals,
so a different rectangle only requires changing them here
*/
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
finds and returns all valid words with their paths
*/
vector<WordSolution> findWordsWithPaths(struct Board b);

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
bool inBounds(const struct Board &b, int x, int y);

/*
in between step for snake() calling itself that does the bounds checks
*/
void snakeRecursion(struct Board &b, int x, int y);

/*
creates and returns a hashmap of valid and potentially valid words
*/
map<string,int> createMap();

/*
input: infile name and outfile name.
output: nothing
prints all words and almost-words to the file, one per line, marked '!' for a
complete word and '.' for a prefix. should be run whenever english_words.txt changes.
*/
void populatePairs(string infile, string outfile);

#endif //WORDHUNT
