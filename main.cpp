#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <random>
#include "WordPuzzleGenerator.h"
#include "WordPuzzleSolver.h"
#include "SuffixTree.h"

using namespace std;

const string DIR = "/Users/dulimarh/CLionProjects/WordSearch/";

/* In Linux/OSX, the following file is the user dictionary
 * one word per line. This program selects random words from
 * the file. In you attempt to run this on Windows, you
 * have to first create the dictionary yourself. */
const string DICTIONARY = "/usr/share/dict/words";

void doGenerate(int numWord, int grid_width, int grid_height,
                const string& pzl_name, const string& key_name)
{
    std::default_random_engine rnd_engine/* (time(0))*/;
    std::uniform_int_distribution<int> uniformInt (6, 10);
    std::uniform_real_distribution<float> uniformFloat;
    ofstream pzl_out(pzl_name);
    ofstream key_out(key_name);

    SuffixTree t;
    regex alpha_only("[a-z]{6,10}"); /* words with 6-10 characters */
    auto touppercase = [](const string& s) {
        string t;
        for (auto c : s)
            t += toupper(c);
        return t;
    };
    vector<vector<string>> vw(11);
    ifstream words(DICTIONARY);
    for_each (istream_iterator<string>(words), istream_iterator<string>(),
            /* the two local variables must be passed to the functor */
              [&alpha_only, &vw, touppercase](const string& w) {
                  if (regex_match (w, alpha_only))
                      vw[w.length()].push_back(touppercase(w));
              });

    int count = 0;
    /* Insert selected random words into the Infix tree */
    vector<string> all_words;
    set<string> picked;
    while (count < numWord) {
        int len = uniformInt(rnd_engine);
        int idx = uniformFloat(rnd_engine) * vw.at(len).size();
        auto result = picked.insert(vw.at(len)[idx]);
        if (!result.second) continue;
        if (t.insert(vw[len][idx])) {
            all_words.push_back(vw[len][idx]);
            count++;
        }
    }

    WordPuzzleGenerator g(grid_width, grid_height, all_words);
    if (g.generate()) {
        g.save(pzl_out, key_out);
        clog << "Puzzle generated successfully into " << pzl_name << " and " << key_name << endl;
    }
    else {
        clog << "Unable to generate puzzle, rerun with lower percentage" << endl;
    }
    pzl_out.close();
    key_out.close();
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        cerr << "Insufficient argument...." << endl;
        exit (1);
    }
    string task (argv[1]);
    /* command line arguments:
     -g
     -s height width
     -p percentage of board to fill with actual words [0-100]
     -z puzzle_file
     -k key_file
     */
    if (task == "-g") {
        if (argc >= 11) {
            int desired_width, desired_height, desired_percentage;
            string puzzle_filename, key_filename;
            for (int k = 2; k < 11; k++) {
                if (strcmp(argv[k], "-s") == 0) {
                    desired_height = atoi(argv[k + 1]);
                    desired_width = atoi(argv[k + 2]);
                    k += 2;
                }
                else if (strcmp(argv[k], "-p") == 0) {
                    desired_percentage = atoi(argv[k+1]);
                    k ++;
                }
                else if (strcmp(argv[k], "-z") == 0) {
                    puzzle_filename = string(argv[k+1]);
                    k++;
                }
                else if (strcmp(argv[k], "-k") == 0) {
                    key_filename = string(argv[k+1]);
                    k++;
                }
            }
            /* assumming 8 letters per word on average */
            int num_words = desired_percentage * desired_height * desired_width / 800;
            doGenerate(num_words, desired_width, desired_height, puzzle_filename, key_filename);
        }
        else {
            cerr << "Usage: " << argv[0] << " -g -s height width -p word_percentage "
                << "-z puzzle_file -k key_file" << endl;
        }
    }
    else {
        if (argc >= 3) {
            ifstream puzz_in(argv[2]);
//            doSolve(puzz_in);
            WordPuzzleSolver s(puzz_in);
            s.solve();
            puzz_in.close();
        }
        else {
            cerr << "Usage: " << argv[0] << " -s puzzle_file" << endl;
        }
    }

    return 0;
}
