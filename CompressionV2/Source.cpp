#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <set>
#include <math.h>
#include <chrono>
#include <vector>
using namespace std;

//GLOBAL VARIABLES THAT NEED TO BE CHANGED ACCORDINGLY
int version = 2;
int limit = 10;
int recursiveLimit = 1000;
//int runLimit = 100;
int runLimit = 1000000;
string location_main = "C:\\Users\\Bruger\\Desktop\\books\\THESIS start aug 3\\datasets\\";
//file name here
//string fileName = "my_complete_genome.txt";
//fileName = "test_ref_only.txt";
//string fileName = "genome.fa";
//THESE TWO FILES SHOULD BE LOGGED
string fileName = "dna.200MB";
//string fileName = "embl50.h178.fa";
//change according to new version

string* dnaArray;
unordered_map<string, vector<int>> fingerPrints;
unordered_map<char, int> singleChar;
string relativeString;
int relativeSize;
int countSingleChar = 0;
int memoryVar = 0;
int memoryOld = 0;

int mb = 1024 * 1024;
int kb = 1024;

vector<int>* indexRelative;
vector<int>* indexCString;
string extra = "";

//FIND HOW MANY STRINGS ARE THERE
int findSize(string location) {
    ifstream myfile;
    myfile.open(location);
    cout << "dna file opened" << endl;
    string x;
    int size = 0;

    while (myfile >> x) {
        if (x[0] == '>') 
            size += 1;
    }

    myfile.close();
    return size;
}

//FIND HOW MANY STRINGS ARE THERE
int findSizePizzaChilli(string& location) {
    ifstream myfile;
    myfile.open(location);
    cout << "dna file opened" << endl;
    string x;

    int size = 0;
    while (myfile >> x) {
        size += 1;
    }

    myfile.close();
    return size;
}

//CREATE ARRAY OF DNA STRINGS FROM THE FILE
void readDna(string location, int size) {
    ifstream myfile;
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
        else 
            temp += x;
    }

    //temp += "$";
    dnaArray[size] = temp;
    myfile.close();
    cout << endl << "dna file closed" << endl;
}

//CREATE ARRAY OF DNA STRINGS FROM THE FILE
void readDnaPizzaChilli(string& location, int& size) {
    ifstream myfile;
    myfile.open(location);
    dnaArray = new string[size];
    size = 0;
    string x;
    while (myfile >> x) {
        dnaArray[size] = x;
        size++;
    }
    myfile.close();
    cout << endl << "dna file closed and size " << size << endl;
}

//WRITE A LOG TO THE FILE, WHICH WILL BE USED TO PLOT TIME AND SPACE USED BY COMPRESSION TECHNIQUE
void writeLog(string location, string fileName, int version, int memoryVar, float compression, int time) {
    fstream myfile;
    myfile.open(location, fstream::app);
    cout << "log file opened" << endl;
    myfile << "\n";
    myfile << fileName << ";" << version << ";" << memoryVar << ";" << compression << ";" << time;
    myfile.close();
    cout << "log file closed" << endl;
}

//PRINT DATA STORED BY THE COMPRESSED DATASTRUCTURE : INDEX OF MAIN STRING + INDEX OF RELATIVE STRING  
void printCompressed(vector<int>& indexRelativeElement, vector<int>& indexCString) {
    cout << " size of relative indices " << indexRelativeElement.size() << " " << indexCString.size() << endl;
    /*for (int i = 0; i < indexRelativeElement.size();i++) {
        cout << indexRelativeElement[i] << " " << indexCString[i] << endl;
    }*/
}

//PRINT THE FINGERPRINT DATA : SUBSTRING + LIST OF INDICES IN THE STRING
void printFingerPrint() {
    unordered_map<string, vector<int>>::iterator itPrint = fingerPrints.begin();
    while (itPrint != fingerPrints.end()) {
        cout << "substring : " << itPrint->first << " : ";
        vector<int> readVector = itPrint->second;
        for (vector<int>::iterator itV = readVector.begin(); itV != readVector.end(); itV++) 
            cout << *itV << " , ";
        cout << endl;
        itPrint++;
    }
}

