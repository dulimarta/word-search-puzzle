//
// Created by Hans Dulimarta on 9/10/15.
//

#include <iostream>
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
    for (k = N - 1; k > 0; k--) {
        sub = w.substr(k, N - k);
        cout << "Inserting suffix: " << sub << endl;
        if (_insert (root, w, k, N - 1, false, 0) == false)
            break;
    }
    if (k > 0) {
        /* fail to insert, we must rollback */
        return false;
    }
    /* insert the entire word */
    cout << "Inserting entire word: " << w << endl;
    return _insert (root, w, 0, N - 1, true, 0);
}

bool InfixTree::_insert(InfixNode*& top, const string &word, int L, int R, bool is_whole, int depth) {
    /* TODO the following if-statement may not be needed */
    if (L > R) return true; /* attempt to insert an "empty" string */
//    string sub = word.substr(L, R - L + 1);
//    cout << "Attempt to insert " << sub << " of " << word << endl;
    int child_idx = toupper(word[L]) - 'A';
    bool result;
    if (top == nullptr) {
        top = new InfixNode;
        cout << "Create a new node for " << word[L] << endl;
        top->used[child_idx] = true;
        _insert (top->children[child_idx], word, L + 1, R, is_whole, depth + 1);
        result = true; /* when the tree is expanding, we have no duplicate */
    }
    else {
        if (!top->end) {
            if (depth == word.length() - 1 && top->used[child_idx]) {
                cout << "FAILED: The current set contains a suffix of " << word << endl;
                result = false;
            }
            else {
                cout << "Descend to the existing node of " << word[L] << endl;
                top->used[child_idx] = true;
                result = _insert(top->children[child_idx], word, L + 1, R, is_whole, depth + 1);
            }
        }
        else {
            cout << "FAILED: Duplicate entry" << endl;
            result = false;
        }
    }
    if (is_whole && result != false) {
        top->end = depth == word.length() - 1;
        if (top->end)
            cout << "Node for " << word[L] << " is an end node of " << word << endl;
    }
    return result;
}