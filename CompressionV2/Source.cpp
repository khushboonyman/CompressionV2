#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "ReadFile.h"
#include "IndexLength.h";

using namespace std;

unordered_map<string, vector<int>> fingerPrints;
unordered_map<char, int> singleChar;
int limit = 5;

void printFingerPrint() {
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

vector<int> findFingerPrint(string checkFingerPrint) {
    unordered_map<string, vector<int>>::iterator itCheck = fingerPrints.find(checkFingerPrint);
    vector<int> returnVector;
    if (itCheck != fingerPrints.end()) {
        returnVector = itCheck->second;
    }
    return returnVector;
}

void printSingleChar() {
    unordered_map<char, int>::iterator itChar = singleChar.begin();
    while (itChar != singleChar.end()) {
        cout << "char : " << itChar->first << " index : " << itChar->second<< endl;
        itChar++;
    }
}

int findSingleChar(char checkChar) {
    unordered_map<char, int>::iterator itCheck = singleChar.find(checkChar);
    if (itCheck != singleChar.end()) {
        return itCheck->second;
    }
    return -1;
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
    //unordered_map<string, vector<int>> fingerPrints;
    //unordered_map<char, int> singleChar;
    fingerPrints.empty();
    int relativeSize = relativeString.size();

    cout << "size is " << relativeSize;
    for (i = 0; i <= relativeSize-limit ; i++) {
        string fingerPrint = relativeString.substr(i, limit);
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
    //IndexLength il = IndexLength(0, 0);
    printSingleChar();
    string toCompress = dnaArray[1];
    int start = 0;
    int end = toCompress.size();
    int countNotFound = 0;
    while (start <= end-limit) {
        
        vector<int> indices;
        string checkFingerPrint;
        checkFingerPrint = toCompress.substr(start, limit);
        indices = findFingerPrint(checkFingerPrint);
        if (indices.size() == 0) {
            //cout << "start " << start << " " << end << endl;
            int index = findSingleChar(toCompress[start]);
            if (index == -1) {
                cout << "char not found " << toCompress[start] <<endl ;
            }
            start++;
            countNotFound++;
        }
        else {
            start += limit;
        }
    }

    cout << "not found" << countNotFound;
    delete[] dnaArray;
}
    