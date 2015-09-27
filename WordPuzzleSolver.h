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
    vector<string> all_words;

    /* extra data structures to speed up search */
    /* the map will group the words by its length, i.e. each set in the
     * map hold words of the same length.
     */
    map<int,set<string>> words_by_length;

    /* to print the quad result in order, we associate each word with
     * its original position in the list */
    map<string,int> word_to_position;

    /* associate each letter with (r,c) positions on the grid,
     * (this data structure is used by fast_triple) */
    map<char,vector<pair<int,int>>> first_letter_pos;

    void solve_own();
    void solve_with_slow_triple() const;
    void solve_with_fast_triple() const;
    void solve_with_quad() const;
    bool check_triple_across(int,int,const string&) const;
    bool check_triple_down(int,int,const string&) const;

    void check_quad_across (int, int, int, int,
                            vector<tuple<int,int,string>>&) const;
    void check_quad_down (int, int, int, int,
                          vector<tuple<int,int,string>>&) const;
};


#endif //WORDSEARCHGENERATOR_WORDPUZZLESOLVER_H
