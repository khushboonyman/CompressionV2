#include <string>
#include <iostream>
#include "windows.h"

using namespace std;

int findSize(string location);

string* readDna(string location, int size);

void writeLog(string location, string fileName, int version, int memoryVar, int time);
