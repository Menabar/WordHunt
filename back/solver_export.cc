#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include "wordhunt.h"

extern "C" {

// Takes a board string in row-major order, _width * _height characters long
// (e.g. "CATSDOGSFISHBIRD" for the default 4x4).
// Returns a malloc'd JSON string; caller must free() it.
char* solve_board(const char* letters) {
    Board board;

    for (int y = 0; y < board.height; y++) {
        for (int x = 0; x < board.width; x++) {
            char c = letters[y * board.width + x];
            board.board[x][y] = (char)toupper((unsigned char)c);
            board.board_available[x][y] = true;
        }
    }

    std::vector<WordSolution> solutions = findWordsWithPaths(board);

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < solutions.size(); i++) {
        if (i > 0) json << ",";
        json << "{\"word\":\"" << solutions[i].word << "\",\"path\":[";
        for (size_t j = 0; j < solutions[i].path.size(); j++) {
            if (j > 0) json << ",";
            json << "{\"x\":" << solutions[i].path[j].x
                 << ",\"y\":" << solutions[i].path[j].y << "}";
        }
        json << "]}";
    }
    json << "]";

    std::string result = json.str();
    char* buf = (char*)malloc(result.size() + 1);
    memcpy(buf, result.c_str(), result.size() + 1);
    return buf;
}

} // extern "C"
