#ifndef DEMOBTREE_INDEXHELPER_H
#define DEMOBTREE_INDEXHELPER_H
#include "bits/stdc++.h"
#include "CustomFunctions.h"

using namespace std;

class IndexHelper {
public:
    CustomFunctions custom;
    vector<pair<int, int>>
    insertFullSplit(int **btree, int maxcolumn, int maxrow, vector<int> whereToGo, vector<pair<int, int>> &data);
    void updateRoot(int **btree, pair<int, int> child1, pair<int, int> child2, int maxrow, int maxcolumn, vector<int> parent,
               int maximum);
    vector<set<int>> insertNotFull(int **btree, int MaxColumn, vector<int> whereToGoI, vector<pair<int, int>> data);

    void splitRoot(int **btree, int maxcolumn, int maxrow, vector<pair<int, int>> &data);

    vector<int> whereToGoFunction(int **btree, int value, int maxRow, int maxColumn, int i=1);

    void updateheader(int **btree, int value);

    void updateRootNormal(int **btree, int maxcolumn, vector<int> whereToGoI, set<int> mset);
    int** ReadFile(char* filename, int MAX_ROWS, int MAX_COLS);
    void WriteToFile(int **array, int numberOfRecords, int m, const char *filename);
};


#endif //DEMOBTREE_INDEXHELPER_H
