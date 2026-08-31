#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "../back/wordhunt.h"
#include "../back/trie.h"

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

// Test Trie insertion and search
void testTrieBasics() {
    Trie trie;
    trie.insert("CAT", 1);
    trie.insert("CA", 2);
    trie.insert("DOG", 1);

    bool passed = true;

    // Test exact word match
    if(trie.search("CAT") != 1) {
        passed = false;
    }

    // Test prefix
    if(trie.search("CA") != 2) {
        passed = false;
    }

    // Test full word
    if(trie.search("DOG") != 1) {
        passed = false;
    }

    // Test non-existent word
    if(trie.search("ZEBRA") != 0) {
        passed = false;
    }

    reportTest("Trie Basics", passed);
}

// Test Trie with multiple words
void testTrieMultipleWords() {
    Trie trie;
    trie.insert("WORD", 1);
    trie.insert("WORDS", 1);
    trie.insert("WOR", 2);
    trie.insert("WORK", 1);

    bool passed = true;

    if(trie.search("WORD") != 1) passed = false;
    if(trie.search("WORDS") != 1) passed = false;
    if(trie.search("WOR") != 2) passed = false;
    if(trie.search("WORK") != 1) passed = false;
    if(trie.search("W") != 2) passed = false; // Should be prefix
    if(trie.search("WORKING") != 0) passed = false; // Should not exist

    reportTest("Trie Multiple Words", passed);
}

// Test WordSolution structure
void testWordSolution() {
    vector<Position> path;
    path.push_back(Position(0, 0));
    path.push_back(Position(1, 0));
    path.push_back(Position(2, 0));

    WordSolution sol("CAT", path);

    bool passed = (sol.word == "CAT" && sol.path.size() == 3);

    reportTest("WordSolution Structure", passed);
}

// Test Position structure
void testPosition() {
    Position pos(2, 3);

    bool passed = (pos.x == 2 && pos.y == 3);

    reportTest("Position Structure", passed);
}

// Test findWordsWithPaths integration
void testFindWordsWithPaths() {
    Board b;
    string test_input = "CATSDOGSFISHBIRD";

    // Populate board
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            b.board[x][y] = test_input[y * 4 + x];
            b.board_available[x][y] = true;
        }
    }

    // Find words (this will initialize the Trie internally)
    vector<WordSolution> solutions = findWordsWithPaths(b);

    bool passed = true;

    // Check that we found some words
    if(solutions.empty()) {
        passed = false;
        cout << "  No solutions found!" << endl;
    }

    // Check that solutions have paths
    for(const auto& sol : solutions) {
        if(sol.path.empty()) {
            passed = false;
            cout << "  Solution '" << sol.word << "' has empty path!" << endl;
            break;
        }

        // Verify path length matches word length
        if(sol.word.length() != sol.path.size()) {
            passed = false;
            cout << "  Solution '" << sol.word << "' path length mismatch!" << endl;
            break;
        }
    }

    if(passed) {
        cout << "  Found " << solutions.size() << " words with valid paths" << endl;
    }

    reportTest("Find Words With Paths", passed);
}

// Test that paths are valid (within bounds and connected)
void testPathValidity() {
    Board b;
    string test_input = "CATSDOGSFISHBIRD";

    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            b.board[x][y] = test_input[y * 4 + x];
            b.board_available[x][y] = true;
        }
    }

    vector<WordSolution> solutions = findWordsWithPaths(b);

    bool passed = true;

    for(const auto& sol : solutions) {
        // Check each position is in bounds
        for(const auto& pos : sol.path) {
            if(pos.x < 0 || pos.x >= 4 || pos.y < 0 || pos.y >= 4) {
                passed = false;
                cout << "  Solution '" << sol.word << "' has out-of-bounds position!" << endl;
                break;
            }
        }

        // Check consecutive positions are adjacent
        for(size_t i = 1; i < sol.path.size(); i++) {
            int dx = abs(sol.path[i].x - sol.path[i-1].x);
            int dy = abs(sol.path[i].y - sol.path[i-1].y);

            // Adjacent means difference of at most 1 in both x and y
            if(dx > 1 || dy > 1) {
                passed = false;
                cout << "  Solution '" << sol.word << "' has non-adjacent positions!" << endl;
                break;
            }
        }

        if(!passed) break;
    }

    reportTest("Path Validity", passed);
}

// Test board initialization
void testBoardInitialization() {
    Board b;
    bool passed = (b.height == 4 && b.width == 4);
    reportTest("Board Initialization", passed);
}

// Test that each path spells its own word on the board and reuses no cell
void testPathSpellsWord() {
    Board b;
    string test_input = "CATSDOGSFISHBIRD";

    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            b.board[x][y] = test_input[y * 4 + x];
            b.board_available[x][y] = true;
        }
    }

    vector<WordSolution> solutions = findWordsWithPaths(b);

    bool passed = !solutions.empty();

    for(size_t s = 0; s < solutions.size() && passed; s++) {
        const WordSolution& sol = solutions[s];

        for(size_t i = 0; i < sol.path.size(); i++) {
            // the letter under each step must match the word
            if(b.board[sol.path[i].x][sol.path[i].y] != sol.word[i]) {
                cout << "  '" << sol.word << "' step " << i
                     << " sits on '" << b.board[sol.path[i].x][sol.path[i].y]
                     << "', not '" << sol.word[i] << "'" << endl;
                passed = false;
                break;
            }

            // a tile may not be used twice in one word
            for(size_t j = 0; j < i; j++) {
                if(sol.path[i].x == sol.path[j].x && sol.path[i].y == sol.path[j].y) {
                    cout << "  '" << sol.word << "' reuses cell ("
                         << sol.path[i].x << "," << sol.path[i].y << ")" << endl;
                    passed = false;
                    break;
                }
            }
        }
    }

    reportTest("Path Spells Word", passed);
}

// Main test runner
int main() {
    cout << "Running Optimized Word Hunt Test Suite" << endl;
    cout << "=======================================" << endl << endl;

    testBoardInitialization();
    testPosition();
    testWordSolution();
    testTrieBasics();
    testTrieMultipleWords();
    testFindWordsWithPaths();
    testPathValidity();
    testPathSpellsWord();

    cout << endl << "=======================================" << endl;
    cout << "Test Results: " << tests_passed << " passed, " << tests_failed << " failed" << endl;
    cout << "=======================================" << endl;

    return (tests_failed == 0) ? 0 : 1;
}
