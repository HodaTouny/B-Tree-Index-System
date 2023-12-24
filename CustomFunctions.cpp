#include "CustomFunctions.h"


//when split first line need to shift the children one record down to add root
void CustomFunctions:: shiftDown(int **btree ,int cols, int numRows, int rowToShift) {
    for (int i = numRows - 1; i > rowToShift; --i) {
        for (int j = 0; j < cols; ++j) {

            btree[i][j] = btree[i - 1][j];
        }
    }
    btree[rowToShift][0] = 1;
    for (int j = 1; j < cols; ++j) {
        btree[rowToShift][j] = -1;
    }
    int value = btree[0][1]+2;
    int x = value+1;
    for(int i=value;i<numRows-1;i++){
        btree[i][1] = x;
        x++;
    }
    for(int j = 0; j < cols; ++j){
        btree[numRows-1][j]=-1;
    }
}

//find position of the first negative column in specific line
int CustomFunctions:: findFirstNegativePosition(int **btree,int whereToGoI,int MaxColumn){
    int value =-1;
    for(int i=1;i<MaxColumn;i+=2){
        if(btree[whereToGoI][i] == -1){
            value = i;
            break;
        }
    }
    return value;
}

//print 2d array in console
void CustomFunctions:: print(int**matrix,int MAX_ROWS,int MAX_COLS){
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

//boolean if i can insert value in this row or i need to split
bool CustomFunctions:: emptyToInsert(int **btree, int goI,int maxColumn) {
    if (findFirstNegativePosition(btree, goI, maxColumn) != -1) {
        return true;
    }
    return false;
}

//initialize the 2d array with shape like the assigment [no need but required]
int** CustomFunctions::CreateIndexArray(int numberOfRecords, int m) {
    int** array = (int**)malloc(numberOfRecords * sizeof(int*));
    for (int i = 0; i < numberOfRecords; i++) {
        array[i] = (int*)malloc(m * sizeof(int));

        for (int j = 0; j < m; j++) {
            if (j == 1 && i != numberOfRecords - 1) {
                array[i][j] = i + 1;
            } else {
                array[i][j] = -1;
            }
        }
    }
    return array;
}

//free the 2d array inorder to not to make memory leak
void CustomFunctions::freeIndexArray(int** array, int numberOfRecords) {
    for (int i = 0; i < numberOfRecords; i++) {
        free(array[i]);
    }
    free(array);
}

//return item in specific position in the set
int CustomFunctions::findInSet(const set<int >& mySet, size_t position) {
    if (position < mySet.size()) {
        auto it = mySet.begin();
        std::advance(it, position);
        return *it;
    } else {
        return int{};
    }
}