//FIND IF THE FINGERPRINT EXISTS AND WHICH INDICES ARE THERE
vector<int> findFingerPrint(string &checkFingerPrint) {
    unordered_map<string, vector<int>>::iterator itCheck = fingerPrints.find(checkFingerPrint);
    vector<int> returnVector;
    if (itCheck != fingerPrints.end()) 
        returnVector = itCheck->second;
    return returnVector;
}

//PRINT LIST OF SINGLE CHARACTERS AND THEIR FIRST POSITION IN THE STRING
void printSingleChar() {
    unordered_map<char, int>::iterator itChar = singleChar.begin();
    while (itChar != singleChar.end()) {
        cout << "char : " << itChar->first << " index : " << itChar->second<< endl;
        itChar++;
    }
}

//FIND IF THE SINGLE CHARACTER EXISTS AND WHAT IS THE INDEX
int findSingleChar(char &checkChar) {
    unordered_map<char, int>::iterator itCheck = singleChar.find(checkChar);
    if (itCheck != singleChar.end()) 
        return itCheck->second;
    return -1;
}

//SET UP THE DATASTRUCTURE CONTAINING FINGERPRINTS AND SINGLE CHARACTERS ALONG WITH THE INDICES
void setFingerPrintSingleChar() {
    int i;
    for (i = 0; i <= relativeSize - limit; i++) {
        string fingerPrint;
        fingerPrint.reserve(limit);
        fingerPrint.append(relativeString, i, limit);
        char single = relativeString[i];
        unordered_map<string, vector<int>>::const_iterator it = fingerPrints.find(fingerPrint);
        unordered_map<char, int>::const_iterator itC = singleChar.find(single);
        if (itC == singleChar.end()) 
            singleChar[single] = i;
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
        if (itC == singleChar.end()) 
            singleChar[single] = i;
        i++;
    }
}

//ADD A NEW CHARACTER TO THE RELATIVE STRING, IF IT WASN'T EXISTING INITIALLY WHEN THE RELATIVE STRING WAS JUST THE FIRST STRING FROM THE FILE 
int expandRelative(char charToAdd) {
    relativeString += charToAdd;
    relativeSize += 1;
    singleChar[charToAdd] = relativeSize - 1 ;
    return relativeSize - 1;
}

//COMPRESS ONE STRING AT A TIME
void compress(string& toCompress, vector<int>& indexRelativeElement, vector<int>& indexCStringElement) {
    int start = 0;
    int end = toCompress.size();

    while (start <= end - limit) {
        vector<int> indices;
        string checkFingerPrint;
        checkFingerPrint.reserve(limit);
        checkFingerPrint.append(toCompress, start, limit);
        indices = findFingerPrint(checkFingerPrint);
        //limit size substring fingerprint not found
        if (indices.size() == 0) {
            int index = findSingleChar(toCompress[start]);
            if (index == -1) {
                cout << "char not found : " << toCompress[start] << endl;
                index = expandRelative(toCompress[start]);
            }
            indexRelativeElement.push_back(index);
            indexCStringElement.push_back(start);
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
            indexRelativeElement.push_back(max_index);
            indexCStringElement.push_back(start);
            start += max_length;
        }
    }

    while (start < end) {
        int index = findSingleChar(toCompress[start]);
        if (index == -1) {
            cout << "char not found : " << toCompress[start] << endl;
            index = expandRelative(toCompress[start]);
        }
        indexRelativeElement.push_back(index);
        indexCStringElement.push_back(start);
        start++;
        countSingleChar++;
    }
}

//FIND LOCATION OF THE INDEX OF SEARCHED STRING IN THE RELATIVE STRING
int findLocation(vector<int>&indexCStringElement, int& charIndex) {
    int count = 0;
    int first = 0; 
    int last = ((charIndex < (int)indexCStringElement.size()) ? charIndex : (indexCStringElement.size() - 1));
    //int last = indexCStringElement.size() - 1;
    int mid = last / 2;
    int indexCStringCurrent, indexCStringNext, indexCStringNextNext;
    while (true) {
        count++;
        if (count > recursiveLimit) {
            cout << "limit reached" << endl;
            break;
        }
        indexCStringCurrent = indexCStringElement[mid];

        if (mid < (int)(indexCStringElement.size() - 1)) {
            indexCStringNext = indexCStringElement[mid+1];
            if (indexCStringCurrent <= charIndex && indexCStringNext > charIndex) {
                break;
            }
            if (mid < (int)(indexCStringElement.size() - 2)) {
                indexCStringNextNext = indexCStringElement[mid + 2];
                if (indexCStringNext <= charIndex && indexCStringNextNext > charIndex) {
                    mid++;
                    break;
                }
            }
        }
        else {
            if (indexCStringCurrent <= charIndex) {
                break;
            }
        }

        if (mid == first) {
            mid = last;
        }
        else {
            if (indexCStringCurrent > charIndex) {
                last = mid;
            }
            else {
                first = mid;
            }

            mid = (first + last) / 2;
        }
                  
    }
    return mid;
}

