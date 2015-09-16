//
// Created by Hans Dulimarta on 9/10/15.
//

#ifndef WORDSEARCHGENERATOR_WORDPUZZLESOLVER_H
#define WORDSEARCHGENERATOR_WORDPUZZLESOLVER_H

#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

class WordPuzzleSolver {
public:
    WordPuzzleSolver(ifstream&);
    void solve();
private:
    vector<vector<char>> grid;
    map<int,set<string>> words_by_length;
};


#endif //WORDSEARCHGENERATOR_WORDPUZZLESOLVER_H
