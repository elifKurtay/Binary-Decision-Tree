#include "DecisionTreeNode.h"
/**
* Author : Elif Kurtay
* ID: 21803373
* Section : 1
* Assignment : 2
*/
// Default constructor
DecisionTreeNode::DecisionTreeNode() {
    leaf = 0;
    split = -1;
    classDecision = 0;
    right = NULL;
    left = NULL;
}
DecisionTreeNode::~DecisionTreeNode() {
    delete right;
    delete left;
    right = NULL;
    left = NULL;
}
DecisionTreeNode::DecisionTreeNode(const TreeItemType& nodeItem, bool isLeaf,
        DecisionTreeNode *l,
        DecisionTreeNode *r) {
    if(isLeaf)
    {
        classDecision = nodeItem;
        split = -1;
    } else {
        split = nodeItem;
        classDecision = 0;
    }
    leaf = isLeaf;
    left = l;
    right = r;
}


int DecisionTreeNode::getItem() const {
    if(leaf)
        return classDecision;
    return split;
}

