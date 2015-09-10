//
// Created by Hans Dulimarta on 9/8/15.
//

#include "WordPuzzleGenerator.h"
#include <iostream>
#include <iomanip>

using namespace std;

WordPuzzleGenerator::WordPuzzleGenerator(int width, int height, const vector<string>& words) {
    grid_width = width;
    grid_height = height;
    grid.resize((unsigned long) height, vector<char>((unsigned long) width, EMPTY));
    multiplicity.resize((unsigned long)height, vector<int>((unsigned long) width, 0));
    for (string s : words)
        all_words.push(s);
}

vector<int> WordPuzzleGenerator::shared_letters_positions(const string &w) const {
    vector<int> cross;
    for (int k = 0; k < w.length(); k++)
    {
        const char& c = w[k];
        auto&& iter = letter_to_cells.find(c);
        /* if the k-th letter of the incoming word has a matching
         * letter on the grid */
        /* after a word removal, some map entries may be associated
         * with an EMPTY set, so make sure we check the set size below */
        if (iter != letter_to_cells.end() && iter->second.size() != 0)
            cross.push_back(k);
    }
    return cross;
}

bool WordPuzzleGenerator::has_room_for_word(const string& word, int r_start, int c_start, Direction d) const {
    int dr, dc;
    if (d == HORIZONTAL) {
        dr = 0;
        dc = +1;
    }
    else {
        dr = +1;
        dc = 0;
    }
    int r = r_start, c = c_start;
    for (int k = 0; k < word.length(); k++) {
        if (r >= grid_height || r < 0) return false;
        if (c >= grid_width || c < 0) return false;
        if (grid[r][c] != EMPTY && grid[r][c] != word[k]) return false;
        r += dr;
        c += dc;
    }
    return true;
}

void WordPuzzleGenerator::insert_word_into_grid(const string &w, int r, int c, Direction d) {
    word_to_grid[w] = make_tuple(r + 0, c, d); /* without + 0, this line was flagged as a compile error? */
    if (d == HORIZONTAL) {
        for (int k = 0; k < w.length(); k++) {
            grid[r][c + k] = w[k];
            multiplicity[r][c + k]++;
            letter_to_cells[w[k]].emplace(r + 0, c + k);
        }
    }
    else {
        for (int k = 0; k < w.length(); k++) {
            grid[r + k][c] = w[k];
            multiplicity[r + k][c]++;
            letter_to_cells[w[k]].emplace(r + k, c);
        }
    }

}

void WordPuzzleGenerator::remove_word_from_grid(const string &w) {
    tuple<int, int, Direction>& info = word_to_grid[w];
    int r = get<0>(info);
    int c = get<1>(info);
    Direction d = get<2>(info);

    if (d == HORIZONTAL) {
        for (int k = 0; k < w.length(); k++) {
            multiplicity[r][c+k]--;
            if (multiplicity[r][c+k] == 0) {
                grid[r][c + k] = EMPTY;
                auto& pos = letter_to_cells[w[k]];
                pos.erase(make_pair(r,c+k));
            }
        }
    }
    else {
        for (int k = 0; k < w.length(); k++) {
            multiplicity[r+k][c]--;
            if (multiplicity[r+k][c] == 0) {
                grid[r + k][c] = EMPTY;
                auto& pos = letter_to_cells[w[k]];
                pos.erase(make_pair(r+k,c));
            }
        }
    }
    word_to_grid.erase(w);
}

bool WordPuzzleGenerator::place_word_isolated(const string &w, Direction d) {
    int r, c;
    int max_attempts;
    max_attempts = grid_height * grid_width;
    if (d == HORIZONTAL) {
        while (max_attempts > 0) {
            r = (int) (uni(gen) * grid_height);
            c = (int) (uni(gen) * (grid_width - w.length()));
            if (has_room_for_word(w, r, c, d)) {
                insert_word_into_grid(w, r, c, d);
                return true;
            }
            max_attempts--;
        }
    }
    else {
        while (max_attempts > 0) {
            r = (int) (uni(gen) * (grid_height - w.length()));
            c = (int) (uni(gen) * grid_width);
            if (has_room_for_word(w, r, c, d)) {
                insert_word_into_grid(w, r, c, d);
                return true;
            }
            max_attempts--;
        }
    }
    return false;
}

bool WordPuzzleGenerator::place_word_connected(const string &w, Direction d, vector<int>& pos) {
    for (int p : pos) {
//        cout << "Attempt to match " << w[p] << " of " << w << " with the letters on the board" << endl;
        auto& hints = letter_to_cells.at(w[p]);
        for (auto ph : hints) {
//            cout << "Letter " << w[p] << " has been placed at (" << ph.first << ":" << ph.second << ")" << endl;
            int start_row, start_col;
            if (d == HORIZONTAL) {
                start_row = ph.first;
                start_col = ph.second - p;
            }
            else {
                start_row = ph.first - p;
                start_col = ph.second;
            }
            if (has_room_for_word(w, start_row, start_col, d)) {
                insert_word_into_grid(w, start_row, start_col, d);
                return true;
            }
        }
    }
    return false;
}

void WordPuzzleGenerator::solve_it() {
    Direction word_dir = HORIZONTAL;

    printPuzzle();
    bool placementSuccessful;
    while (all_words.size() > 0) {
        string current_word = all_words.top();
        vector<int>&& positions = shared_letters_positions(current_word);
        placementSuccessful = false;
        if (positions.size() > 0
            &&
                uni(gen) < 0.5f) { /* add a 50% chance of choosing between connected/isolated */
            /* attempt to place the word intersected with the words on the grid */
            if (place_word_connected(current_word, word_dir, positions)) {
                all_words.pop();
                placed_words.push(current_word);
                placementSuccessful = true;
            }
        }
        else {
            if (place_word_isolated(current_word, word_dir)) {
                all_words.pop();
                placed_words.push(current_word);
                placementSuccessful = true;
            }
        }
        if (!placementSuccessful) {
            /* do we backtrack here? */
            if (placed_words.size() > 0) {
                all_words.push(placed_words.top());
                remove_word_from_grid(placed_words.top());
                placed_words.pop();
            }
        }
        else {
            word_dir = Direction(1 - word_dir);
        }
        printPuzzle();
    }
    fill_in_noise_letters();
    for (auto& x : word_to_grid) {
        auto& tup = x.second;
        cout << setw(15) << left << x.first << " at " << right << setw(2) << get<0>(tup) << ","
            << setw(2) << get<1>(tup) << "  "
                << (get<2>(tup) == 0 ? "HORIZONTAL" : "VERTICAL") << endl;
    }
}

void WordPuzzleGenerator::printPuzzle() const {
    cout << "  ";
    for (int c = 0; c < grid_width; c++) {
        if (c % 10 || c == 0)
            cout << "  ";
        else
            cout << setw(2) << (c / 10);
    }
    cout << endl << "   ";
    for (int c = 0; c < grid_width; c++) {
        cout << (c % 10) << ' ';
    }
    cout << endl;
    for (int r = 0; r < grid_height; r++) {
        cout << setw(2) << r << ' ';
        for (int c = 0; c < grid_width; c++)
        {
            cout << grid[r][c] << ' ';
        }
        cout << endl;
    }
}

void WordPuzzleGenerator::fill_in_noise_letters() {
    for (int r = 0; r < grid_height; r++) {
        for (int c = 0; c < grid_width; c++)
        {
            if (grid[r][c] == EMPTY) {
                grid[r][c] = 'A' + (int) (uni(gen) * 26);
            }
        }
    }

}