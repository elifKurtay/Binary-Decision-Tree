#ifndef __DECISION_TREE_NODE_H
#define __DECISION_TREE_NODE_H
#include <string>
using namespace std;
/**
* Author : Elif Kurtay
* ID: 21803373
* Section : 1
* Assignment : 2
*/
typedef int TreeItemType;

class DecisionTreeNode {
public:
    DecisionTreeNode();
    DecisionTreeNode(const TreeItemType& nodeItem, bool isLeaf = false,
        DecisionTreeNode *l = NULL,
        DecisionTreeNode *r = NULL);
    ~DecisionTreeNode();
    int getItem() const;
private:
    bool leaf;
    int split;
    int classDecision;
    DecisionTreeNode *right;
    DecisionTreeNode *left;

    friend class DecisionTree;
};
#endif // __DECISION_TREE_NODE_H
