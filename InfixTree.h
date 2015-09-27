//
// Created by Hans Dulimarta on 9/10/15.
//

#ifndef WORDSEARCHGENERATOR_INFIXTREE_H
#define WORDSEARCHGENERATOR_INFIXTREE_H

#include <string>
#include <array>
#include <vector>
using namespace std;

class InfixTree {
private:
    class InfixNode {
    public:
        InfixNode ();
        array<InfixNode*,26> children;
        array<int,26> sharedCount;
        bool isEndOfWord, isLeaf;

    };

    InfixNode* root;

    /* the following function is recursive */
    bool _insert(InfixNode *&top, /* top of the current subtree */
                 const string &word, /* current whole word being inserted */
                 int l_pos, /* left end of the current subword */
                 bool is_whole, string) const; /* recursion depth */
    void _undo (InfixNode*& top, const string&, int) const;
    void printAll (const InfixNode *, vector<char>&) const;
    void _dump (const InfixNode*, vector<string>&) const;
public:
    InfixTree();
    bool insert (const string&); /* non-recursive that calls _insert */
    void printAll() const {
        vector<char> currentWord;
        printAll(root, currentWord);
    }
    void dump() const {
        vector<string> tmp;
        _dump(root, tmp);
    }
};


#endif //WORDSEARCHGENERATOR_INFIXTREE_H
