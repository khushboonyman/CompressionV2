#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "ReadFile.h"

using namespace std;

#include <iostream>
#include <string>
#include <unordered_map>

void printFingerPrint(unordered_map<string, vector<int>> fingerPrints) {
    unordered_map<string, vector<int>>::iterator itPrint = fingerPrints.begin();
    while (itPrint != fingerPrints.end()) {
        cout << "substring : " << itPrint->first << " : ";
        vector<int> readVector = itPrint->second;
        for (vector<int>::iterator itV = readVector.begin(); itV != readVector.end(); itV++) {
            cout << *itV << " , ";
        }
        cout << endl;
        itPrint++;
    }
}

int main() {
    int i;
    string location = "C:\\Users\\Bruger\\Desktop\\books\\THESIS start aug 3\\datasets\\embl50.h178.fa";
    int length = FindSize(location);
    cout << length;

    string* dnaArray = new string[length];
    dnaArray = ReadDna(location, length);

    string relativeString = dnaArray[0];
    //string relativeString = "abcdefghijabcdefghij";
    unordered_map<string, vector<int>> fingerPrints;
    fingerPrints.empty();
    int relativeSize = relativeString.size();

    cout << "size is " << relativeSize;
    for (i = 0; i <= relativeSize-10 ; i++) {
        string fingerPrint = relativeString.substr(i, 10);

        unordered_map<string, vector<int>>::const_iterator it = fingerPrints.find(fingerPrint);

        if (it == fingerPrints.end()) {
            vector<int> newVector;
            newVector.push_back(i);
            fingerPrints[fingerPrint] = newVector;
        }
        else {
            vector<int> existingVector = it->second;
            existingVector.push_back(i);
            fingerPrints[fingerPrint] = existingVector;
        }
    }
    printFingerPrint(fingerPrints);
    delete[] dnaArray;
}
    