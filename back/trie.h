#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <unordered_map>

using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    int wordStatus; // 0 = not a word, 1 = is a word, 2 = is a prefix

    TrieNode() : wordStatus(0) {}

    ~TrieNode() {
        for(auto& pair : children) {
            delete pair.second;
        }
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie();
    ~Trie();
    void insert(const string& word, int status);
    int search(const string& word) const;
    void loadFromFile(const string& filename);
};

#endif // TRIE_H
