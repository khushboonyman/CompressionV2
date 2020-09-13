#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <set>
#include "IndexLength.h"
#include <math.h>
#include <chrono>

using namespace std;

//change according to new version
int version = 2;
//int runLimit = 10000;
int runLimit = 1000000;
unordered_map<string, vector<int>> fingerPrints;
unordered_map<char, int> singleChar;
int limit = 5;
string relativeString;
int relativeSize;
int countSingleChar = 0;
int memoryVar = 0;
int mb = 1024 * 1024;
int kb = 1024;

int findSize(string location) {
    ifstream myfile;
    myfile.open(location);
    cout << "file read" << endl;

    string x;
    int size = 0;

    while (myfile >> x) {
        if (x[0] == '>') {
            size += 1;
        }
    }

    myfile.close();

    return size;
}

string* readDna(string location, int size) {

    ifstream myfile;

    string* dnaArray;
    dnaArray = new string[size];

    myfile.open(location);

    size = -1;
    string temp, x;
    while (myfile >> x) {
        if (x[0] == '>') {
            if (size > -1) {
                //temp += "$";
                dnaArray[size] = temp;
            }
            temp = "";
            size += 1;
        }
        else {
            temp += x;
        }
    }

    //temp += "$";
    dnaArray[size] = temp;

    myfile.close();
    cout << endl << "file closed" << endl;

    return dnaArray;
}

void writeLog(string location, string fileName, int version, int memoryVar, int time) {
    fstream myfile;
    myfile.open(location, fstream::app);
    cout << "file opened" << endl;

    // + memoryDna+ ";"+ memoryFingerPrint + ";" + memoryCompressed + ";" + memoryVar + ";" + time;
    myfile << "\n";
    myfile << fileName << ";" << version << ";" << memoryVar << ";" << time;
    myfile.close();
    cout << "file closed" << endl;
}


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

int expandRelative(char charToAdd) {
    relativeString += charToAdd;
    relativeSize += 1;
    singleChar[charToAdd] = relativeSize - 1 ;
    return relativeSize - 1;
}

vector<IndexLength> compress(string &toCompress) {
    vector<IndexLength> compressedVector;
    int start = 0;
    int end = toCompress.size();

    while (start <= end - limit) {
        vector<int> indices;
        string checkFingerPrint;
        checkFingerPrint = toCompress.substr(start, limit);
        indices = findFingerPrint(checkFingerPrint);
        //limit size substring fingerprint not found
        if (indices.size() == 0) {
            int index = findSingleChar(toCompress[start]);
            if (index == -1) {
                cout << "char not found : " << toCompress[start] << endl;
                index = expandRelative(toCompress[start]);
            }
            IndexLength il = IndexLength(index, 1,start);
            compressedVector.push_back(il);
            start++;
            countSingleChar++;
        }
        //fingerprint(s) found, so now we find the longest substring that matches through the fingerprint(s)
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
            cout << "char not found : " << toCompress[start] << endl;
            index = expandRelative(toCompress[start]);
        }

        IndexLength il = IndexLength(index, 1, start);
        compressedVector.push_back(il);
        start++;
        countSingleChar++;
    }
    return compressedVector;
}

int findLocation(vector<IndexLength>& compressedVector, int& charIndex) {
    int first = 0; 
    //int last = ((charIndex < compressedVector.size()) ? charIndex : (compressedVector.size() - 1));
    int last = compressedVector.size() - 1;
    int mid = last / 2;
    IndexLength ilTemp, ilTempNext;
    while (true) {
        
        ilTemp = compressedVector[mid];
        if (ilTemp.getIndexCString() <= charIndex && ilTemp.getIndexCString() + ilTemp.getLength() - 1 >= charIndex) {
            break;
        }
        if (mid < (int)(compressedVector.size()-1)) {
            ilTempNext = compressedVector[mid+1];
            if (ilTempNext.getIndexCString() <= charIndex && ilTempNext.getIndexCString() + ilTempNext.getLength() - 1 >= charIndex) {
                mid++;
                break;
            }
        }

        if (ilTemp.getIndexCString() > charIndex) {
            last = mid;
        }
        else {
            first = mid;
        }

        mid = (first + last) / 2;          
    }
    return mid;
}

