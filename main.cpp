#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <random>
#include "WordPuzzleGenerator.h"
#include "WordPuzzleSolver.h"
#include "InfixTree.h"

using namespace std;

const string DIR = "/Users/dulimarh/CLionProjects/WordSearch/";
const string DICTIONARY = "/usr/share/dict/words";

void doGenerate(int numWord, int grid_width, int grid_height,
                ofstream& pzl_out, ofstream& key_out)
{
    std::default_random_engine rnd_engine;
    std::uniform_int_distribution<int> uniformInt (6, 10);
    std::uniform_real_distribution<float> uniformFloat;

    InfixTree t;
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
              [&alpha_only, &vw](const string& w) {
                  if (regex_match (w, alpha_only))
                      vw[w.length()].push_back(w);
              });

    for (auto& v : vw) {
        cout << v.size() << endl;
    }
    int count = 0;
    vector<string> all_words;
    while (count < 50) {
        int len = uniformInt(rnd_engine);
        int idx = uniformFloat(rnd_engine) * vw[len].size();
        cout << vw[len][idx] << endl;
        if (t.insert(vw[len][idx])) {
            all_words.push_back(touppercase(vw[len][idx]));
            count++;
        }
    }
    t.printAll();

    WordPuzzleGenerator g(grid_width, grid_height, all_words);
    g.generate();
    g.save(pzl_out, key_out);
}

int main(int argc, char * argv[]) {

    if (argc < 2) {
        cerr << "Insufficient argument...." << endl;
        exit (1);
    }
    string task (argv[1]);
    if (task == "-g") {
        if (argc >= 7) {
            ofstream p_out(argv[5]);
            ofstream k_out(argv[6]);
            doGenerate(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), p_out, k_out);
            p_out.close();
            k_out.close();
        }
        else {
            cerr << "Generator: Insufficient argument" << endl;
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
            cerr << "Solver: Insufficient argument" << endl;
        }
    }
    return 0;
}
