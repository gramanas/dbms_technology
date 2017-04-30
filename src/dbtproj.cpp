#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>
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
//   3 is for both num and 

template <typename T>
bool searchAndInsert (vector<T> * vect, T item, int first, int last) {
    //binary search
    uint mid;

    if (vect->size() == 0) {
        vect->push_back(item);
        return false;
    }

    if (vect->at(last) < item) {
        vect->push_back(item);
        return false;
    }

    if (item < vect-> at(first)) {
        vect->insert(vect->begin(), item);
        return false;
    }
    
    while (first <= last) {
        mid = first + (last - first) / 2;
        if (vect->at(mid) == item) {
            return true;
        } else if (item < vect->at(mid)) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }

    vect->insert(vect->begin() + first, item);
    return false;
}
 
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
    vector<string> strVect;
    vector<uint> numVect;
    uint i = 0;

    while(!feof(in)) {
        while (i < nmem_blocks && !feof(in)) {
            fread(&buffer[i], 1, sizeof(block_t), in);
            for (uint j = 0; j < buffer[i].nreserved; j++) { 
                switch (field) {
                case '0': // recid
                    // if num exists in numList
                    if (searchAndInsert(&numVect, buffer[i].entries[j].recid, 0, numVect.size() == 0 ? 0 : numVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    }
                    break;
                case '1': // num
                    // if num exists in numList
                    if (searchAndInsert(&numVect, buffer[i].entries[j].num, 0, numVect.size() == 0 ? 0 : numVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    }
                    break;
                case '2': // str
                    if (searchAndInsert(&strVect, string(buffer[i].entries[j].str), 0, strVect.size() == 0 ? 0 : strVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    }
                    break;
                case '3': // str + num
                    if (searchAndInsert(&numVect, buffer[i].entries[j].num, 0, numVect.size() == 0 ? 0 : numVect.size()-1) &&
                        searchAndInsert(&strVect, string(buffer[i].entries[j].str), 0, strVect.size() == 0 ? 0 : strVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    }
                    break;
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
