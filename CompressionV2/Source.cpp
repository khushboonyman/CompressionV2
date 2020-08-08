#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "ReadFile.h"

using namespace std;

#include <iostream>
#include <string>
#include <unordered_map>
#include "IndexLength.h";

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

void printSingleChar(unordered_map<char, int> singleChar) {
    unordered_map<char, int>::iterator itChar = singleChar.begin();
    while (itChar != singleChar.end()) {
        cout << "char : " << itChar->first << " index : " << itChar->second<< endl;
        itChar++;
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
    unordered_map<char, int> singleChar;
    fingerPrints.empty();
    int relativeSize = relativeString.size();

    cout << "size is " << relativeSize;
    for (i = 0; i <= relativeSize-10 ; i++) {
        string fingerPrint = relativeString.substr(i, 10);
        char single = relativeString[i];
        unordered_map<string, vector<int>>::const_iterator it = fingerPrints.find(fingerPrint);
        unordered_map<char, int>::const_iterator itC = singleChar.find(single);
        if (itC == singleChar.end()) {
            singleChar[single] = i;
        }
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
    //printFingerPrint(fingerPrints);
    /*string stringToCompress = dnaArray[1];
    int j = 0;
    while (j < stringToCompress.size()) {
        j++;
    }

    IndexLength il = IndexLength(0, 0);
    cout << il.getIndex() << " " << il.getLength();
    */
    printSingleChar(singleChar);
    delete[] dnaArray;
}
    