//FIND WHICH CHARACTER IS PRESENT ON THE STRING INDEX
char findCharacter(vector<int> &indexRelativeElement, vector<int>& indexCStringElement, int &charIndex) {
    int indexFound = findLocation(indexCStringElement, charIndex);
    int distance = charIndex - indexCStringElement[indexFound];
    return relativeString[indexRelativeElement[indexFound] + distance];
}

//FIND SUBSTRING FROM THE SEARCHED STRING, INPUT IS START INDEX AND LENGTH OF SUBSTRING
string findSubString(vector<int>& indexRelativeElement, vector<int>& indexCStringElement, int& charIndex, int& length) {
    int indexFound = findLocation(indexCStringElement, charIndex);
    int distance = charIndex - indexCStringElement[indexFound];
    int indexOnRelative = indexRelativeElement[indexFound] + distance;   
    string toReturn(1,relativeString[indexOnRelative]);
    length--;
    charIndex++;

    while (true) {
        if (length == 0)
            break;
        if (indexFound < indexRelativeElement.size()) {
            int nextIndex = indexFound + 1;
            if (charIndex < indexCStringElement[nextIndex]) {
                indexOnRelative++;
            }
            else {
                if (indexFound < (int)indexCStringElement.size() - 1) {
                    indexFound++;
                }
                else {
                    break;
                }
                indexOnRelative = indexRelativeElement[indexFound];
            }
        }
        else
            indexOnRelative++;
        
        toReturn += relativeString[indexOnRelative];
        length--;
        charIndex++;
    }
    return toReturn ;
}

//PROCESS REQUEST FROM THE USER, WHERE INPUT IS WHICH STRING NUMBER (FROM 0) + INDEX WITHIN THE STRING
void processSingleCharRequestFromUser(int &numberOfStrings, int* sizes) {
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
        vector<int> indexRelativeElement = indexRelative[stringIndex];
        vector<int> indexCStringElement = indexCString[stringIndex];
        //measuring time start
        auto start = chrono::high_resolution_clock::now();

        //this function finds the character from the compressed datastructure
        char charFound = findCharacter(indexRelativeElement, indexCStringElement, charIndex);

        //measuring time end
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        cout << "your character is " << charFound << " it took " << duration.count() << " milliseconds " << endl;
    }
}

//PROCESS REQUEST FROM THE USER, WHERE INPUT IS WHICH STRING NUMBER (FROM 0) + INDEX WITHIN THE STRING + SIZE OF THE SUBSTRING BEING SEARCHED
void processSubstringFromUser(int& numberOfStrings, int* sizes) {

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

        vector<int> indexRelativeElement = indexRelative[stringIndex];
        vector<int> indexCStringElement = indexCString[stringIndex];
        //measuring time start
        auto start = chrono::high_resolution_clock::now();

        //this function finds the character from the compressed datastructure
        string subStringFound = findSubString(indexRelativeElement, indexCStringElement, charIndex, subStringLength);

        //measuring time end
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

        cout << "your substring is " << subStringFound << " it took " << duration.count() << " milliseconds " << endl;
    }
}

//PROCESS ONE MILLION REQUESTS OF FINDING A CHARACTER ON A RANDOM INDEX FROM A RANDOM STRING
auto processMillionRequest(int& numberOfStrings, int* sizes) {
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
        
        vector<int> indexRelativeElement = indexRelative[stringIndex];
        vector<int> indexCStringElement = indexCString[stringIndex];
        //this function finds the character from the compressed datastructure
        char charFound = findCharacter(indexRelativeElement, indexCStringElement, charIndex);
        //cout << "character found at string " << stringIndex << " at index " << charIndex << " is " << charFound << " actual " << dnaArray[stringIndex][charIndex]<<endl;
        counter++;
    }
    //measuring time end
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    return duration;
}