char findCharacter(vector<IndexLength> &compressedVector, int &charIndex) {
    int indexFound = findLocation(compressedVector, charIndex);
    IndexLength ilTemp = compressedVector[indexFound];
    int distance = charIndex - ilTemp.getIndexCString();
    return relativeString[ilTemp.getIndexRelative() + distance];
}

string findSubString(vector<IndexLength>& compressedVector, int& charIndex, int& length) {
    int indexFound = findLocation(compressedVector, charIndex);
    IndexLength ilTemp = compressedVector[indexFound];
    int distance = charIndex - ilTemp.getIndexCString();
    int indexOnRelative = ilTemp.getIndexRelative() + distance;
    string toReturn(1,relativeString[indexOnRelative]);
    length--;

    while (true) {
        if (length == 0)
            break;
        if (indexOnRelative - ilTemp.getIndexRelative() + 1 < ilTemp.getLength()) {
            indexOnRelative ++;
        }
        else {
            if (indexFound < (int)(compressedVector.size() - 1)) {
                indexFound ++;
            }
            else {
                break;
            }
            ilTemp = compressedVector[indexFound];
            indexOnRelative = ilTemp.getIndexRelative();
        }
        toReturn += relativeString[indexOnRelative];
        length--;
    }
    return toReturn ;
}

void processSingleCharRequestFromUser(int &numberOfStrings, int* sizes, vector<IndexLength>* compressedVectors) {

    char response;
    int stringIndex, charIndex;

    while (true) {
        cout << " do you want to retrieve a character ? Y/N " << endl;
        cin >> response;
        if (toupper(response) != 'Y')
            break;
        cout << "enter string index starting from 0 " << endl;
        cin >> stringIndex;
        if (stringIndex < 0 || stringIndex > numberOfStrings - 1) {
            cout << "you entered a wrong string index" << endl;
            continue;
        }
        cout << "enter index within the string " << endl;
        cin >> charIndex;
        if (charIndex < 0 || charIndex > sizes[stringIndex] - 1) {
            cout << "the string is not that long " << endl;
            continue;
        }
        vector<IndexLength> compressedVector = compressedVectors[stringIndex];
        //measuring time start
        auto start = chrono::high_resolution_clock::now();

        //this function finds the character from the compressed datastructure
        char charFound = findCharacter(compressedVector, charIndex);

        //measuring time end
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        cout << "your character is " << charFound << " it took " << duration.count() << " milliseconds " << endl;
    }
}

void processSubstringFromUser(int& numberOfStrings, int* sizes, vector<IndexLength>* compressedVectors) {

    char response;
    int stringIndex, charIndex, subStringLength;

    while (true) {

        cout << " do you want to retrieve a substring ? Y/N " << endl;
        cin >> response;
        if (toupper(response) != 'Y')
            break;
        cout << "enter string index starting from 0 " << endl;
        cin >> stringIndex;
        if (stringIndex < 0 || stringIndex > numberOfStrings - 1) {
            cout << "you entered a wrong string index" << endl;
            continue;
        }
        cout << "enter index within the string " << endl;
        cin >> charIndex;
        if (charIndex < 0 || charIndex > sizes[stringIndex] - 1) {
            cout << "the string is not that long " << endl;
            continue;
        }

        cout << "enter length of substring " << endl;
        cin >> subStringLength;
        if (subStringLength < 0) {
            cout << "length can't be negative " << endl;
            continue;
        }

        vector<IndexLength> compressedVector = compressedVectors[stringIndex];
        //measuring time start
        auto start = chrono::high_resolution_clock::now();

        //this function finds the character from the compressed datastructure
        string subStringFound = findSubString(compressedVector, charIndex, subStringLength);

        //measuring time end
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        cout << "your substring is " << subStringFound << " it took " << duration.count() << " milliseconds " << endl;
    }
}

