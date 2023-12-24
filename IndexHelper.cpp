#include "IndexHelper.h"

//split the node when rich to maximum and there is need to insert node in it
//return vector<pair<int,int>> values to update root
//pair 1,2:values to insert in root , pair 3: line in which value inserted
vector<pair<int, int>> IndexHelper:: insertFullSplit(int **btree, int maxcolumn, int maxrow,
                                       vector<int> whereToGo, vector<pair<int, int>> &data) {
    int value=data[0].first;
    int whereTo;
    whereTo=whereToGo[0];
    int m = ceil(maxrow / 2);
    bool found = false;
    int minimum = ceil(m / 2)+1;
    btree[whereTo][0] = -1;
    vector<pair<int, int>> newData;
    int newNodeValue = data[0].first;
    for (int i = 1; i < maxcolumn; i += 2) {
        data.push_back({btree[whereTo][i], btree[whereTo][i + 1]});
    }

    sort(data.begin(), data.end());
    for (int i = 0; i < minimum && i < data.size(); i++) {
        newData.push_back(data[i]);
    }
    data.erase(data.begin(), data.begin() + minimum);
    auto it = std::find_if(newData.begin(), newData.end(),
                           [value](const std::pair<int, int>& pair) {
                               return pair.first == value;
                           });
    int chooseChild;
    if (it != newData.end()) {
        chooseChild=1;
    } else {
        chooseChild=2;
    }
    int maximum = data[data.size()-1].first;
    if(maximum == newNodeValue){
        maximum = data[data.size()-2].first;
    }
    pair<int,int> FirstChild,SecondChild;
    for(int j=1;j<maxrow;j++){
        if(btree[j][0] == -1){
            if(j!=whereTo) {
                found = true;
            }
            if(!newData.empty()){
                int l=1;
                for(auto & i : newData){
                    btree[j][l] = i.first;
                    btree[j][l+1] = i.second;
                    l+=2;
                }
                for(int i = (newData.size()*2)+1;i<maxcolumn;i++){
                    btree[j][i]=-1;
                }
                btree[j][0] = 0;
                FirstChild = {newData[newData.size()-1].first,j};
                newData.erase(newData.begin(),newData.end());
            }else if(!data.empty()){
                int l=1;
                for(auto & i : data){
                    btree[j][l] = i.first;
                    btree[j][l+1] = i.second;
                    l+=2;
                }
                for(int i = (data.size()*2)+1;i<maxcolumn;i++){
                    btree[j][i]=-1;
                }
                btree[j][0] = 0;
                SecondChild = {data[data.size()-1].first,j};
                data.erase(data.begin(),data.end());
            }
        }
    }

    vector<pair<int,int>> toReturn;
    if(!found){
        toReturn.push_back({-1,-1});
        return toReturn;
    }

    if (whereTo==1){
        custom.shiftDown(btree,maxcolumn,maxrow,1);
        FirstChild.second++;
        SecondChild.second++;

    }

    toReturn.push_back(FirstChild); toReturn.push_back(SecondChild); toReturn.push_back({maximum,0});
    if(chooseChild==1){
        toReturn.push_back({FirstChild.second,-1});
    }
    else{
        toReturn.push_back({SecondChild.second,-1});
    }
    updateheader(btree,SecondChild.second+1);

    return toReturn;

}



//update root in case of split
void IndexHelper:: updateRoot(int** btree,pair<int,int>child1,pair<int,int>child2,int maxrow,int maxcolumn,vector<int>parent,int maximum){
    vector<pair<int,int>>Rootdata;

    for (int i = 1; i < maxcolumn; i += 2) {
        if(btree[parent[1]][i]==-1){ break;}
        Rootdata.push_back({btree[parent[1]][i], btree[parent[1]][i + 1]});
    }

    int value = maximum;
    Rootdata.erase(remove_if(Rootdata.begin(), Rootdata.end(), [value](const pair<int, int> &p) {
        return p.first == value;
    }), Rootdata.end());

    Rootdata.push_back(child1);
    Rootdata.push_back(child2);
    sort(Rootdata.begin(), Rootdata.end());
    if(custom.emptyToInsert(btree,parent[1],maxcolumn)) {
        int l = 1;
        for (int i = 0; i < Rootdata.size(); i++) {
            btree[parent[1]][l] = Rootdata[i].first;
            btree[parent[1]][l + 1] = Rootdata[i].second;
            l += 2;
        }
    }else{
        splitRoot(btree,maxcolumn,maxrow,Rootdata);
    }
}
//insert in the line if there is an existing space in it and return <vector<set>>
//set 1 : greatest value in the line to update root , set 2: line in which this child is inserted
vector<set<int>> IndexHelper:: insertNotFull(int** btree,int MaxColumn ,vector<int> whereToGoI,vector<pair<int, int>> data){
    int num = custom.findFirstNegativePosition(btree,whereToGoI[0],MaxColumn);
    int newNodeValue = btree[whereToGoI[0]][num-2];
    int insertIndex = -1;
    for (int i = 1; i < MaxColumn; i+=2) {
        if (btree[whereToGoI[0]][i] > data[0].first) {

            insertIndex = i;
            break;
        }
    }
    if(insertIndex != -1) {
        for (int i = MaxColumn - 1; i > insertIndex; --i) {
            btree[whereToGoI[0]][i] = btree[whereToGoI[0]][i - 2];
        }
        btree[whereToGoI[0]][insertIndex] = data[0].first;
        btree[whereToGoI[0]][insertIndex + 1] = data[0].second;
    }else{
        int whereToGoJ = custom.findFirstNegativePosition(btree, whereToGoI[0], MaxColumn);
        btree[whereToGoI[0]][whereToGoJ] = data[0].first;
        btree[whereToGoI[0]][whereToGoJ + 1] = data[0].second;
    }

    int maximum = data[0].first;
    vector<set<int>> firstvector;
    set<int> mset;
    mset.insert(maximum);
    mset.insert(newNodeValue);
    firstvector.push_back(mset);
    set<int> index;
    index.insert(whereToGoI[0]);
    firstvector.push_back(index);
    return firstvector;
}

