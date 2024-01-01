#include "IndexInterface.h"
#include "CustomFunctions.h"
using namespace std;

int main() {
    // required in assigment but not use
//    CustomFunctions custom;
//    int records = 10;
//    int columns = 11;
//    int** arr =custom.CreateIndexArray(records,columns);
//    custom.print(arr,records,columns);
    IndexInterface interface(11,10);
    interface.MainMenu();
    return 0;
}