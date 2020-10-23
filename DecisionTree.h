#ifndef __DECISION_TREE_H
#define __DECISION_TREE_H
#include <string>
#include "DecisionTreeNode.h"
using namespace std;
/**
* Author : Elif Kurtay
* ID: 21803373
* Section : 1
* Assignment : 2
*/

double calculateEntropy(const int* classCounts, const int numClasses);
double calculateInformationGain(const bool** data, const int* labels,
                                const int numSamples, const int numFeatures, const bool* usedSamples,
                                const int featureId);

class DecisionTree {
private:
    DecisionTreeNode *root;
    int numSamples;
    int numFeatures;
    void destroyTree(DecisionTreeNode * &treePtr);
    void copyTree(DecisionTreeNode *treePtr, DecisionTreeNode* & newTreePtr) const;
    void preorder(DecisionTreeNode *treePtr, int tab);
    void split( DecisionTreeNode* curr, const bool** data, const int* labels, bool* usedSamples,
           const int feature);
    int get_answer(DecisionTreeNode *treePtr, const bool* data) const;
    int gain(const bool** data, const int* labels, const bool* usedSamples);
    void get_split(const bool** data, const int* labels, const bool* used,
           const int id, int* s, int& lC, int& rC);
public:
    void train(const bool**, const int*, const int, const int);
    void train(const string, const int, const int);
    int predict(const bool*);
    double test(const bool**, const int*, const int);
    double test(const string, const int);
    void print();
    //essential methods
    DecisionTree();
    DecisionTree(const TreeItemType& rootItem);
    DecisionTree(const DecisionTree& tree);
    ~DecisionTree();
    bool isEmpty();
    DecisionTreeNode* attachLeftOf(const TreeItemType& newItem, bool leaf, DecisionTreeNode* node = NULL);
    DecisionTreeNode* attachRightOf(const TreeItemType& newItem, bool leaf, DecisionTreeNode* node = NULL);
};


#endif // __DECISION_TREE_H
