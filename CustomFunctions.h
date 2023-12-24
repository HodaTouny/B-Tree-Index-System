
#ifndef DEMOBTREE_CUSTOMFUNCTIONS_H
#define DEMOBTREE_CUSTOMFUNCTIONS_H
#include "bits/stdc++.h"
using namespace std;
class CustomFunctions {
public:
    void shiftDown(int **btree ,int cols, int numRows, int rowToShift);
    int findFirstNegativePosition(int **btree,int whereToGoI,int MaxColumn);
    void print(int**matrix,int MAX_ROWS,int MAX_COLS);
    bool emptyToInsert(int **btree, int goI,int maxColumn);
    int** CreateIndexArray(int numberOfRecords, int m);
    void freeIndexArray(int** array, int numberOfRecords);
    int findInSet(const set<int >& mySet, size_t position);


};


#endif //DEMOBTREE_CUSTOMFUNCTIONS_H
