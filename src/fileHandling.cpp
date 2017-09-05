#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dbtproj.hpp"
#include "./fileHandling.hpp"

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

            record.num = rand() % 1000; //Give the record a random number
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

    block_t buffer;
    uint count = 0;

    while (fread(&buffer, 1, sizeof(block_t), in)) {
        for (uint i = 0; i < buffer.nreserved; i++) {
            if (buffer.entries[i].valid) {
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

void printFile(char *filename, uint recordId, bool recBool, uint blockId, bool blockBool) {
    FILE * in;

    in = fopen(filename, "rb");
    if (in == NULL) {
        cerr << "No such file." << endl;
    }

    block_t buffer;
    while (sizeof(block_t) == fread(&buffer, 1, sizeof(block_t), in)) {
        for (uint i = 0; i < buffer.nreserved; i++) {
            if (recBool == true && buffer.entries[i].recid == recordId) {
                printRecord(buffer, i);
                return;
            }
            //if (buffer.entries[i].valid) {
                if (recBool == false && blockBool == false) {
                    printRecord(buffer, i);
                } else if (blockBool == true && buffer.blockid == blockId) {
                    printRecord(buffer ,i);
                }
                //}
        }
    }
    fclose(in);
}

// Removes invalid entries so the other functions
// won't have to care about it
void clearFile(char *filename) {
    FILE *in, *out;

    in = fopen(filename, "rb");
    if (in == NULL) {
        cerr << "No such file." << endl;
    }

    out = fopen("tempClear", "wb");
    if (out == NULL) {
        cerr << "No such file." << endl;
    }

    block_t buffer;
    uint blockid = 0;
    record_t tempRec[MAX_RECORDS_PER_BLOCK];
    uint nreserved = 0;
    uint count = 0;
    //while (sizeof(block_t) == fread(&buffer, 1, sizeof(block_t), in)) {
    while (!feof(in)) {
        if (count == 0) {
            fread(&buffer, 1, sizeof(block_t), in);
        }

        while (count < buffer.nreserved && nreserved < MAX_RECORDS_PER_BLOCK) {
            if (buffer.entries[count].valid == true) {
                tempRec[count] = buffer.entries[count];
                nreserved++;
            }
            count++;
        }

        if (nreserved == MAX_RECORDS_PER_BLOCK || feof(in)) {
            block_t temp;

            temp.blockid = blockid++;
            temp.valid = true;
            temp.nreserved = nreserved;
            for (uint i = 0; i < nreserved; i++) {
                temp.entries[i] = tempRec[i];
            }
            for (uint i = nreserved; i < MAX_RECORDS_PER_BLOCK; i++) {
                temp.entries[i].valid = false;
            }
            fwrite(&temp, 1, sizeof(block_t), out);

            nreserved = 0;
        }

        if (count >= MAX_RECORDS_PER_BLOCK) {
            count = 0;
        }
    }

    fclose(in);
    fclose(out);

    remove(filename);
    rename("tempClear", filename);
}