//update root in case of notFull
void IndexHelper::updateRootNormal(int** btree, int maxcolumn,vector<int> whereToGoI,set<int> mset){
    if (mset.size()==1){
        return;
    }
    else{
        for(int j=1;j<whereToGoI.size();j+=2) {
            for (int i = 1; i < maxcolumn; i += 2) {
                if (btree[whereToGoI[j]][i] == custom.findInSet(mset, 0)) {
                    btree[whereToGoI[j]][i] = custom.findInSet(mset, 1);
                    break;
                }

            }
        }
    }

}

//when root is full split it into root and 2 parents [root childs & parent for other nodes]
void IndexHelper::splitRoot(int **btree, int maxcolumn, int maxrow,vector<pair<int, int>> &data){
    int m = ceil(maxrow / 2);
    int minimum = ceil(m / 2)+1;
    vector<pair<int,int>> newData;
    for (int i = 0; i < minimum && i < data.size(); i++) {
        newData.push_back(data[i]);
    }
    data.erase(data.begin(), data.begin() + minimum);
    pair<int,int> FirstChild,SecondChild;

    for(int j=1;j<maxrow;j++){
        if(btree[j][0] == -1){
            if(!newData.empty()){
                int l=1;
                for(auto & i : newData){
                    btree[j][l] = i.first;
                    btree[j][l+1] = i.second;
                    l+=2;
                }
                for(int i = (newData.size()*2)+1;i<maxcolumn;i++){
                    btree[j][i]=-1;
                }
                btree[j][0] = 1;
                FirstChild = {newData[newData.size()-1].first,j};
                newData.erase(newData.begin(),newData.end());
            }else if(!data.empty()){
                int l=1;
                for(auto & i : data){
                    btree[j][l] = i.first;
                    btree[j][l+1] = i.second;
                    l+=2;
                }
                for(int i = (data.size()*2)+1;i<maxcolumn;i++){
                    btree[j][i]=-1;
                }
                btree[j][0] = 1;
                SecondChild = {data[data.size()-1].first,j};
                data.erase(data.begin(),data.end());
            }
        }
    }

    if (SecondChild.second == maxrow-1) {
        updateheader(btree, -1);
    }
    else {
        updateheader(btree, SecondChild.second + 1);
    }



    btree[1][1] = FirstChild.first;
    btree[1][2] = FirstChild.second;
    btree[1][3] = SecondChild.first;
    btree[1][4] = SecondChild.second;

    for(int i = 5; i<=maxrow;i++){
        btree[1][i]=-1;
    }
}

//return the number of the line which The value exist in or must insert in, and it's parent/s index
// vector<in> -> [Line,Parent,Line,Parent]
vector<int> IndexHelper::whereToGoFunction(int** btree, int value, int maxRow, int maxColumn, int i) {
    vector<int> mydata;
    int index = 1;
    bool foundValue = false;
    int parentLine = 1;

    if (btree[i][0] == 1) {
        for (int j = 1; j < maxColumn; j += 2) {
            if (value <= btree[i][j]) {
                index = btree[i][j + 1];
                parentLine = i;
                foundValue = true;
                break;
            }
        }
        if (!foundValue) {
            int negativeOneIndex = custom.findFirstNegativePosition(btree, i, maxColumn);
            if (negativeOneIndex != -1) {
                index = btree[i][negativeOneIndex - 1];
                parentLine = i;
            } else {
                index = btree[i][maxColumn - 1];
                parentLine = i;
            }
        }
        if (btree[index][0] == 0) {
            mydata.push_back(index);
            mydata.push_back(parentLine);
        } else {
            mydata.push_back(index);
            mydata.push_back(parentLine);
            vector<int> childData = whereToGoFunction(btree, value, maxRow, maxColumn, index);
            mydata.insert(mydata.begin(), childData.begin(), childData.end());
        }
    }
    mydata.push_back(index);
    mydata.push_back(parentLine);

    return mydata;
}

//update First Line in the 2d array
void IndexHelper::updateheader(int** btree,int value) {
    btree[0][1]=value;
}


//Write my 2D array To the File
void IndexHelper:: WriteToFile(int** array, int numberOfRecords, int m, const char* filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file for writing: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < numberOfRecords; ++i) {
        for (int j = 0; j < m; ++j) {
            outputFile << array[i][j] << " ";
        }
        outputFile << endl;
    }

    outputFile.close();
}

//read data from file into 2D array
int** IndexHelper:: ReadFile(char* filename, int MAX_ROWS, int MAX_COLS) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening the file: " << filename << endl;
        return nullptr;
    }
    int** matrix = new int*[MAX_ROWS];
    for (int i = 0; i < MAX_ROWS; ++i) {
        matrix[i] = new int[MAX_COLS];
        for (int j = 0; j < MAX_COLS; ++j) {
            inputFile >> matrix[i][j];
        }
    }
    inputFile.close();
    return matrix;
}