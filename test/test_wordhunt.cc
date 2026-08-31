#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>
#include "../back/wordhunt.h"

using namespace std;

// Test counter
int tests_passed = 0;
int tests_failed = 0;

// Helper function to report test results
void reportTest(string test_name, bool passed) {
    if (passed) {
        cout << "[PASS] " << test_name << endl;
        tests_passed++;
    } else {
        cout << "[FAIL] " << test_name << endl;
        tests_failed++;
    }
}

// Test board initialization
void testBoardInitialization() {
    Board b;
    bool passed = (b.height == 4 && b.width == 4);
    reportTest("Board Initialization", passed);
}

// Test populateBoard function
void testPopulateBoard() {
    Board b;
    string test_input = "ABCDEFGHIJKLMNOP";
    populateBoard(b, test_input);

    // board[x][y] comes from input[width*y + x], so an interior cell catches
    // a transposition that the corners would not
    bool passed = (b.board[0][0] == 'A'
                && b.board[3][3] == 'P'
                && b.board[1][2] == 'J');

    if (!passed) {
        cout << "  Got board[0][0]=" << b.board[0][0]
             << " board[1][2]=" << b.board[1][2]
             << " board[3][3]=" << b.board[3][3]
             << ", expected A J P" << endl;
    }

    reportTest("Populate Board", passed);
}

// Test inBounds function
void testInBounds() {
    Board b;
    b.height = 4;
    b.width = 4;

    // Initialize board_available
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b.board_available[i][j] = true;
        }
    }

    bool passed = true;

    // Test valid bounds
    if (!inBounds(b, 0, 0) || !inBounds(b, 3, 3)) {
        passed = false;
    }

    // Test invalid bounds
    if (inBounds(b, -1, 0) || inBounds(b, 4, 0) || inBounds(b, 0, -1) || inBounds(b, 0, 4)) {
        passed = false;
    }

    // Test unavailable tile
    b.board_available[2][2] = false;
    if (inBounds(b, 2, 2)) {
        passed = false;
    }

    reportTest("InBounds Function", passed);
}

// Test searchDictionary function with known words
void testSearchDictionary() {
    bool passed = true;

    // Test with a known word (should return 1)
    int result1 = searchDictionary("THE");
    if (result1 != 1) {
        cout << "  Expected 'THE' to return 1, got " << result1 << endl;
        passed = false;
    }

    // Test with a non-existent word (should return 0)
    int result2 = searchDictionary("XYZQWERTY");
    if (result2 != 0) {
        cout << "  Expected 'XYZQWERTY' to return 0, got " << result2 << endl;
        passed = false;
    }

    reportTest("Search Dictionary", passed);
}

// Test board printing
void testPrintBoard() {
    Board b;
    string test_input = "ABCDEFGHIJKLMNOP";
    populateBoard(b, test_input);

    bool passed = true;
    try {
        // Redirect cerr to capture output
        streambuf* old_cerr = cerr.rdbuf();
        ostringstream capture;
        cerr.rdbuf(capture.rdbuf());

        printBoard(b);

        // Restore cerr
        cerr.rdbuf(old_cerr);

        string output = capture.str();

        // the first row must read across, not down
        if (output.substr(0, 8) != "A B C D\n") {
            cout << "  Expected first row 'A B C D', got '"
                 << output.substr(0, 7) << "'" << endl;
            passed = false;
        }
    } catch (...) {
        passed = false;
    }

    reportTest("Print Board", passed);
}

// Test readTiles against the sample board committed at the repo root.
// Tests run from the project root, so "data/input.txt" resolves there.
void testReadTiles() {
    Board b;
    string result = readTiles(b);

    bool passed = (result == "HELLOPUZZLEWORLD");
    if (!passed) {
        cout << "  Expected HELLOPUZZLEWORLD from input.txt, got '"
             << result << "'" << endl;
    }

    reportTest("Read Tiles", passed);
}

// Test edge cases
void testEdgeCases() {
    Board b;
    bool passed = true;

    // Test with all same letters
    string same_letters = "AAAAAAAAAAAAAAAA";
    populateBoard(b, same_letters);

    // Check if board is populated
    if (b.board[0][0] != 'A' || b.board[3][3] != 'A') {
        passed = false;
    }

    reportTest("Edge Cases - Same Letters", passed);
}

// Main test runner
int main() {
    cout << "Running Word Hunt Test Suite" << endl;
    cout << "=============================" << endl << endl;

    testBoardInitialization();
    testPopulateBoard();
    testInBounds();
    testPrintBoard();
    testReadTiles();
    testSearchDictionary();
    testEdgeCases();

    cout << endl << "=============================" << endl;
    cout << "Test Results: " << tests_passed << " passed, " << tests_failed << " failed" << endl;
    cout << "=============================" << endl;

    return (tests_failed == 0) ? 0 : 1;
}
