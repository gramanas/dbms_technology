#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <stdlib.h>
#include "./dbtproj.h"

using namespace std;

// generate a randomg string 
void genRandomString(char *s, const int len);

// get true or false with a 50 - 50 chance
bool randomBool();

// Create a random binary file
void createRandFile(char* filename, uint blockNum);

// prints a binary file in the project's form
void printFile(char* filename, uint recordId, bool recBool, uint blockId, bool blockBool);

#endif
