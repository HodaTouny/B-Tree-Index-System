#ifndef DEMOBTREE_INDEXSYSTEM_H
#define DEMOBTREE_INDEXSYSTEM_H
#include "bits/stdc++.h"
#include "CustomFunctions.h"
#include "IndexHelper.h"
using namespace std;


class IndexSystem {
public:
    IndexHelper index;
    CustomFunctions customFunction;
    int insert(int maxrow,int maxcolumn, vector<pair<int, int>> data , int** btree);
    int SearchItem(int ** btree,int value,int maxColumn,int maxRows);
    int deleteItem(int ** btree,int value,int maxColumn,int maxRows);
};


#endif //DEMOBTREE_INDEXSYSTEM_H
