#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include<set>
#include "ReadFile.h"
#include "IndexLength.h";

unordered_map<string, vector<int>> fingerPrints;
unordered_map<char, int> singleChar;
set<char> notFound;
int limit = 5;
string relativeString;
int relativeSize;
int countSingleChar = 0;
int countNotFound = 0;


void printCompressed(vector<IndexLength> &compressedVector) {
    for (vector<IndexLength>::iterator itV = compressedVector.begin(); itV != compressedVector.end(); itV++) {
        IndexLength il = *itV;
        cout << il.getIndexRelative() << " " << il.getLength() << " " << il.getIndexCString() << endl;
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

vector<int> findFingerPrint(string &checkFingerPrint) {
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

int findSingleChar(char &checkChar) {
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

vector<IndexLength> compress(string &toCompress) {
    vector<IndexLength> compressedVector;
    int start = 0;
    int end = toCompress.size();
    cout << "compressing" << endl;

    while (start <= end - limit) {
        vector<int> indices;
        string checkFingerPrint;
        checkFingerPrint = toCompress.substr(start, limit);
        indices = findFingerPrint(checkFingerPrint);
        if (indices.size() == 0) {
            int index = findSingleChar(toCompress[start]);
            if (index == -1) {
                countNotFound++;
                notFound.insert(toCompress[start]);
            }
            IndexLength il = IndexLength(index, 1,start);
            compressedVector.push_back(il);
            start++;
            countSingleChar++;
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
            IndexLength il = IndexLength(max_index, max_length, start);
            compressedVector.push_back(il);
            start += max_length;
        }
    }

    while (start < end) {
        int index = findSingleChar(toCompress[start]);

        if (index == -1) {
            countNotFound++;
            notFound.insert(toCompress[start]);
        }

        IndexLength il = IndexLength(index, 1, start);
        compressedVector.push_back(il);
        start++;
        countSingleChar++;
    }
    return compressedVector;
}

char findCharacter(vector<IndexLength> &compressedVector, int &charIndex) {
    int first = 0, last = compressedVector.size(), mid ;
    mid = last / 2;
    int count = 0;
    IndexLength ilTemp;
    while (true) {
        cout << "char index " << charIndex << " mid " << mid << " first " << first << " last " << last;
        ilTemp = compressedVector[mid];
        if (ilTemp.getIndexCString() <= charIndex && ilTemp.getIndexCString() + ilTemp.getLength() - 1 >= charIndex) {
            break;
        }
        if (ilTemp.getIndexCString() > charIndex) {
            last = mid ;
        }
        else {
            first = mid ;
        }
        mid = (first + last) / 2;
        count++;
        if (count > 1000) {
            break;
        }
    }
    int distance = charIndex - mid;
    cout << "we return this index " << ilTemp.getIndexRelative() + distance << endl ;
    return relativeString[ilTemp.getIndexRelative() + distance];
}


int main() {
    int i;
    string location = "C:\\Users\\Bruger\\Desktop\\books\\THESIS start aug 3\\datasets\\embl50.h178.fa";
    int numberOfStrings = FindSize(location);
    cout << numberOfStrings ;

    string* dnaArray = new string[numberOfStrings];
    dnaArray = ReadDna(location, numberOfStrings);

    relativeString = dnaArray[0];
    
    fingerPrints.empty();
    relativeSize = relativeString.size();

    cout << "size of relative string is " << relativeSize << endl ;

    setFingerPrintSingleChar();
    printSingleChar();

    vector<IndexLength>* compressedVectors = new vector<IndexLength> [numberOfStrings];

    // first string can be compressed to itself
    vector<IndexLength> vIL;
    IndexLength il = IndexLength(0, relativeSize - 1, 0);
    vIL.push_back(il);
    compressedVectors[0] = vIL;

    for (int j = 0; j < numberOfStrings; j++) {
        string toCompress = dnaArray[j];
        vector<IndexLength> compressedVector = compress(toCompress);
        compressedVectors[j] = compressedVector;
        cout << "printing compressed info : string number " <<j<<" size of vector : "<< compressedVector.size() <<" size of original string : " << toCompress.size() << endl;
    }

    cout << "single character compressions" << countSingleChar << endl ;
    cout << "single character not found " << countNotFound << " number of different chars " << notFound.size() <<endl ;

    char response;
    int stringIndex, charIndex;
    while(true) {
        cout << " do you want to retrieve a character ? Y/N " << endl;
        cin >> response;
        if (toupper(response) != 'Y')
            break;
        cout << "enter string number starting from 0 " << endl;
        cin >> stringIndex;
        cout << "enter index within the string " << endl;
        cin >> charIndex;
        vector<IndexLength> compressedVector = compressedVectors[stringIndex];
        char found = findCharacter(compressedVector, charIndex);
        cout << "your character is " << found;
    }
    delete[] dnaArray;
    delete[] compressedVectors;
}
    