#ifndef DEMOBTREE_INDEXINTERFACE_H
#define DEMOBTREE_INDEXINTERFACE_H
#include "IndexSystem.h"
#include "IndexHelper.h"
#include "CustomFunctions.h"

class IndexInterface {
    int** Btree;
    int MaxColumn;
    int MaxRecords;
    IndexSystem indexSystem;
    IndexHelper indexHelper;
    CustomFunctions customFunctions;
    char fileName[20] = "Index.txt";


    void InsertHandler();
    void SearchHandler();
public:
    IndexInterface(int maxColumn,int maxRows);
    ~IndexInterface();

    void MainMenu();

    void deleteHandler();
};


#endif //DEMOBTREE_INDEXINTERFACE_H
