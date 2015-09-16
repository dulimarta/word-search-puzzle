//
// Created by Hans Dulimarta on 9/8/15.
//

#ifndef WORDSEARCHGENERATOR_WORDPUZZLEGENERATOR_H
#define WORDSEARCHGENERATOR_WORDPUZZLEGENERATOR_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <random>
#include <tuple>
#include <fstream>
#include <unordered_set>

using namespace std;

class WordPuzzleGenerator {
public:
    WordPuzzleGenerator(int width, int height, const vector<string>& words);
    void printPuzzle() const;
    bool generate();
    void save (ofstream&, ofstream&) const;

private:
    const char EMPTY = '.';
    enum Direction{HORIZONTAL, VERTICAL};
    vector<int> shared_letters_positions(const string &w) const;
    bool place_word_isolated (const string& w, Direction d);
    bool place_word_connected (const string& w, Direction d, vector<int>&);
    bool has_room_for_word(const string&, int, int, Direction) const;
    void insert_word_into_grid(const string &, int, int, Direction);
    void remove_word_from_grid(const string &w);
    void fill_in_noise_letters();
private:
    int grid_width, grid_height;
    default_random_engine gen;
    uniform_real_distribution<float> uni;
    vector<vector<char>> grid;
    vector<vector<int>> multiplicity;
    vector<string> all_words, placed_words;
    map<char,set<pair<int,int>>> letter_to_cells;
    map<string,tuple<int,int,Direction>> word_to_grid;
};


#endif //WORDSEARCHGENERATOR_WORDPUZZLEGENERATOR_H
