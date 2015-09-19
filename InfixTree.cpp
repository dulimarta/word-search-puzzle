//
// Created by Hans Dulimarta on 9/10/15.
//

#include <iostream>
#include <assert.h>
#include "InfixTree.h"

InfixTree::InfixNode::InfixNode() {
    isEndOfWord = false;
    isLeaf = false;

    /* require explicit initialization */
    fill(children.begin(), children.end(), nullptr);
    fill(sharedCount.begin(), sharedCount.end(), 0);
}

InfixTree::InfixTree() {
    root = nullptr;
}

bool InfixTree::insert(const string & w) {
//    cout << "*** INSERTING " << w << endl;
    const int N = w.length();
    /* insert all the prefixes, starting from the shortest one */
    int k;
    string sub;
    bool allInserted = true;
    for (k = N - 1; k >= 0; k--) {
        sub = w.substr(k, N - k);
//        cout << "Inserting suffix: " << sub << endl;
        if (_insert (root, w, k, k == 0) == false) {
            allInserted = false;
            break;
        }
    }
    if (k >= 0) {
        /* fail to insert, we must rollback all previous suffixes */
        for (int i = k; i < N; i++) {
            sub = w.substr(i, N - i);
//            clog << "Rolling back " << sub << endl;
            _undo (root, sub, 0);
//            clog << "Dump AFTER rollback " << sub << endl;
//            dump();
        }
        return false;
    }
    /* insert the entire word */
    return allInserted;
}

bool InfixTree::_insert(InfixNode*& top, const string &word, int L, bool is_whole) const {
    bool result;
    if (top == nullptr) {
        top = new InfixNode;
        if (L < word.length()) {
            /* new node for a letter */
//            cout << "Create a new letter node for " << word[L] << endl;
            int child_idx = toupper(word[L]) - 'A';
            top->isLeaf = false;
            top->sharedCount[child_idx] = 1;
            _insert (top->children[child_idx], word, L + 1, is_whole);
        }
        else {
            /* new leaf node */
//            cout << "Create a new leaf node" << endl;
            top->isLeaf = true;
            top->isEndOfWord = is_whole;
//            if (is_whole) {
//                cout << "end of word " << word << " here" << endl;
//            }
        }
        result = true; /* when the tree is expanding, we have no duplicate */
    }
    else {
        if (!top->isEndOfWord) {
            if (L < word.length()) {
                int child_idx = toupper(word[L]) - 'A';
                top->sharedCount[child_idx]++;
                top->isLeaf = false;
//                cout << "Descend to " << word[L] << " share count is " <<
//                        top->sharedCount[child_idx] << endl;
                result = _insert(top->children[child_idx], word, L + 1, is_whole);
            }
            else {
                /* TODO: do we need something else here? */
//                cout << "WHAT SHOULD WE DO HERE????" << endl;
                result = !is_whole;
            }
        }
        else {
            cerr << "FAILED: Duplicate entry" << endl;
            result = false;
        }
    }
    return result;
}

void InfixTree::_undo(InfixNode *&top, const string& word, int depth) const {
    if (top == nullptr) return;
    if (depth >= word.length()) return;
    if (top->isEndOfWord) return;
    int idx = (int) (toupper(word[depth]) - 'A');
    _undo(top->children[idx], word, depth + 1);
    if (--top->sharedCount[idx] == 0) {
//        clog << "Must prune the node of " << (char) ('A' + idx) << endl;
        assert (top->children[idx]);
        delete top->children[idx];
        top->children[idx] = nullptr;
        bool allNull = true;
        for (int k = 0; k < top->children.size(); k++)
            if (top->children[k] != nullptr) {
                allNull = false;
                break;
            }
        if (allNull) {
//            clog << "Leaf adjustment required" << endl;
            top->isLeaf = true;
        }
    }
//    else {
//        clog << "Decrement share count of letter " << word[depth] << " to "
//        << top->sharedCount[idx] << endl;
//    }
}

void InfixTree::_dump(const InfixNode *top, vector<string>& path) const {
#if 0
    if (top != nullptr) {
        if (top->isLeaf) {
            for (auto s : path)
                clog << s;
            clog << endl;
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
#endif
}

void InfixTree::printAll(const InfixNode *top, vector<char>& word) const {
    if (top == nullptr)
        return;
    if (top->isEndOfWord) { /* no more children when you hit an END node */
        for (char c : word)
            clog << c;
        clog << endl;
        return;
    }
    if (top->isLeaf)
        return;
    for (int k = 0; k < top->children.size(); k++)
    {
        if (top->children[k]) {
            word.push_back ((char) ('A' + k));
            printAll (top->children[k], word);
            word.pop_back();
        }
    }
}