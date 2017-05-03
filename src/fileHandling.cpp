#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dbtproj.h"
#include "./fileHandling.h"

using namespace std;

void genRandomString(char *s, const int len) {
    static const char alphanum[] =
        //63^32 = 3.79225543573e57
        //"0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz ";
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

bool randomBool() {
	return rand() % 2 == 1;
}

void createRandFile(char* filename, uint blockNum){
    FILE * out;

    out = fopen(filename, "wb");
    if (out == NULL) {
        cerr << "Could not open input file." << endl;
    }

    record_t record;
    uint record_id = 0;

    block_t block;

    srand(time(NULL));

    char* temp_str;
    temp_str = (char*)malloc(STR_LENGTH);
    if (temp_str == NULL) {
        cerr << "Memory error" << endl;
        exit(1);
    }        

    for(uint i = 0; i < blockNum; i++){ //Make blockNum blocks and put records inside
        block.blockid = i; 

        for(uint j = 0; j < MAX_RECORDS_PER_BLOCK; j++) { // Put records in blocks 
            record.recid = record_id++;

            genRandomString(temp_str,STR_LENGTH - 1);
            strcpy(record.str, temp_str);

            record.num = rand() % 100000; //Give the record a random number
            record.valid = true;
            memmove(&block.entries[j], &record, sizeof(record_t));
        }
        block.nreserved = MAX_RECORDS_PER_BLOCK;
        block.valid = true;
        fwrite(&block, 1, sizeof(block_t), out);
    }
    fclose(out);
}

uint countValid(char* filename) {
    FILE * in;

    in = fopen(filename, "rb");
    if (in == NULL) {
        cerr << "No such file." << endl;
    }
    
    block_t block;
    uint count = 0;

    while (fread(&block, 1, sizeof(block_t), in)) {
        for (uint i = 0; i < block.nreserved; i++) {
            if (block.entries[i].valid) {
                count++;
            }
        }
    }    
    fclose(in);
    return count;
}

void printRecord(block_t block, int i) { 
    cout << "B_ID  : " << block.blockid          << endl; 
    cout << "R_ID  : " << block.entries[i].recid << endl;
    cout << "R_NUM : " << block.entries[i].num   << endl;
    cout << "R_STR : " << block.entries[i].str   << endl;
    cout << "Valid : " << block.entries[i].valid << endl;
    cout << "----------------------------------" << endl;
}

void printFile(char* filename, uint recordId, bool recBool, uint blockId, bool blockBool) {
    FILE * in;

    in = fopen(filename, "rb");
    if (in == NULL) {
        cerr << "No such file." << endl;
    }

    block_t block;
    while (fread(&block, 1, sizeof(block_t), in)) {
        for (uint i = 0; i < block.nreserved; i++) {
            if (recBool == true && block.entries[i].recid == recordId) {
                printRecord(block, i);
                return;
            }
            if (block.entries[i].valid) {
                if (recBool == false && blockBool == false) {
                    printRecord(block, i);
                } else if (blockBool == true && block.blockid == blockId) {
                    printRecord(block ,i);
                }
            }
        }
    }
    fclose(in);
}
