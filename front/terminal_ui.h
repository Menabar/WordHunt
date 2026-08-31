#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include <string>
#include <vector>
#include "../back/wordhunt.h"

using namespace std;

class TerminalUI {
private:
    Board board;
    vector<WordSolution> solutions;

public:
    TerminalUI();
    void run();
    void displayWelcome();
    void displayBoard();
    void inputBoard();
    void solveAndDisplay();
    void displaySolutions();
    void clearScreen();
};

#endif // TERMINAL_UI_H
