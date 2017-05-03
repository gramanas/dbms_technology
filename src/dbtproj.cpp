#include <iostream>
#include <stdio.h>
#include <cstdlib>
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

void RecordSwap(record_t *x, record_t *y) {
    record_t temp;
    temp.num = y->num;
    temp.rdummy1 = y->rdummy1;
    temp.rdummy2 = y->rdummy2;
    temp.rdummy3 = y->rdummy3;
    temp.recid = y->recid;
    temp.valid = y->valid;
    strcpy(temp.str, y->str);

    y->num = x->num;
    y->rdummy1 = x->rdummy1;
    y->rdummy2 = x->rdummy2;
    y->rdummy3 = x->rdummy3;
    y->recid = x->recid;
    y->valid = x->valid;
    strcpy(y->str, x->str);
    
    x->num = temp.num;
    x->rdummy1 = temp.rdummy1;
    x->rdummy2 = temp.rdummy2;
    x->rdummy3 = temp.rdummy3;
    x->recid = temp.recid;
    strcpy(x->str, temp.str);
    x->valid = temp.valid;
}

void QuickSort(block_t *buffer, uint left, uint right) {
    uint i = left, j = right;
    uint pivot = left + (right - left) / 2;

    while (i <= j) {
        if (j > right) return;
        while (buffer->entries[i].num < buffer->entries[pivot].num) {
            i++;
        }
        while (buffer->entries[pivot].num < buffer->entries[j].num) {
            j--;
        }
        if (i <= j) {
            RecordSwap(&(buffer->entries[i]), &(buffer->entries[j]));
            i++;
            j--;
        }
    }

    if (left < j && j <= right) {
        QuickSort(buffer, left, j);
    }
    if (i < right) {
        QuickSort(buffer, i, right);
    }
}

void Sort(block_t *buffer) {
    uint left = 0;
    uint right = buffer->nreserved - 1;

    QuickSort(buffer, left, right);
}

void WriteValidRecords(FILE *outfile, block_t *buffer, uint *nios) {
    for (uint i = 0; i < buffer->nreserved; i++) {
        if (buffer->entries[i].valid) {
            fwrite(&(buffer->entries[i]), 1, sizeof(record_t), outfile);
            //fseek(outfile, -sizeof(record_t), SEEK_CUR);
            record_t tmp;
            fread(&tmp, 1, sizeof(record_t), outfile);
            cout << tmp.recid << endl;
            cout << tmp.num << endl;
            cout << tmp.str << endl;
            ++*nios;
        }
    }
}

void Invalidate(block_t * buffer) {
    for (uint i = 0; i < buffer->nreserved; i++) {
        buffer->entries[i].valid = false;
    }
}

void MergeSort (char *infile, unsigned char field,
                block_t *buffer, unsigned int nmem_blocks,
                char *outfile, unsigned int *nsorted_segs,
                unsigned int *npasses, unsigned int *nios) { 
    FILE *in, *out;

    in = fopen(infile, "rb");
    if (in == NULL) {
        cerr << "Could not open input file." << endl;
    }

    out = fopen(outfile, "wb");
    if (out == NULL) {
        cerr << "Could not open output file." << endl;
    }

    // This is external Merge Sort!
    // 1. Read nmem_blocks from infile
    // 2. sort them and save to temp file
    // 3. repeat till infile is done for
    // 4. Merge the temp files.
    
    // This is external Merge Sort!
    // 1. Read nmem_blocks from infile
    // 2. sort each block and merge them all to a temp file
    // 3. repeat 1-2 till infile is done for
    // 4. Merge the temp files.
    
    //TODO
    // May need to define more FOPEN_MAX


    size_t n = sizeof(block_t);
    int tempNum = 0;
    block_t tempBlk;
    while(sizeof(block_t) == fread(&tempBlk, 1, sizeof(block_t), in)) {
        fseek(in, -sizeof(block_t), SEEK_CUR);
        buffer = new block_t[nmem_blocks];
        for (uint i = 0; i < nmem_blocks; i++) {
            n = fread(&buffer[i], 1, sizeof(block_t), in);
            if (n != sizeof(block_t)) {
                break;
            }
        }
        for (uint i = 0; i < nmem_blocks; i++) {
            if (&buffer[i] == NULL) {
                break;
            }
            Sort(&buffer[i]);
        }

        FILE * temp;
        temp = fopen(("temp" + to_string(tempNum)).c_str(), "rwb");

        for (uint i = 0; i < nmem_blocks; i++) {
            fwrite(&buffer[i], 1, sizeof(block_t), temp);
            //WriteValidRecords(temp, &buffer[i], nios);
            //Invalidate(&buffer[i]);
        }
        fclose(temp);

        tempNum++;
        free(buffer);
    }

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
    in = fopen(infile, "rb");
    if (in == NULL) {
        cerr << "Could not open input file." << endl;
    }

    out = fopen(outfile, "wb");
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

    size_t n = sizeof(block_t);
    while(n == sizeof(block_t)) {
        while (i < nmem_blocks && n == sizeof(block_t)) {
            n = fread(&buffer[i], 1, sizeof(block_t), in);
            if (n != sizeof(block_t)) {
                break;
            }
            ++*nios;
            for (uint j = 0; j < buffer[i].nreserved; j++) { 
                switch (field) {
                case '0': // User selected record
                        if (searchAndInsert(&numVect, buffer[i].entries[j].recid, 0,
                                            numVect.size() == 0 ? 0 : numVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                case '1': // User selected number
                    if (searchAndInsert(&numVect, buffer[i].entries[j].num, 0,
                                        numVect.size() == 0 ? 0 : numVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                case '2': // User selected string
                    if (searchAndInsert(&strVect, string(buffer[i].entries[j].str),
                                        0, strVect.size() == 0 ? 0 : strVect.size()-1)) {
                        buffer[i].entries[j].valid = false;
                    } else {
                        ++*nunique;
                    }
                    break;
                case '3': // User selected numstr
                    if (searchAndInsert(&numVect, buffer[i].entries[j].num, 0,
                                        numVect.size() == 0 ? 0 : numVect.size()-1) &&
                        searchAndInsert(&strVect, string(buffer[i].entries[j].str),
                                        0, strVect.size() == 0 ? 0 : strVect.size()-1)) {
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
            // TODO: Here it should write only valid entries,
            // not the whole block
            fwrite(&buffer[i], 1, sizeof(block_t), out);
            //WriteValidRecords(out, &buffer[i], nios);
        }
    }
    cout << "Vector size: " << numVect.size()*sizeof(buffer[1].entries[1].str) << endl;
    cout << "Buffer size: " << sizeof(block_t)*nmem_blocks << endl;
    cout << "Buffer2 size: " << sizeof(*buffer) << endl;
    fclose(in);
    fclose(out);
}

void MergeJoin (char *infile1, char *infile2,
                unsigned char field, block_t *buffer,
                unsigned int nmem_blocks, char *outfile,
                unsigned int *nres, unsigned int *nios) {
    // http://www.dcs.ed.ac.uk/home/tz/phd/thesis/node20.htm
    // also
    // https://www.youtube.com/watch?v=HyZtBGXLN00
    //TODO

    // Implement Merge sort first

    // Merge Sort infile1

    // Merge Sort infile2

    // Join
}

void HashJoin (char *infile1, char *infile2,
               unsigned char field, block_t *buffer,
               unsigned int nmem_blocks, char *outfile,
               unsigned int *nres, unsigned int *nios) {
    // Classic hash join I guess
    // https://en.wikipedia.org/wiki/Hash_join
    //TODO
}
