#include "IndexSystem.h"
//handle insert cases and return line in which node inserted
int IndexSystem:: insert(int maxrow,int maxcolumn, vector<pair<int, int>> data , int** btree) {
    sort(data.begin(), data.end());
    for(int i=1;i<=maxrow;i++){

        if(btree[i][0] == -1){
            btree[i][0]=0;
            index.updateheader(btree,btree[i][1]);
            btree[i][1] = data[0].first;
            btree[i][2] = data[0].second;
            return i;
        }else if (btree[i][0] == 1){
            vector<int> temp = index.whereToGoFunction(btree,data[0].first,maxrow,maxcolumn);
            int whereToGoI = temp[0];
            bool emptyToInsertNode = customFunction.emptyToInsert(btree,whereToGoI,maxcolumn);
            if(emptyToInsertNode){
                vector<set<int>> mset = index.insertNotFull(btree,maxcolumn,temp,data);
                index.updateRootNormal(btree,maxcolumn,temp,mset[0]);
                return customFunction.findInSet(mset[1],0);
            }else {
                vector<pair<int,int>> ll = index.insertFullSplit(btree,maxcolumn,maxrow,temp,data);
                if(ll[0].first==-1){
                    return -1;
                }
                index.updateRoot(btree,ll[0],ll[1],maxrow,maxcolumn,temp,ll[2].first);
                return ll[3].first;
            }
        }else if(btree[i][0] == 0){
            vector<int> temp = index.whereToGoFunction(btree,data[0].first,maxrow,maxcolumn);
            int whereToGoI = temp[0];
            bool emptyToInsertNode = customFunction.emptyToInsert(btree,whereToGoI,maxcolumn);
            if(emptyToInsertNode){

                vector<set<int>> mset =  index.insertNotFull(btree,maxcolumn,temp,data);

                return customFunction.findInSet(mset[1],0);
            }else{
                vector<pair<int,int>> ll= index.insertFullSplit(btree,maxcolumn,maxrow,temp,data);
                if(ll[0].first==-1){
                    return -1;
                }
                index.updateRoot(btree,ll[0],ll[1],maxrow,maxcolumn,temp,ll[2].first);
                return ll[3].first;

            }
        }
    }
    return 0;

}
//Search If ID Exist or not & return its offset or -1 if not exist
int IndexSystem:: SearchItem(int ** btree,int value,int maxColumn,int maxRows){
    vector<int> temp = index.whereToGoFunction(btree,value,maxRows,maxColumn);
    for(int i=1;i<maxColumn;i+=2){
        if(btree[temp[0]][i] == value){
            return btree[temp[0]][i+1];
        }
    }
    return -1;
}
bool getIsNotInternalNode(int ** btree,int value,int maxColumn,int maxRows,vector<int> temp){
    bool isNotInternalNode = false;
    for(int i=1;i<maxColumn;i+=2){
        if(btree[temp[0]][i] == value){
            if(i + 2 <= maxRows){
                isNotInternalNode = btree[temp[0]][i + 2] == -1;
            }else{
                isNotInternalNode = true;
            }
        }
    }
    return isNotInternalNode;
}
int getNumberOfNodes(int ** btree,int maxColumn,int maxRows,int parentIndex){
    if(parentIndex == -1 || parentIndex == 0){
        return 0;
    }
    int numberOFNodes = 0;
    for(int i=1;i<maxColumn;i+=2){
        if(btree[parentIndex][i] == -1){
            break;
        }
        numberOFNodes += 1;

    }
    return numberOFNodes;
}
void simpleDeleteItem(int ** btree,int value,int maxColumn,int maxRows,int row,int col,int replaceWith = -1){
    for(int i = col;i < maxColumn - 3;i += 2){
        int nextValue = btree[row][i + 2];
        int nextOffset = btree[row][i + 3];
        btree[row][i] = nextValue;
        btree[row][i + 1] = nextOffset;
    }


}
vector<int> getParents(vector<int> whereToGo){
    vector<int> parentsIndices;
    for(int i=1;i<whereToGo.size();i+=2){
        parentsIndices.push_back(whereToGo[i]);
    }
    return parentsIndices;
}
void sortRow(int ** btree,int maxColumn,int maxRows,int i){
        vector<pair<int,int>> parentData;
        for(int j=1;j<maxColumn;j+=2){
            if(btree[i][j] == -1){
                continue;
            }
            parentData.push_back(pair<int,int> {btree[i][j],btree[i][j + 1]});
        } 
        sort(parentData.begin(),parentData.end(),[](const auto& a, const auto& b) {
        return a.first < b.first;
        });
        int parentDataIndex = 0;
        for(int j=1;j<maxColumn;j+=2){
            if(btree[i][j] == -1){
                break;
            }
            btree[i][j] = parentData[parentDataIndex].first;
            btree[i][j + 1] = parentData[parentDataIndex].second;
            parentDataIndex += 1;
        } 
}

