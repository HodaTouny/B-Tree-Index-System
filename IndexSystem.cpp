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
