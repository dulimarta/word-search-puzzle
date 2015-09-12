//
// Created by Hans Dulimarta on 9/10/15.
//

#ifndef WORDSEARCHGENERATOR_INFIXTREE_H
#define WORDSEARCHGENERATOR_INFIXTREE_H

#include <string>
#include <array>
using namespace std;

class InfixTree {
private:
    class InfixNode {
    public:
        InfixNode () : children{nullptr}, used{false} {
            isEndOfWord = false;
        }
        array<InfixNode*,26> children;
        array<bool,26> used;
        bool isEndOfWord, isLeaf;

    };

    InfixNode* root;

    /* the following function is recursive */
    bool _insert(InfixNode *&top, /* top of the current subtree */
                 const string &word, /* current whole word being inserted */
                 int l_pos, /* left end of the current subword */
                 bool is_whole,
                 int depth); /* recursion depth */
    void printAll (InfixNode *) const;
public:
    InfixTree();
    bool insert (const string&); /* non-recursive that calls _insert */
    void printAll() const { printAll(root); }
};


#endif //WORDSEARCHGENERATOR_INFIXTREE_H
