#include "DecisionTree.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
using namespace std;
/**
* Author : Elif Kurtay
* ID: 21803373
* Section : 1
* Assignment : 2
*/
// Default constructor
DecisionTree::DecisionTree() : root(NULL) {

}
// Copy constructor
DecisionTree::DecisionTree(const DecisionTree& tree) {
		copyTree(tree.root, root);
}

bool DecisionTree::isEmpty() {
    return root == NULL;
}
// Constructor
DecisionTree::DecisionTree(const TreeItemType& rootItem) {
		root = new DecisionTreeNode(rootItem, false, NULL, NULL);
}
// Uses preorder traversal for the copy operation
// (Visits first the node and then the left and right children)
void DecisionTree::copyTree(DecisionTreeNode *treePtr, DecisionTreeNode *& newTreePtr) const {

		if (treePtr != NULL) {		// copy node
			newTreePtr = new DecisionTreeNode(treePtr->getItem(), NULL, NULL);
			copyTree(treePtr->left, newTreePtr->left);
			copyTree(treePtr->right, newTreePtr->right);
		}
		else
			newTreePtr = NULL;	// copy empty tree
}
// Destructor
DecisionTree::~DecisionTree() {
		destroyTree(root);
}

// Uses postorder traversal for the destroy operation
// (Visits first the left and right children and then the node)
void DecisionTree::destroyTree(DecisionTreeNode *& treePtr) {

		if (treePtr != NULL){
			destroyTree(treePtr->left);
			destroyTree(treePtr->right);
			delete treePtr;
			treePtr = NULL;
		}
}

DecisionTreeNode* DecisionTree::attachLeftOf(const TreeItemType& newItem, bool leaf, DecisionTreeNode* node) {
		// Assertion: nonempty tree; no left child
		if(node == NULL)
            node = root;
        if (!isEmpty() )
        {
            node->left = new DecisionTreeNode(newItem, leaf, NULL, NULL);
            if(leaf)
                return node;
            return node->left;
        }
        return NULL;
}
DecisionTreeNode* DecisionTree::attachRightOf(const TreeItemType& newItem, bool leaf, DecisionTreeNode* node) {
		// Assertion: nonempty tree; no left child
		if(node == NULL)
            node = root;
		if (!isEmpty() )
        {
            node->right = new DecisionTreeNode(newItem, leaf, NULL, NULL);
            if(leaf)
                return node;
            return node->right;
        }
        return NULL;
}

void DecisionTree::preorder(DecisionTreeNode *treePtr, int tab)
{
    if(treePtr != NULL)
    {
        for(int i = 0; i < tab; i++)
            cout<<"\t";
        if(treePtr->leaf)
            cout<<"class="<<(treePtr->getItem())<<endl;
        else
            cout<<(treePtr->getItem())<<endl;
        preorder(treePtr->left, tab+1);
        preorder(treePtr->right, tab+1);
    }
}

int DecisionTree::gain(const bool** data, const int* labels, const bool* usedSamples)
{
    double gain = 0.0, maxGain = 0.0;
    int featureId = 0;
    //select root
    for(int i = 0; i < numFeatures; i++) {
        gain = calculateInformationGain( data, labels, numSamples, numFeatures, usedSamples, i);
        if( gain > maxGain ) {
            featureId = i;
            maxGain = gain;
        }
    }
    return featureId;
}
void DecisionTree::get_split(const bool** data, const int* labels, const bool* used,
           const int id, int* s, int& lC, int& rC)
{
    rC = 0, lC = 0;
    for(int i = 0; i < numSamples; i++) {
        if(!used[i])
        {
            if(data[i][id])
            {
                s[i] = 1;
                rC++;
            }
            else
            {
                s[i] = -1;
                lC++;
            }
        }
    }
}

//for left s = -1, right = +1
bool isPure(const int* labels, const int* split, int s, int size, int& name)
{
    bool first = true;
    int prev = 0;
    for(int i = 0; i < size; i++)
    {
        if(split[i] == s)
        {
            if(first)
            {
                prev = labels[i];
                first = false;
            }
            else if(labels[i] != prev)
                return false;
        }

    }
    name = prev;
    return true;
}

void DecisionTree::split( DecisionTreeNode* curr, const bool** data, const int* labels, bool* usedSamples,
           const int feature)
{
    int sp[numSamples];
    for(int i = 0; i < numSamples; i++)
        sp[i] = 0;
    int lSize = 0, rSize = 0, id = 0;
    get_split(data, labels, usedSamples, feature, sp, lSize, rSize);
    bool end = true;;
    //is either empty?
    if(lSize == 0 || rSize == 0)
        return;
    // are all samples used?
    for(int i = 0; i < numSamples; i++)
        if(!usedSamples[i])
            end = false;
    if(end)
        return;
    //min size = 1
    // is left becoming a leaf?
    if( isPure(labels, sp, -1, numSamples, id))
    {
        curr = attachLeftOf(id, true, curr);
        for(int i = 0; i < numSamples; i++)
            if(sp[i] == -1)
                usedSamples[i] = true;

    } else {
        //update usedSamples to disregard right rows
        for(int i = 0; i < numSamples; i++)
            if(sp[i] == 1)
                usedSamples[i] = true;
        id = gain(data, labels, usedSamples);
        DecisionTreeNode* old = curr;
        curr= attachLeftOf(id, false, curr); //new feature node
        split( curr, data, labels, usedSamples, id);
        curr = old;
        for(int i = 0; i < numSamples; i++)
            if(sp[i] == 1)
                usedSamples[i] = false;
    }
    // is right becoming a leaf?
    if( isPure(labels, sp, +1, numSamples, id) )
    {
        curr = attachRightOf(id, true, curr);
        for(int i = 0; i < numSamples; i++)
            if(sp[i] == 1)
                usedSamples[i] = true;

    } else {
        //update usedSamples to disregard left rows
        for(int i = 0; i < numSamples; i++)
            if(sp[i] == -1)
                usedSamples[i] = true;
        id = gain(data, labels, usedSamples);
        curr = attachRightOf(id, false, curr); //new feature node
        split( curr, data, labels, usedSamples, id);
        for(int i = 0; i < numSamples; i++)
            if(sp[i] == -1)
                usedSamples[i] = false;
    }
}

