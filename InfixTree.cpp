//
// Created by Hans Dulimarta on 9/10/15.
//

#include <iostream>
#include <sstream>
#include "InfixTree.h"

InfixTree::InfixTree() {
    root = nullptr;
}

bool InfixTree::insert(const string & w) {
    cout << "*** INSERTING " << w << endl;
    const int N = w.length();
    /* insert all the prefixes, starting from the shortest one */
    int k;
    string sub;
    bool allInserted = true;
    for (k = N - 1; k >= 0; k--) {
        sub = w.substr(k, N - k);
        cout << "Inserting suffix: " << sub << endl;
        if (_insert (root, w, k, k == 0, 0) == false) {
            allInserted = false;
            break;
        }
    }
    if (k >= 0) {
        /* fail to insert, we must rollback all previous suffixes */
        for (int i = k; i < N; i++) {
            sub = w.substr(i, N - i);
            cout << "Must rollback " << sub << endl;
            _undo (root, sub, 0);
            dump();
        }
        return false;
    }
    /* insert the entire word */
    return allInserted;
}

bool InfixTree::_insert(InfixNode*& top, const string &word, int L, bool is_whole, int depth) const {
    /* TODO the following if-statement may not be needed */
    if (L >= word.length()) {
        if (top == nullptr) {
            cout << "Creating a leaf node" << endl;
            top = new InfixNode;
            top->isEndOfWord = is_whole;
            if (is_whole) {
                cout << "end of word " << word << " here" << endl;
            }
            top->isLeaf = true;
        }
        else {
            if (top->isEndOfWord) {
                cout << "Duplicate due to shared suffix"<<endl;
                return false;
            }
        }
        return true;
    } /* attempt to insert an "empty" string */
    int child_idx = toupper(word[L]) - 'A';
    bool result;
    if (top == nullptr) {
        top = new InfixNode;
        top->isLeaf = false;
        cout << "Create a new node for " << word[L] << endl;
        top->sharedCount[child_idx] = 1;
        _insert (top->children[child_idx], word, L + 1, is_whole, depth + 1);
        result = true; /* when the tree is expanding, we have no duplicate */
    }
    else {
        if (!top->isEndOfWord) {
            if (depth == word.length() - 1 && top->sharedCount[child_idx] > 0) {
                cout << "FAILED: The current set contains a suffix of " << word << endl;
                result = false;
            }
            else {
                top->sharedCount[child_idx]++;
                top->isLeaf = false;
                cout << "Descend to " << word[L] << " share count is " <<
                        top->sharedCount[child_idx] << endl;
                result = _insert(top->children[child_idx], word, L + 1, is_whole, depth + 1);
            }
        }
        else {
            cout << "FAILED: Duplicate entry" << endl;
            result = false;
        }
    }
    return result;
}

void InfixTree::_undo(InfixNode *&top, const string & word, int depth) const {
    if (top != nullptr) {
        if (/*depth >= word.length() || */ top->isLeaf) {
            cout << "Attempt to remove a leaf?" << endl;
        }
        else {
            int idx = (int) (toupper(word[depth]) - 'A');
            _undo(top->children[idx], word, depth + 1);
            int &letter_count = top->sharedCount[idx];
            cout << "Must undo letter " << word[depth] << ", it was shared by "
            << letter_count << " words" << endl;
            letter_count--;
            if (letter_count == 0) {
                cout << "Must prune a node" << endl;
                delete top;
                top = nullptr;
            }
        }
    }
}

void InfixTree::_dump(InfixNode *top, vector<string>& path) const {
    if (top != nullptr) {
        if (top->isLeaf) {
            for (auto s : path)
                cout << s;
            cout << endl;
            return;
        }
        for (int k = 0; k < top->children.size(); k++)
        {
            if (top->children[k]) {
                ostringstream oss;
                oss << "(" << (char)('A'+k) << " " << top->sharedCount[k] << ")";
                path.push_back(oss.str());
                _dump(top->children[k], path);
                path.pop_back();
            }
        }
    }
}

void InfixTree::printAll(InfixNode *top, vector<char>& word) const {
    if (top == nullptr)
        return;
    if (top->isEndOfWord) { /* no more children when you hit an END node */
        for (char c : word)
            cout << c;
        cout << endl;
        return;
    }
    for (int k = 0; k < top->children.size(); k++)
    {
        if (top->children[k]) {
            word.push_back ((char) ('A' + k));
            printAll (top->children[k], word);
            word.pop_back();
        }
    }
}