#include <iostream>
#include <cmath>
#include "DecisionTree.h"
using namespace std;

int main() {
    DecisionTree t;
    int numFea = 21;
    t.train("train_data.txt",498, numFea);
    t.print();
    cout<<"test score: " <<t.test("test_data.txt", 473)<<endl;
    return 0;
}