void updateParents(int ** btree,int value,int maxColumn,int maxRows,vector<int> whereToGo,int newValue,bool isDelete = false,int newIndex = -1){
    vector<int> parentsIndices = getParents(whereToGo);

    for(auto i : parentsIndices){
        for(int j=1;j<maxColumn;j+=2){

            if(btree[i][j] == value){
                if(btree[btree[i][j + 1]][0] == -1 || isDelete){
                    simpleDeleteItem(btree,value,maxColumn,maxRows,i,j);
                }else{
                    btree[i][j] = newValue;
                    if(newIndex != -1){
                        btree[i][j + 1] = newIndex;
                    }

                }

            }
        } 
        sortRow(btree,maxColumn,maxRows,i);
            if(getNumberOfNodes(btree,maxColumn,maxRows,i) == 0){
                for(int j = 0;j < maxColumn;j++){
                    btree[i][j] = -1;
                }
            }

    }
}
pair<int,int> deleteNotInternalNode(int ** btree,int value,int maxColumn,int maxRows,int row,int col,int replaceWith = -1,int replaceWithOffset = -1 ){

    btree[row][col] = replaceWith;
    btree[row][col + 1] = replaceWithOffset;
    int prevOffset = btree[row][col - 1];
    if(prevOffset == -1 || col == 1){
        return pair<int,int> {-1,-1};
    }
    int prevValue = btree[row][col - 2];
    return pair<int,int> {prevValue,prevOffset};

    return pair<int,int> {-1,-1};
}
int getColumnIndex(int ** btree,int value,int maxColumn,int maxRows,vector<int> temp){
    for(int i=1;i<maxColumn;i+=2){
        if(btree[temp[0]][i] == value){
            return  i;
        }
    }
}
pair<int,int> getRightLeftSiblingsHelper(vector<int> whereToGo,int** btree,int maxColumns,int parentIndex){
    pair<int,int>  rightLeftSiblings;
    for(int i=2;i<maxColumns;i+=2){
        if(btree[parentIndex][i] == whereToGo[0]){
            int leftSibling = -1;
            int rightSibling = -1;
            if(i - 2 >= 2){
                leftSibling = btree[parentIndex][i - 2];
            }
            if(i + 2 <= maxColumns){
                rightSibling = btree[parentIndex][i + 2];
            }
            rightLeftSiblings.first = leftSibling;
            rightLeftSiblings.second = rightSibling;
        }
    }
    return rightLeftSiblings;
}

