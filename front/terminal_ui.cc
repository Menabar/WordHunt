#include "terminal_ui.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <cctype>

using namespace std;

namespace {

const int CELL_COUNT = _width * _height;

/*
prints text centred inside a box, so the borders line up whatever the text is
*/
void banner(const string &text){
    const int inner = 41;
    string top = "╔", bottom = "╚";
    for(int i=0; i<inner; i++){
        top += "═";
        bottom += "═";
    }
    top += "╗";
    bottom += "╝";

    int pad = (inner - (int)text.length()) / 2;
    string middle = "║" + string(pad, ' ') + text
                  + string(inner - pad - (int)text.length(), ' ') + "║";

    cout << top << endl << middle << endl << bottom << endl;
}

/*
builds one horizontal rule of the grid from its left, joining and right pieces
*/
string gridRule(const string &left, const string &joint, const string &right){
    string rule = "   " + left;
    for(int x=0; x<_width; x++){
        rule += "───";
        rule += (x+1 == _width) ? right : joint;
    }
    return rule;
}

}

TerminalUI::TerminalUI() {
    for(int x=0; x<_width; x++){
        for(int y=0; y<_height; y++){
            board.board[x][y] = ' ';
            board.board_available[x][y] = true;
        }
    }
}

void TerminalUI::clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear the screen
}

void TerminalUI::displayWelcome() {
    clearScreen();
    cout << endl;
    banner("WORD HUNT SOLVER");
    cout << endl;
}

void TerminalUI::displayBoard() {
    cout << endl << "   Board:" << endl;
    cout << gridRule("┌", "┬", "┐") << endl;

    for(int y=0; y<_height; y++){
        cout << "   │";
        for(int x=0; x<_width; x++){
            char cell = board.board[x][y];
            cout << " " << (cell == ' ' ? '.' : cell) << " │";
        }
        cout << endl;

        if(y+1 < _height){
            cout << gridRule("├", "┼", "┤") << endl;
        }
    }

    cout << gridRule("└", "┴", "┘") << endl << endl;
}

void TerminalUI::inputBoard() {
    cout << "Enter your " << _width << "x" << _height << " board, "
         << CELL_COUNT << " letters, left to right and top to bottom." << endl;
    cout << "Spaces are ignored, so \"CATS DOGS FISH BIRD\" works." << endl << endl;

    string letters;
    while((int)letters.length() < CELL_COUNT){
        if(letters.empty()){
            cout << "Board: ";
        }
        else{
            cout << "Need " << (CELL_COUNT - (int)letters.length())
                 << " more: ";
        }

        string line;
        if(!getline(cin, line)){
            //stdin closed before a full board arrived; without this the read
            //loop would spin forever on the failed stream
            throw runtime_error("unexpected end of input, got "
                                + to_string(letters.length()) + " of "
                                + to_string(CELL_COUNT) + " letters");
        }

        for(size_t i=0; i<line.length(); i++){
            if(isalpha((unsigned char)line[i])){
                letters += toupper((unsigned char)line[i]);
            }
        }
    }

    //any letters past the end of the board are ignored
    for(int y=0; y<_height; y++){
        for(int x=0; x<_width; x++){
            board.board[x][y] = letters[y * _width + x];
        }
    }
}

void TerminalUI::solveAndDisplay() {
    clearScreen();
    displayBoard();

    cout << "Solving..." << endl;

    solutions = findWordsWithPaths(board);

    //longest first, then alphabetically within a length
    sort(solutions.begin(), solutions.end(),
         [](const WordSolution& a, const WordSolution& b) {
             if(a.word.length() != b.word.length()) {
                 return a.word.length() > b.word.length();
             }
             return a.word < b.word;
         });

    displaySolutions();
}

void TerminalUI::displaySolutions() {
    cout << endl;

    if(solutions.empty()) {
        cout << "No words found." << endl;
        return;
    }

    banner("FOUND " + to_string(solutions.size()) + " WORDS");

    int currentLength = -1;
    for(size_t i=0; i<solutions.size(); i++){
        const WordSolution &solution = solutions.at(i);

        if((int)solution.word.length() != currentLength){
            currentLength = solution.word.length();
            cout << endl << "--- " << currentLength << " letters ---" << endl;
        }

        cout << left << setw(16) << solution.word << " [";
        for(size_t j=0; j<solution.path.size(); j++){
            if(j > 0) cout << " ";
            cout << "(" << solution.path.at(j).x << "," << solution.path.at(j).y << ")";
        }
        cout << "]" << endl;
    }

    cout << endl;
}

void TerminalUI::run() {
    displayWelcome();
    inputBoard();
    solveAndDisplay();
}
