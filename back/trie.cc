#include "trie.h"
#include <fstream>
#include <iostream>

using namespace std;

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    delete root;
}

void Trie::insert(const string& word, int status) {
    TrieNode* current = root;

    // Insert all characters
    for(char c : word) {
        if(current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];

        // Mark intermediate nodes as prefixes (2) if not already marked as words
        if(current->wordStatus == 0) {
            current->wordStatus = 2;
        }
    }

    // Mark the final node with the actual status
    current->wordStatus = status;
}

int Trie::search(const string& word) const {
    TrieNode* current = root;

    for(char c : word) {
        if(current->children.find(c) == current->children.end()) {
            return 0; // Not found, not even a prefix
        }
        current = current->children[c];
    }

    return current->wordStatus;
}

void Trie::loadFromFile(const string& filename) {
    ifstream file(filename);

    if(!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return;
    }

    string line;

    // each line is a word marked '!' for complete or '.' for a prefix
    while(getline(file, line)) {
        if(line.empty()) continue;

        int status = (line.back() == '!') ? 1 : 2;
        insert(line.substr(0, line.length() - 1), status);
    }

    file.close();
}
