#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

#include "./dbtproj.h"
#include "./fileHandling.h"

using namespace std;

void genRandomString(char *s, const int len) {
    static const char alphanum[] =
        "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz ";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

bool randomBool() {
	return rand() % 2 == 1;
}

void createRandFile(char* filename, uint blockNum){
    int file = creat(filename, S_IRWXU);

    record_t record;
    uint record_id = 0;

    block_t block;

    srand(time(NULL));

    char* temp_str;
    temp_str = (char*)malloc(STR_LENGTH);
    if (temp_str == NULL) exit(1);

    for(uint i = 0; i < blockNum; i++){ //Make blockNum blocks and put records inside
        block.blockid = i; 

        for(uint j = 0; j < MAX_RECORDS_PER_BLOCK; j++) { // Put records in blocks 
            record.recid = record_id += 1;

            genRandomString(temp_str,STR_LENGTH - 1);
            strcpy(record.str, temp_str);

            record.num = rand() % 20000; //Give the record a random number
            record.valid = true;
            memmove(&block.entries[j], &record, sizeof(record_t));
        }
        block.nreserved = MAX_RECORDS_PER_BLOCK;
        block.valid = true;
        write(file, &block, sizeof(block_t));
    }
    close(file);
}

void printFile(char* filename) {
    int file = open(filename, O_RDONLY, S_IRWXU);
    if (file == -1) exit(1);
    
    block_t block;
    while (read(file, &block, sizeof(block_t))) {
        for(uint i = 0; i < block.nreserved; i++) {
            if(block.entries[i].valid){
                cout << "B_ID  : " << block.blockid          << endl; 
                cout << "R_ID  : " << block.entries[i].recid << endl;
                cout << "R_NUM : " << block.entries[i].num   << endl;
                cout << "R_STR : " << block.entries[i].str   << endl;
                cout << "----------------------------------" << endl;
            }
        }
    }
    close(file);
}
