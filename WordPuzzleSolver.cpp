//
// Created by Hans Dulimarta on 9/10/15.
//

#include "WordPuzzleSolver.h"
#include <iostream>
#include <iomanip>
#include <map>

WordPuzzleSolver::WordPuzzleSolver(ifstream &fin) {
    int width, height;
    fin >> width >> height;
    grid.resize((unsigned long) height, vector<char>((unsigned long)width));
    for (int k = 0; k < height; k++)
        for (int m = 0; m < width; m++)
        {
            fin >> ws >> grid[k][m]; /* skip whitespace */
        }
    int num_words;
    fin >> num_words;
    for (int k = 0; k < num_words; k++)
    {
        string s;
        fin >> s;
        words_by_length[s.length()].insert(s);
        all_words.push_back(s);
    }
}

void WordPuzzleSolver::solve() {
    std::map<string,tuple<int,int,string>> result;
    for (auto& x : words_by_length) {
        const int N = x.first;
        auto& word_set = x.second;
        clog << word_set.size() << " words of length " << N << endl;
        for (int k = 0; k < grid.size(); k++) {
            for (int m = 0; m < grid[k].size() - N; m++) {
                string word;
                for (int c = 0; c < N; c++) {
                    word += grid[k][m+c];
                }
                if (word_set.find(word) != word_set.end()) {
//                    clog << word << " " << k << " " << m << " ACROSS" << endl;
                    word_set.erase(word);
                    result[word] = make_tuple(k, m, "ACROSS");
                }
            }
        }
        for (int k = 0; k < grid.size() - N; k++) {
            for (int m = 0; m < grid[k].size(); m++) {
                string word;
                for (int c = 0; c < N; c++) {
                    word += grid[k+c][m];
                }
                if (word_set.find(word) != word_set.end()) {
//                    clog << word << " " << k << " " << m << " DOWN" << endl;
                    result[word] = make_tuple(k, m, "DOWN");
                    word_set.erase(word);
                }
            }
        }
    }

    for (string& s : all_words) {
        auto x = result.find(s);
        if (x != result.end()) {
            tuple<int, int, string> &info = x->second;
            cout << s << " " << get<0>(info) << " " << get<1>(info) << " " << get<2>(info) << endl;
        }
        else
            cerr << "Word " << s << " is not in the grid?" << endl;
    }
}