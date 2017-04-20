#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <list>
#include <algorithm>
#include <iterator>

#include "./dbtproj.h"

using namespace std;

void MergeSort (char *infile, unsigned char field,
                block_t *buffer, unsigned int nmem_blocks,
                char *outfile, unsigned int *nsorted_segs,
                unsigned int *npasses, unsigned int *nios) { 
    FILE *in, *out;

    in = fopen(infile, "r");
    if (in == NULL) {
        cerr << "Could not open input file." << endl;
    }

    out = fopen(outfile, "w");
    if (out == NULL) {
        cerr << "Could not open output file." << endl;
    }

    
    //TODO

    fclose(in);
    fclose(out);    
}
//   field: which field will be used for sorting: 0 is for recid, 1 is for num, 2 is for str and 
//   3 is for both num and str
 
void EliminateDuplicates (char *infile, unsigned char field,
                          block_t *buffer, unsigned int nmem_blocks,
                          char *outfile, unsigned int *nunique, unsigned int *nios) {
    FILE *in, *out;

    in = fopen(infile, "r");
    if (in == NULL) {
        cerr << "Could not open input file." << endl;
    }

    out = fopen(outfile, "w");
    if (out == NULL) {
        cerr << "Could not open output file." << endl;
    }
    
    buffer = new block_t[nmem_blocks];
    list<uint> numList;
    list<char *> strList;
    uint i = 0;

    //////////////////////////
    // THIS IS WAY TOO SLOW //
    //////////////////////////

    // TODO should implement a binary search on the list
    //      to check if the value exists

    // returns 1 if the value is not there (it also inserts it in the corret place)
    // returns 0 if the value is there

    // List is not a good data structure for this, since we need direct access to every cell
    // Prolly vector...
    while(!feof(in)) {
        while (i < nmem_blocks && !feof(in)) {
            fread(&buffer[i], 1, sizeof(block_t), in);
            for (uint j = 0; j < buffer[i].nreserved; j++) { 
                switch (field) {
                case '0': // recid
                    // if num exists in numList
                    if (find(numList.begin(), numList.end(), buffer[i].entries[j].recid) != numList.end()) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        numList.push_back(buffer[i].entries[j].recid);
                    }
                    break;
                case '1': // num
                    // if num exists in numList
                    if (find(numList.begin(), numList.end(), buffer[i].entries[j].num) != numList.end()) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        numList.push_back(buffer[i].entries[j].num);
                    }
                    break;
                    // case 2: // str
                    //TODO
                    //     break;
                    // case 3: // str + num
                    //TODO
                    //     break;
                default:
                    cerr << "wrong filed" << endl;
                    exit(1);
                }
            }
            fwrite(&buffer[i], 1, sizeof(block_t), out);
        }
    }
    fclose(in);
    fclose(out);
}

void MergeJoin (char *infile1, char *infile2,
                unsigned char field, block_t *buffer,
                unsigned int nmem_blocks, char *outfile,
                unsigned int *nres, unsigned int *nios) {
    //TODO
}

void HashJoin (char *infile1, char *infile2,
               unsigned char field, block_t *buffer,
               unsigned int nmem_blocks, char *outfile,
               unsigned int *nres, unsigned int *nios) {
    //TODO
}
