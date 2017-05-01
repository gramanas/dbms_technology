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
//   field: which field will be used for sorting: 0 is for recid, 1 is for num, 2 is for str and 
//   3 is for both num and 

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

// This fuction performs binary search on a given vector. The only limitation
// is that the vector type must have comparison operators working (<, <=, ==)
//
// If binary search fails it iserts the value in the vector, thus ensuring
// correct order of the values.
template <typename T>
bool searchAndInsert (vector<T> * vect, T item, int first, int last) {
    uint mid;

    // if vector is empty
    if (vect->size() == 0) {
        vect->push_back(item);
        return false;
    }

    // if new item is greater than the biggest on the vector
    if (vect->at(last) < item) {
        vect->push_back(item);
        return false;
    }

    // if new item is smaller than the smallest on the vector
    if (item < vect-> at(first)) {
        vect->insert(vect->begin(), item);
        return false;
    }
    
    while (first <= last) {
        mid = first + (last - first) / 2;
        // if item is on vector 
        if (vect->at(mid) == item) {
            return true;
        } else if (item < vect->at(mid)) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }

    // Insert it in the correct spot
    vect->insert(vect->begin() + first, item);
    return false;
}
 
void EliminateDuplicates (char *infile, unsigned char field,
                          block_t *buffer, unsigned int nmem_blocks,
                          char *outfile, unsigned int *nunique, unsigned int *nios) {
    FILE *in, *out;

    // Files
    in = fopen(infile, "r");
    if (in == NULL) {
        cerr << "Could not open input file." << endl;
    }

    out = fopen(outfile, "w");
    if (out == NULL) {
        cerr << "Could not open output file." << endl;
    }

    // buffer is a place in memory with size assigned by the user in witch
    // blocks are stored
    buffer = new block_t[nmem_blocks];

    // The vectors containing the unique values
    vector<string> strVect;
    vector<uint> numVect;

    uint i = 0;
    
    *nios = 0;
    *nunique = 0;
    
    while(!feof(in)) {
        while (i < nmem_blocks && !feof(in)) {
            fread(&buffer[i], 1, sizeof(block_t), in);
            ++*nios;
            for (uint j = 0; j < buffer[i].nreserved; j++) { 
                switch (field) {
                case '0': // User selected record
                        if (searchAndInsert(&numVect, buffer[i].entries[j].recid, 0, numVect.size() == 0 ? 0 : numVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                case '1': // User selected number
                    if (searchAndInsert(&numVect, buffer[i].entries[j].num, 0, numVect.size() == 0 ? 0 : numVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                case '2': // User selected string
                    if (searchAndInsert(&strVect, string(buffer[i].entries[j].str), 0, strVect.size() == 0 ? 0 : strVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                case '3': // User selected numstr
                    if (searchAndInsert(&numVect, buffer[i].entries[j].num, 0, numVect.size() == 0 ? 0 : numVect.size()-1) &&
                        searchAndInsert(&strVect, string(buffer[i].entries[j].str), 0, strVect.size() == 0 ? 0 : strVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                default:
                    cerr << "wrong filed" << endl;
                    exit(1);
                }
            }
            // write file
            fwrite(&buffer[i], 1, sizeof(block_t), out);
            ++*nios;
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
