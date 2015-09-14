#include <iostream>
#include <fstream>
#define GENERATE
#ifdef GENERATE
#include <vector>
#include "WordPuzzleGenerator.h"
#else
#include "WordPuzzleSolver.h"
#endif
#include "InfixTree.h"

using namespace std;

const string DIR = "/Users/dulimarh/CLionProjects/WordSearch/";

int main() {
    InfixTree t;

    for (auto x : {"clean", "cleanse",  "distribution", "clever", /*"downtown",*/
        "bread", "breed", "clearly", "tribute"}) {
        cout << t.insert(x) << endl;
        t.dump();
    }
    t.printAll();
}

int main0(int argc, char*argv[]) {
#ifdef GENERATE
    ifstream word_files (DIR + "words.txt");
    vector<string> all_words;

    cout << "Is the file open? " << word_files.is_open() << endl;
    int width, height;
    word_files >> height >> width;

    string w;
    while (word_files >> w) {
        all_words.push_back(w);
        cout << w << endl;
    }

    WordPuzzleGenerator g(width, height, all_words);
    g.solve_it();
    ofstream out (DIR + "ws_out.txt");
    g.save(out);
    out.close();
#else
    ifstream puzzle (DIR + "ws_out.txt");
    WordPuzzleSolver s(puzzle);
    puzzle.close();
#endif
    return 0;
}