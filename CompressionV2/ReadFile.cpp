#include <iostream>
#include <fstream>
#include <string>
#include "ReadFile.h"

using namespace std;

int FindSize(string location) {
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

string* ReadDna(string location, int size) {
    
    ifstream myfile;

    string* dnaArray;
    dnaArray = new string[size];

    myfile.open(location);

    size = -1;
    string temp, x;
    while (myfile >> x) {
        if (x[0] == '>') {
            if (size > -1) {
                temp += "$";
                dnaArray[size] = temp;
            }
            temp = "";
            size += 1;
        }
        else {
            temp += x;
        }
    }

    myfile.close();
    cout << endl << "file closed" << endl ;
    
    return dnaArray;
}