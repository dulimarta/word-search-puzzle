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
        if (_insert (root, w, k, false, 0) == false)
            break;
    }
    if (k > 0) {
        /* fail to insert, we must rollback */
        return false;
    }
    /* insert the entire word */
    cout << "Inserting entire word: " << w << endl;
    return _insert (root, w, 0, true, 0);
}

bool InfixTree::_insert(InfixNode*& top, const string &word, int L, bool is_whole, int depth) {
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
        top->used[child_idx] = true;
        _insert (top->children[child_idx], word, L + 1, is_whole, depth + 1);
        result = true; /* when the tree is expanding, we have no duplicate */
    }
    else {
        if (!top->isEndOfWord) {
            if (depth == word.length() - 1 && top->used[child_idx]) {
                cout << "FAILED: The current set contains a suffix of " << word << endl;
                result = false;
            }
            else {
                cout << "Descend to the existing node of " << word[L] << endl;
                top->used[child_idx] = true;
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

void InfixTree::printAll(InfixNode *top) const {
    if (top == nullptr) {
        cout << "# ";
        return;
    }
    if (top->isEndOfWord) { /* no more children when you hit an END node */
        cout << "* ";
        return;
    }
    for (int k = 0; k < top->children.size(); k++)
    {
        if (top->children[k]) {
            cout << (char) ('A' + k);
            printAll (top->children[k]);
        }
    }
}