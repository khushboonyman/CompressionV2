#include <iostream>
#include <fstream>
#include <string>

#include "ReadFile.h"

using namespace std;

int main() {
    int i;
    string location = "C:\\Users\\Bruger\\Desktop\\books\\THESIS start aug 3\\datasets\\embl50.h178.fa";
    int length = FindSize(location);
    cout << length;

    string* dnaArray = new string[length];
    dnaArray = ReadDna(location, length);
    
    for (i = 0; i < length; i++) {
        cout << dnaArray[i][0] << endl;
    }
    
    delete[] dnaArray;
}
    