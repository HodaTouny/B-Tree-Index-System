#include "IndexInterface.h"

void IndexInterface::InsertHandler(){
    int id,offset;
    cout<<"Enter The record To insert : (value Offset)\n";
    cin>>id>>offset;
    if(indexSystem.SearchItem(Btree,id,MaxColumn,MaxRecords) == -1) {
        vector<pair<int, int>> data;
        data.push_back({id,offset});
        int line = indexSystem.insert(MaxRecords,MaxColumn,data,Btree);
        cout<<"The ID : "<<id<<" is inserted at Line: "<<line<<"\n";
    }else{
        cout<<"Sorry This ID is Existing Before I cannot Insert It Again\n";
    }
}


void IndexInterface:: deleteHandler(){

}

void IndexInterface :: SearchHandler(){
    int id;
    cout<<"Enter The ID of The Needed Record :\n";
    cin>>id;
    int Answer = indexSystem.SearchItem(Btree,id,MaxColumn,MaxRecords);
    if(Answer != -1) {
       cout<<"Offset Of The Record is : "<<Answer<<"\n";
    }else{
        cout<<"I think You Enter ID please check It And Come Again\n";
    }
}

void IndexInterface::MainMenu() {
    cout << "Welcome to the Index Interface!\n";
    while (true) {
        cout << "Please choose an option:\n";
        cout << "1. Search on record\n";
        cout << "2. Insert new record\n";
        cout << "3. Delete record\n";
        cout << "4. Exit\n";
        int choice;
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        switch (choice) {
            case 1:
                SearchHandler();
                break;
            case 2:
                InsertHandler();
                break;
            case 3:
                deleteHandler();
                break;
            case 4:
                cout << "Thanks For Using Our System\n";
                return;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
                break;
        }
    }
}
IndexInterface:: IndexInterface(int maxColumn,int maxRows){
    Btree = indexHelper.ReadFile(fileName, maxRows, maxColumn);
    MaxColumn = maxColumn;
    MaxRecords = maxRows;
}

IndexInterface:: ~IndexInterface(){
    indexHelper.WriteToFile(Btree,MaxRecords,MaxColumn,fileName);
    customFunctions.freeIndexArray(Btree,MaxRecords);
}