void DecisionTree::train(const bool** data, const int* labels, const int numSamples, const int numFeatures)
{
    this->numSamples = numSamples;
    this->numFeatures = numFeatures;
    DecisionTreeNode* curr;
    bool* usedSamples = new bool[numSamples];
    //create the root
    int featureId = gain( data, labels, usedSamples);
    root = new DecisionTreeNode(featureId, false, NULL, NULL);
    curr = root;
    //recursively split
    split( curr, data, labels, usedSamples, featureId);
    return;
}

bool* split_input(string delimiter, string s, int len, int& label) {
    size_t pos = 0;
    int i = 0;
    int t_int = 0;
    bool* arr = new bool[len];
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        if(token == "1")
            arr[i] = true;
        else
            arr[i] = false;
        s.erase(0, pos + delimiter.length());
        i++;
    }
    stringstream myint(s);
    myint>> t_int;
    label = t_int;
    return arr;
}

void DecisionTree::train(const string fileName, const int numSamples,const int numFeatures)
{
    this->numSamples = numSamples;
    this->numFeatures = numFeatures;
    bool** data = new bool*[numSamples];
    int* labels = new int[numSamples];
    int label = 0;
    string info;
    ifstream file(fileName.c_str());
    for(int i = 0; i < numSamples; i++){
        getline(file,info);
        data[i] = split_input(" ", info, numFeatures, label);
        labels[i] = label;
    }
    file.close();
    train((const bool**) data, labels, numSamples, numFeatures);
    return;
}
int DecisionTree::get_answer(DecisionTreeNode* treePtr, const bool* data) const
{

    if(treePtr == NULL)
        return -1;
    if(treePtr->leaf)
        return treePtr->getItem();

    if(!data[treePtr->getItem()])
        get_answer(treePtr->left, data);
    else
        get_answer(treePtr->right, data);
}
int DecisionTree::predict(const bool* data)
{
    if(root == NULL)
        return -1;
    return get_answer(root, data);
}
double DecisionTree::test(const bool** data, const int* labels, const int numSamples)
{
    int result;
    int correct = 0;
    for(int i = 0; i < numSamples; i++)
    {
        result = predict(data[i]);
        if(result == labels[i])
            correct++;
    }
    return correct/(double) numSamples;
}
double DecisionTree::test(const string fileName, const int numSamples)
{
    bool** data = new bool*[numSamples];
    int* labels = new int[numSamples];
    int label = 0;
    string info;
    //FILE *file = fopen(fileName, "r");
    ifstream file(fileName.c_str());
    for(int i = 0; i < numSamples; i++){
        getline(file,info);
        data[i] = split_input(" ", info, numFeatures, label);
        labels[i] = label;
    }
    file.close();
    return test((const bool**) data, labels, numSamples);
}
void DecisionTree::print()
{
    preorder(root, 0);
}


double calculateInformationGain(const bool** data, const int* labels,
                                const int numSamples, const int numFeatures, const bool* usedSamples,
                                const int featureId)
{
    //higher gain is better
    if(numFeatures< featureId || featureId < 0)
        return 0.0;

    double total = 0.0;
    int left[numSamples];
    int right[numSamples];
    int parent[numSamples];
    int leftC = 0, rightC = 0;
    int pC = 0;
    for( int i = 0; i < numSamples; i++) {
        if(!usedSamples[i]) {
            if(data[i][featureId]) {
                right[rightC] = labels[i];
                rightC++;
            } else {
                left[leftC] = labels[i];
                leftC++;
            }
            parent[pC] = labels[i];
            pC++;
        }
    }
    double p = calculateEntropy(parent, pC);
    double l = calculateEntropy(left, leftC);
    double r = calculateEntropy(right, rightC);
    double tC = pC;
    total = leftC/ tC * l + rightC / tC * r;
    return p - total;
}

double calculateEntropy(const int* classCounts, const int numClasses) {
    double h = 0.0;
    double sum = 0.0;
    for(int i = 0; i < numClasses; i++) {
        sum += classCounts[i];
    }
    for(int i = 0; i < numClasses; i++) {
        if(classCounts[i]== 0)
            continue;
        h -= (classCounts[i] /sum) * (log2(classCounts[i]/sum)) ;
    }
    return h;
}

