#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "ReadFile.h"
#include "IndexLength.h";

unordered_map<string, vector<int>> fingerPrints;
unordered_map<char, int> singleChar;
int limit = 5;
vector<IndexLength> compressedVector;
string relativeString;
int relativeSize;
int countNotFound;

void printCompressed() {
    for (vector<IndexLength>::iterator itV = compressedVector.begin(); itV != compressedVector.end(); itV++) {
        IndexLength il = *itV;
        cout << il.getIndex() << " " << il.getLength() << endl;
    }
}

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

void setFingerPrintSingleChar() {
    int i;
    for (i = 0; i <= relativeSize - limit; i++) {
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

    while (i < relativeSize) {
        char single = relativeString[i];
        unordered_map<char, int>::const_iterator itC = singleChar.find(single);
        if (itC == singleChar.end()) {
            singleChar[single] = i;
        }
        i++;
    }
}

void compress(string toCompress) {
    int start = 0;
    int end = toCompress.size();
    countNotFound = 0;
    cout << "compressing" << endl;

    while (start <= end - limit) {
        vector<int> indices;
        string checkFingerPrint;
        checkFingerPrint = toCompress.substr(start, limit);
        indices = findFingerPrint(checkFingerPrint);
        if (indices.size() == 0) {
            int index = findSingleChar(toCompress[start]);
            if (index == -1) {
                cout << "char not found " << toCompress[start] << endl;
            }
            IndexLength il = IndexLength(index, 1);
            compressedVector.push_back(il);
            start++;
            countNotFound++;
        }
        else {
            int max_length = limit;
            int max_index = -1;
            for (vector<int>::iterator itV = indices.begin(); itV != indices.end(); itV++) {
                if (max_index == -1) {
                    max_index = *itV;
                }
                int currIndexString = start + limit;
                int currIndexRelativeString = *itV + limit;
                int length = limit;
                while (true) {
                    if (currIndexString >= end || currIndexRelativeString >= relativeSize) {
                        break;
                    }
                    char stringChar = toCompress[currIndexString];
                    char relativeChar = relativeString[currIndexRelativeString];
                    if (stringChar == relativeChar) {
                        currIndexString++;
                        currIndexRelativeString++;
                        length++;
                    }
                    else {
                        break;
                    }
                }
                if (length > max_length) {
                    max_length = length;
                    max_index = *itV;
                }
            }
            IndexLength il = IndexLength(max_index, max_length);
            compressedVector.push_back(il);
            start += max_length;
        }
    }

    while (start < end) {
        int index = findSingleChar(toCompress[start]);

        if (index == -1) {
            cout << "char not found " << toCompress[start] << endl;
        }

        IndexLength il = IndexLength(index, 1);
        compressedVector.push_back(il);
        start++;
        countNotFound++;
    }
}

int main() {
    int i;
    string location = "C:\\Users\\Bruger\\Desktop\\books\\THESIS start aug 3\\datasets\\embl50.h178.fa";
    int length = FindSize(location);
    cout << length;

    string* dnaArray = new string[length];
    dnaArray = ReadDna(location, length);

    relativeString = dnaArray[0];
    
    fingerPrints.empty();
    relativeSize = relativeString.size();

    cout << "size of relative string is " << relativeSize << endl ;

    setFingerPrintSingleChar();
    printSingleChar();

    string toCompress = dnaArray[1];
    compress(toCompress);
    
    cout << "printing compressed info : " << compressedVector.size() << endl;

    printCompressed();

    cout << "single character compressions" << countNotFound;
    
    delete[] dnaArray;
}
    