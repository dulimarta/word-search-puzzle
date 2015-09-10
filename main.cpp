#include <iostream>
#include <fstream>
#include <vector>
#include "WordPuzzleGenerator.h"

using namespace std;

const string DIR = "/Users/dulimarh/CLionProjects/WordSearchGenerator/";

int main(int argc, char*argv[]) {
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
    g.printPuzzle();

    return 0;
}