auto processMillionRequest(int& numberOfStrings, int* sizes, vector<IndexLength>* compressedVectors) {

    cout << " processing million requests " << endl;

    int counter = 0;
    int stringIndex, charIndex;
    //measuring time start
    auto start = chrono::high_resolution_clock::now();

    while (counter<runLimit) {
        if (counter % 10000 == 0) {
            cout << counter << endl;
        }
        stringIndex = rand() % (numberOfStrings - 1);
        charIndex = rand() % (sizes[stringIndex] - 1);
        vector<IndexLength> compressedVector = compressedVectors[stringIndex];
        //this function finds the character from the compressed datastructure
        char charFound = findCharacter(compressedVector, charIndex);
        counter++;
    }
    //measuring time end
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    return duration;
}

int main() {
    cout << "PROGRAM STARTING!!!" << endl;
    
    int i;
    string location_main = "C:\\Users\\Bruger\\Desktop\\books\\THESIS start aug 3\\datasets\\";
    //file name here
    //string fileName = "Gen178.fa";
    string fileName = "embl50.h178.fa";
    string location = location_main + fileName ;
    
    int numberOfStrings = findSize(location);

    string* dnaArray = new string[numberOfStrings];

    dnaArray = readDna(location, numberOfStrings);

    int* sizes = new int[numberOfStrings];

    for (i = 0; i < numberOfStrings; i++) {
        sizes[i] = dnaArray[i].size();
    }

    cout << "DNA ARRAY !!!" << endl;

    relativeString = dnaArray[0];
    memoryVar += sizeof(relativeString);  //adding space for reference string

    cout << "size of relative string " << memoryVar << endl;

    //fingerPrints.empty();
    relativeSize = relativeString.size();

    cout << "total number of strings : " << numberOfStrings << " and size of relative string : " << relativeSize << endl ;

    setFingerPrintSingleChar();
    printSingleChar();

    vector<IndexLength>* compressedVectors = new vector<IndexLength> [numberOfStrings];

    //to check how long it takes to compress all the data
    auto start = chrono::high_resolution_clock::now();

    cout << "BEFORE COMPRESSION !!!" << endl;

    // first string can be compressed to itself
    vector<IndexLength> vIL;
    IndexLength il = IndexLength(0, relativeSize - 1, 0);
    vIL.push_back(il);
    compressedVectors[0] = vIL;   

    for (int j = 1; j < numberOfStrings; j++) {
        cout << "compressing " << j << endl ;
        string toCompress = dnaArray[j];
        vector<IndexLength> compressedVector = compress(toCompress);
        compressedVectors[j] = compressedVector;
        memoryVar += sizeof(compressedVectors[j]); //adding space for encoding
        //printCompressed(compressedVector);
    }

    delete[] dnaArray;
    cout << "AFTER COMPRESSION!!!" << endl;

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "it took " << duration.count() << " seconds to compress " << numberOfStrings <<  endl;

    //processSingleCharRequestFromUser(numberOfStrings,sizes,compressedVectors);
    //processSubstringFromUser(numberOfStrings, sizes, compressedVectors);

    auto durationMillion = processMillionRequest(numberOfStrings, sizes, compressedVectors);

    cout << durationMillion.count() <<" milliseconds to process million requests ";

    delete[] sizes;
    delete[] compressedVectors;

    cout << "PROGRAM ENDING!!!" << endl;

    //string headers = "FILE_NAME;VERSION;MEMORY;TIME";
    location = location_main + "LOGS.csv";
    int timeUsed = 0;     
    writeLog(location, fileName, version, memoryVar, (int) durationMillion.count());
}
    