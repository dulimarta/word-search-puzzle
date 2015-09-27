//
// Created by Hans Dulimarta on 9/10/15.
//

#include "WordPuzzleSolver.h"
#include <iostream>
#include <iomanip>
#include <limits>

WordPuzzleSolver::WordPuzzleSolver(ifstream &fin) {
    int width, height;
    fin >> height >> width;
    grid.resize((unsigned long) height, vector<char>((unsigned long)width));
    for (int k = 0; k < height; k++)
        for (int m = 0; m < width; m++)
        {
            fin >> ws >> grid[k][m]; /* skip whitespace */
            first_letter_pos[grid[k][m]].push_back(make_pair(k,m));
        }
    int num_words;
    fin >> num_words;
    for (int k = 0; k < num_words; k++)
    {
        string s;
        fin >> s;
        all_words.push_back(s);

        /* build the maps */
        words_by_length[s.length()].insert(s);
        word_to_position[s] = k;
    }
}

void WordPuzzleSolver::solve() {
//    solve_with_slow_triple();
//    solve_with_fast_triple();
//    solve_with_quad();
    solve_own();
}

/** TRIPLE */
bool WordPuzzleSolver::check_triple_across (int start_r, int start_c, const string& word) const {
    const int L = word.size();
    if (start_c + L > grid[0].size()) return false;
    for (int s = 0; s < word.size(); s++)
        if (grid[start_r][start_c + s] != word[s])
            return false;
    return true;
}

bool WordPuzzleSolver::check_triple_down (int start_r, int start_c, const string& word) const {
    const int L = word.size();
    if (start_r + L > grid.size()) return false;
    for (int s = 0; s < word.size(); s++)
        if (grid[start_r + s][start_c] != word[s])
            return false;
    return true;
}

void WordPuzzleSolver::solve_with_slow_triple() const {
    for (auto& word : all_words) {
        const int N = word.length();

        for (int k = 0; k < grid.size(); k++)
        {
            for (int m = 0; m < grid[0].size(); m++) {
                if (check_triple_across (k, m, word)) {
                    cout << word << " " << k << " " << m << " ACROSS" << endl;
                }
                else if (check_triple_down (k, m, word)) {
                    cout << word << " " << k << " " << m << " DOWN" << endl;
                }
            }
        }
    }
}

void WordPuzzleSolver::solve_with_fast_triple() const {
    for (auto& word : all_words) {

        /* get all positions of the word's first letter */
        auto& positions = first_letter_pos.at(word[0]);
        for (auto coord : positions) {
            if (check_triple_across(coord.first, coord.second, word))
                cout << word << " "
                    << coord.first << " " << coord.second
                    << " ACROSS" << endl;
            else if (check_triple_down(coord.first, coord.second, word))
                cout << word << " "
                    << coord.first << " " << coord.second
                    << " DOWN" << endl;
        }
    }
}

//#undef SLOWER_SEARCH_FROM_ARRAY
void WordPuzzleSolver::check_quad_across (int start_row, int start_col, int
shortest, int longest,
vector<tuple<int,int,string>>& sol) const
{
    for (int len = shortest; len <= longest; len++) {
        if (start_col + len > grid[0].size()) return;
        string word (len, ' ');
        for (int k = 0; k < len; k++) {
            word[k] = grid[start_row][start_col + k];
        }

#if SLOWER_SEARCH_FROM_ARRAY
        /* std::find() replaces a traditional for-loop */
        auto where = find (all_words.begin(), all_words.end(), word);
        if (where != all_words.end()) {
            int pos = where - all_words.begin();
            sol[pos]  = make_tuple(start_row, start_col, "ACROSS");
            return;
        }
#else

        /* it is IMPORTANT to use REFERENCE on this_set to avoid
         * making a COPY of the set */
        auto& this_set = words_by_length.at(len);
        auto where =  this_set.find(word);
        if (where != this_set.end()) {
            sol[word_to_position.at(word)] = make_tuple(start_row,
                                                       start_col,
                                            "ACROSS");
            return;
        }
#endif
    }
    return;
}

void WordPuzzleSolver::check_quad_down (int start_row, int start_col,
                                        int shortest, int longest,
vector<tuple<int,int,string>>& sol) const
{
    for (int len = shortest; len <= longest; len++) {
        if (start_row + len > grid.size()) return;
        string word (len, ' ');
        for (int k = 0; k < len; k++) {
            word[k] = grid[start_row + k][start_col];
        }
#if SLOWER_SEARCH_FROM_ARRAY
        /* std::find() replaces a traditional for-loop */
        auto where = find (all_words.begin(), all_words.end(), word);
        if (where != all_words.end()) {
            /* calculate the integer position of the iterator */
            int pos = where - all_words.begin();
            sol[pos]  = make_tuple(start_row, start_col, "DOWN");
            return true;
        }

//        /* Use a traditional for-loop the above code is */
//        for (int i = 0; i < all_words.size(); ++i) {
//            if (word == all_words[i]) {
//                sol[i]  = make_tuple(start_row, start_col, "DOWN");
//                return;
//            }
//        }

#else
        /* it is IMPORTANT to use REFERENCE on this_set to avoid
         * making a COPY of the set */
        auto& this_set = words_by_length.at(len);
        auto where =  this_set.find(word);
        if (where != this_set.end()) {
            sol[word_to_position.at(word)] = make_tuple(start_row,
                                                       start_col,
                                            "DOWN");
            return;
        }
#endif
    }
    return;
}

void WordPuzzleSolver::solve_with_quad() const {
    int minLen = numeric_limits<int>::max();
    int maxLen = 0;
    for (auto& w : all_words) {
        if (minLen > w.size()) minLen = w.size();
        if (maxLen < w.size()) maxLen = w.size();
    }
    /* allocate another array to hold the solution */
    vector<tuple<int,int,string>> solution (all_words.size());
    for (int k = 0; k < grid.size(); k++) {
        for (int m = 0; m < grid[0].size(); m++) {
            /* we have to check BOTH direction */
            check_quad_across (k, m, minLen, maxLen, solution);
            check_quad_down (k, m, minLen, maxLen, solution);
        }
    }
    for (int k = 0; k < all_words.size(); k++) {
        auto& my_tuple = solution[k];
        cout << all_words[k] << " "
            << get<0>(my_tuple) << " "
            << get<1>(my_tuple) << " "
            << get<2>(my_tuple) << endl;
    }
}

void WordPuzzleSolver::solve_own() {
    std::map<string,tuple<int,int,string>> result;
    for (auto& x : words_by_length) {
        const int N = x.first;
        auto& word_set = x.second;
//        clog << word_set.size() << " words of length " << N << endl;
        for (int k = 0; k < grid.size(); k++) {
            for (int m = 0; m < grid[k].size() - N + 1; m++) {
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
        for (int k = 0; k < grid.size() - N + 1; k++) {
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
