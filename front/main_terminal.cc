#include "terminal_ui.h"
#include <iostream>

using namespace std;

int main() {
    try {
        TerminalUI ui;
        ui.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
