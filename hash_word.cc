#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//note: map coding
//0 is never a word
//1 is a word
//2 is a maybe word

/*
creates and returns a hashmap of valid an potentially valid words
*/
map<string,int> createMap(){
    map<string,int> m;
    ifstream r("pairs.txt");
    string t;
    int n;
    while(getline(r,t)){
        n = (t.at(t.length()-1) == '!') ? 1 : 2;
        t = t.substr(0, t.length()-1);
        m.insert(pair<string,int>(t,n));
    }
    return m;
}

/*
input: infile name and outfile name.
output: nothing
prints all words and almost-words to the file, formatted to be in a hashmap named "m".
should only be run every time english_words.txt is updated.
*/
//populatePairs("english_words.txt", "pairs.txt");
void populatePairs(string infile, string outfile){
    ifstream r(infile);
    ofstream output(outfile);
    string t;
    int size;
    map<string,int> m;
    vector<string> v;
    
    while(getline(r,t)){
        m.insert(pair<string,int>(t,1));
        v.push_back(t);
        output << t << "!" << endl;
    }

    for(int i=0; i<v.size(); i++){
        for(int j=v.at(i).length()-1; j>0; j--){
            size = m.size();
            m.insert(pair<string,int>(v.at(i).substr(0,j),2));
            if(size != m.size()) {m.insert(pair<string,int>(v.at(i).substr(0,j),2));
                output << v.at(i).substr(0,j) << "." << endl;
            }
        }
    }
}