pair<int,int> getRightLeftSiblings(vector<int> whereToGo,int** btree,int maxColumns,int maxNumberInRow){
    pair<int,int> rightLeftSiblings = getRightLeftSiblingsHelper(whereToGo,btree,maxColumns,whereToGo[1]);
    if(rightLeftSiblings.first != -1 || rightLeftSiblings.second != -1){
        return rightLeftSiblings;
    }
    if(whereToGo.size() <= 2){
        return rightLeftSiblings;
    }
    int parentIndex = whereToGo[whereToGo.size() - 1];
    for(int i=1;i<maxColumns;i+=2){
        if(btree[parentIndex][i] == maxNumberInRow){
            int leftSibling = -1;
            int rightSibling = -1;
            if(i - 1 >= 2){
                leftSibling = btree[parentIndex][i - 1];
                int leftSiblingValue = btree[parentIndex][i - 2];
                while(btree[leftSibling][0] != 0){

                    for(int k = 1;k < maxColumns;k += 2){

                        if(btree[leftSibling][k] == leftSiblingValue){

                            leftSibling = btree[leftSibling][k + 1];
                            break;
                        }
                    }
                }
            }
            if(i + 3 <= maxColumns){
                rightSibling = btree[parentIndex][i + 3];
                int rightSiblingValue = btree[parentIndex][i + 2];
                if(rightSibling != -1){
                    while(btree[rightSibling][0] != 0){
                        for(int k = 1;k < maxColumns;k += 2){
                            if(btree[rightSibling][k] == rightSiblingValue){
                                rightSibling = btree[rightSibling][k + 1];
                                break;
                            }
                        }
                    }
                }
            }
            rightLeftSiblings.first = leftSibling;
            rightLeftSiblings.second = rightSibling;
        }
    }
    return rightLeftSiblings;
}
int underFlow =0;
bool getCanCauseUnderFlow(int numberOFNodes,int maxrows){
    int m = ceil(maxrows / 2);
    underFlow = ceil(m / 2);
    return numberOFNodes <= underFlow;
}
int getMinimumUnderFlowIndex(){
    return underFlow * 2;
}
pair<int,int> getGreatestNumberInNode(int ** btree,int maxColumn,int maxRows,int parentIndex){
    if(parentIndex == -1){
        return pair<int,int> {-1,-1};
    }
    int greatest = -1;
    int greatestOffset = -1;
    for(int i=1;i<maxColumn;i+=2){
        if(btree[parentIndex][i] > greatest){
            greatest = btree[parentIndex][i];
            greatestOffset = btree[parentIndex][i + 1];
        }

    }
    return pair<int,int> {greatest,greatestOffset};
}
int IndexSystem:: deleteItem(int ** btree,int value,int maxColumn,int maxRows){
    vector<int> temp = index.whereToGoFunction(btree,value,maxRows,maxColumn);
    int row = temp[0];
    int col = getColumnIndex(btree,value,maxColumn,maxRows,temp);
    bool isNotInternalNode = getIsNotInternalNode(btree,value,maxColumn,maxRows,temp);
    int numberOFNodes = getNumberOfNodes(btree,maxColumn,maxRows,row);
    bool canCauseUnderFlow = getCanCauseUnderFlow(numberOFNodes,maxRows) ;
    if(canCauseUnderFlow){
        pair<int,int> greatestNumberCurrentDataFirst = getGreatestNumberInNode(btree,maxColumn,maxRows,row);

        pair<int,int> rightLeftSiblingsIndex = getRightLeftSiblings(temp,btree,maxColumn,greatestNumberCurrentDataFirst.first);
        cout << rightLeftSiblingsIndex.first << " " << rightLeftSiblingsIndex.second << endl;
        int numberoFNodesRight = getNumberOfNodes(btree,maxColumn,maxRows,rightLeftSiblingsIndex.second);
        int numberoFNodesLeft = getNumberOfNodes(btree,maxColumn,maxRows,rightLeftSiblingsIndex.first);
        if(!getCanCauseUnderFlow(numberoFNodesRight,maxRows)){
            int lowestNumberRight = btree[rightLeftSiblingsIndex.second][1];
            int lowestNumberRightOffset = btree[rightLeftSiblingsIndex.second][2];
            pair<int,int> greatestNumberCurrentData = getGreatestNumberInNode(btree,maxColumn,maxRows,row);
            int greatestNumberCurrent = greatestNumberCurrentData.first;

            deleteItem(btree,lowestNumberRight,maxColumn,maxRows);
            if(isNotInternalNode){
                deleteNotInternalNode(btree,value,maxColumn,maxRows,row,col,lowestNumberRight);
                updateParents(btree,value,maxColumn,maxRows,temp,lowestNumberRight);
            }else{
                int underFlowIndex = getMinimumUnderFlowIndex();
                btree[row][underFlowIndex + 1] = lowestNumberRight;
                btree[row][underFlowIndex + 2] = lowestNumberRightOffset;
                simpleDeleteItem(btree,value,maxColumn,maxRows,row,col);
                updateParents(btree,greatestNumberCurrent,maxColumn,maxRows,temp,lowestNumberRight);
            }
        }
        else if(!(numberoFNodesLeft,maxRows)){
            pair<int,int> greatestNumberLeftData = getGreatestNumberInNode(btree,maxColumn,maxRows,rightLeftSiblingsIndex.first);
            int greatestNodeLeft = greatestNumberLeftData.first;

            deleteItem(btree,greatestNodeLeft,maxColumn,maxRows);
            if(!isNotInternalNode){
                btree[row][col] = greatestNumberLeftData.first;
                btree[row][col + 1] = greatestNumberLeftData.second;
                sortRow(btree,maxColumn,maxRows,row);
            }else{
                btree[row][col] = greatestNumberLeftData.first;
                btree[row][col + 1] = greatestNumberLeftData.second;
                sortRow(btree,maxColumn,maxRows,row);
                pair<int,int> greatestNumberCurrentData = getGreatestNumberInNode(btree,maxColumn,maxRows,row);
                int greatestNumberCurrent = greatestNumberCurrentData.first;
                updateParents(btree,value,maxColumn,maxRows,temp,greatestNumberCurrent);
            }
        }else{
            int underFlowIndex = getMinimumUnderFlowIndex();

            if(rightLeftSiblingsIndex.first != -1 && rightLeftSiblingsIndex.first != 0){
                pair<int,int> greatestNumberCurrentDataInitial = getGreatestNumberInNode(btree,maxColumn,maxRows,row);

                btree[row][col] = -1;
                btree[row][col + 1] = -1;
                pair<int,int> greatestNumberCurrentData = getGreatestNumberInNode(btree,maxColumn,maxRows,row);
                pair<int,int> greatestNumberLeftData = getGreatestNumberInNode(btree,maxColumn,maxRows,rightLeftSiblingsIndex.first);
                int greatestNodeLeft = greatestNumberLeftData.first;
                btree[rightLeftSiblingsIndex.first][underFlowIndex + 1] = greatestNumberCurrentData.first;
                btree[rightLeftSiblingsIndex.first][underFlowIndex + 2] = greatestNumberCurrentData.second;
                for(int i = 0;i < maxColumn;i++){
                    btree[row][i] = -1;
                }
                btree[0][1] = row;
                updateParents(btree,greatestNodeLeft,maxColumn,maxRows,index.whereToGoFunction(btree,greatestNodeLeft,maxRows,maxColumn),greatestNumberCurrentData.first,false,rightLeftSiblingsIndex.first);
                updateParents(btree,greatestNumberCurrentDataInitial.first,maxColumn,maxRows,temp,greatestNumberCurrentData.first,false,-1);                
            }else if(rightLeftSiblingsIndex.second != -1 && rightLeftSiblingsIndex.second != 0){
                pair<int,int> greatestNumberCurrentDataInitial = getGreatestNumberInNode(btree,maxColumn,maxRows,row);

                btree[row][col] = -1;
                btree[row][col + 1] = -1;
                pair<int,int> greatestNumberCurrentData = getGreatestNumberInNode(btree,maxColumn,maxRows,row);
                btree[rightLeftSiblingsIndex.second][underFlowIndex + 1] = greatestNumberCurrentData.first;
                btree[rightLeftSiblingsIndex.second][underFlowIndex + 2] = greatestNumberCurrentData.second;
                sortRow(btree,maxColumn,maxRows,rightLeftSiblingsIndex.second);
                for(int i = 0;i < maxColumn;i++){
                    btree[row][i] = -1;
                }
                btree[0][1] = row;
                updateParents(btree,greatestNumberCurrentDataInitial.first,maxColumn,maxRows,temp,-1,true,-1);
            }else{
                if(!isNotInternalNode){
                    simpleDeleteItem(btree,value,maxColumn,maxRows,row,col);
                }
                else{

                    pair<int,int> prevData = deleteNotInternalNode(btree,value,maxColumn,maxRows,row,col,-1,-1);
                    bool isDelete = getNumberOfNodes(btree,maxColumn,maxRows,row) == 0;
                    updateParents(btree,value,maxColumn,maxRows,temp,prevData.first,isDelete);
                }
                if(getNumberOfNodes(btree,maxColumn,maxRows,row) == 0){
                    for(int i = 0;i < maxColumn;i++){
                        btree[row][i] = -1;
                    }
                    int x = 1;
                    for(int l = 0;l < maxRows - 1;l += 1){
                        btree[l][1] = x;
                        x += 1;
                    }
                }


            }
        }
    }
    else if(!isNotInternalNode){
        simpleDeleteItem(btree,value,maxColumn,maxRows,row,col);
    }
    else{

        pair<int,int> prevData = deleteNotInternalNode(btree,value,maxColumn,maxRows,row,col);

        updateParents(btree,value,maxColumn,maxRows,temp,prevData.first);
    }

    return -1;
}