int main() {
    cout << "PROGRAM STARTING!!!" << endl;
    
    int i, numberOfStrings;
    string location = location_main + fileName ;   

    if (fileName.substr(0, 3) == "dna" || fileName.substr(0, 3) == "pro") {
        numberOfStrings = findSizePizzaChilli(location);
    }
    else {
        numberOfStrings = findSize(location);
    }

    dnaArray = new string[numberOfStrings];
    cout << "NUMBER OF STRINGS " << numberOfStrings << endl;

    if (fileName.substr(0, 3) == "dna" || fileName.substr(0, 3) == "pro") {
        readDnaPizzaChilli(location, numberOfStrings);
    }
    else {
        readDna(location, numberOfStrings);
    }

    int* sizes = new int[numberOfStrings];
    //set<int>* characters = new set<int>[numberOfStrings];

    memoryVar += (sizeof(sizes) + (4*numberOfStrings));  //adding size of pointer for size array and each element of size array

    cout << "memory var after size array " << memoryVar << endl;

    for (i = 0; i < numberOfStrings; i++) {
        sizes[i] = dnaArray[i].size();
        memoryOld += sizes[i]; //adding length of each string
    }

    cout << "DNA ARRAY !!!" << endl;

    relativeString = dnaArray[0];
    memoryVar += sizeof(relativeString);  //adding space for reference string

    relativeSize = relativeString.size();

    cout << "total number of strings : " << numberOfStrings << " and size of relative string : " << relativeSize << endl ;

    setFingerPrintSingleChar();
    printSingleChar();

    indexRelative = new vector<int>[numberOfStrings];
    indexCString = new vector<int>[numberOfStrings];

    memoryVar += (sizeof(indexRelative) + sizeof(indexCString)); //adding pointer size for both index arrays

    //to check how long it takes to compress all the data
    auto start = chrono::high_resolution_clock::now();

    cout << "BEFORE COMPRESSION !!!" << endl;

    // first string can be compressed to itself
    indexRelative[0].push_back(0);
    indexCString[0].push_back(0);

    memoryVar += 8; //adding size for compression of first string
    //printCompressed(indexRelative[0], indexCString[0]);

    for (i = 1; i < numberOfStrings; i++) {
        cout << "compressing " << i << endl ;
        string toCompress = dnaArray[i];
        compress(toCompress, indexRelative[i], indexCString[i]);
        //printCompressed(indexRelative[i], indexCString[i]);
        memoryVar += (8*indexRelative[i].size()); //adding space for encoding
        //cout << "size of string " << dnaArray[i].size() << " size of compressed structure " << 8*indexRelative[i].size() << endl;
        //memoryVar += ((4 * indexRelative[i].size()) + (4 * indexCString[i].size())); //adding space for encoding
    }

    memoryVar += relativeSize; //finally adding length of relative string as it was expanded

    delete[] dnaArray;
    cout << "AFTER COMPRESSION!!!" << endl;

    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    //cout << "it took " << duration.count() << " seconds to compress " << numberOfStrings <<  endl;

    //processSingleCharRequestFromUser(numberOfStrings,sizes);
    //processSubstringFromUser(numberOfStrings, sizes);
    auto durationMillion = processMillionRequest(numberOfStrings, sizes);
    //cout << durationMillion.count() <<" milliseconds to process million requests ";
    //delete[] dnaArray;
    delete[] sizes;
    delete[] indexRelative;
    delete[] indexCString;

    cout << "PROGRAM ENDING!!! " << endl;

    //string headers = "FILE_NAME;VERSION;MEMORY;COMPRESSION;TIME";
    float compression = (float(memoryVar) / float(memoryOld)) * 100;
    cout << "old memory : " << memoryOld << " compressed memory : " << memoryVar << " compression : " << compression << endl;
    
    location = location_main + "LOGS.csv";
    writeLog(location, fileName, version, memoryVar, compression, (int) durationMillion.count());
}
    