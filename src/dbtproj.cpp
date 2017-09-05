#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "./dbtproj.hpp"

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

void Invalidate(block_t * buffer) {
    for (uint i = 0; i < buffer->nreserved; i++) {
        buffer->entries[i].valid = false;
    }
}


void MergeSort (char *infile, unsigned char field,
                block_t *buffer, unsigned int nmem_blocks,
                char *outfile, unsigned int *nsorted_segs,
                unsigned int *npasses, unsigned int *nios) {
    if (nmem_blocks < 2) {
        cerr << "Buffer must have at least 2 blocks avaliable to merge sort" << endl;
        exit(1);
    }

    FILE *in, *out;

    in = fopen(infile, "rb");
    if (in == NULL) {
        cerr << "Could not open input file." << endl;
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
    cout << "Size of block: " <<  n;
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
        int blocksInFile = nmem_blocks - 1; // number of blocks in the last read file
        for (uint i = 0; i < nmem_blocks; i++) {
            if (&buffer[i] == NULL) {
                blocksInFile = i;
                break;
            }
            Sort(&buffer[i]);
        }

        FILE *temp;
        temp = fopen(("temp" + to_string(tempNum)).c_str(), "wb");
        for (uint i = 0; i <= blocksInFile; i++) {
            fwrite(&buffer[i], 1, sizeof(block_t), temp);
            Invalidate(&buffer[i]);
        }
        fclose(temp);

        tempNum++;
        delete buffer;
    }

    cout << "nios: " << *nios << endl;

    fclose(in);

    // out = fopen(outfile, "wb");
    // if (out == NULL) {
    //     cerr << "Could not open output file." << endl;
    // }

    // // BEGIN MERGE

    // // open FOPEN_MAX files
    // // create new blocks and
    // // start filling them by merging the opened files
    // // when block is full write it to file
    // // repeat until files run out
    // // then repeat untill there is only 1 file
    // // that is the output

    // int myFOpenMax;
    // if (FOPEN_MAX < nmem_blocks) {
    //     myFOpenMax = nmem_blocks;
    // } else {
    //     myFOpenMax = FOPEN_MAX; // 16
    // }

    // int filesToMerge = tempNum;
    // int it = 0; // itteration

    // while (filesToMerge != 1) {
    //     FILE *files[myFOpenMax];
    //     buffer = new block_t[nmem_blocks];

    //     for (int i = 0; i < myFOpenMax && i < filesToMerge; i++) {
    //         files[i] = fopen(("temp" + to_string(i + it*tempNum)).c_str(), "rb");
    //         fread(&buffer[i], 1, sizeof(block_t), files[i]);
    //     }

    //     while (true) {
    //         block_t temp;
    //     }
    // }


    // fclose(out);
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
            fwrite(&buffer[i], 1, sizeof(block_t), out);
        }
    }
    cout << "Vector size: " << numVect.size()*sizeof(buffer[1].entries[1].str) << endl;
    cout << "Buffer size: " << sizeof(block_t)*nmem_blocks << endl;
    cout << "Buffer2 size: " << sizeof(*buffer) << endl;
    fclose(in);
    fclose(out);
}

/* ----------------------------------------------------------------------------------------------------------------------
   infile1: the name of the first input file
   infile2: the name of the second input file
   field: which field will be used for the join: 0 is for recid, 1 is for num, 2 is for str and 
     3 is for both num and str
   buffer: pointer to memory buffer
   nmem_blocks: number of blocks in memory
   outfile: the name of the output file
   nres: number of pairs in output (this should be set by you)
   nios: number of IOs performed (this should be set by you)
   ----------------------------------------------------------------------------------------------------------------------
*/
void MergeJoin (char *infile1, char *infile2,
                unsigned char field, block_t *buffer,
                unsigned int nmem_blocks, char *outfile,
                unsigned int *nres, unsigned int *nios) {
    // http://www.dcs.ed.ac.uk/home/tz/phd/thesis/node20.htm
    // https://www.youtube.com/watch?v=HyZtBGXLN00

    //Files initiation
    FILE *in_1,*in_2, *out;

    // Files
    in_1 = fopen(infile1, "rb");
    in_2 = fopen(infile2, "rb");
    if (in_1 == NULL || in_2 == NULL) {
        cerr << "Could not open input files." << endl;
    }

    out = fopen(outfile, "wb");
    if (out == NULL) {
         cerr << "Could not open output file." << endl;
    }
    char* inFileSorted_1 = new char(sizeof(infile1) + sizeof("_sorted"));
    inFileSorted_1 = strcat(infile1,"_sorted");
    
    uint* nsorted_segs = new uint;
    uint* npasses = new uint;
    

    // Sort the the 1st infile and take sorted file inFileSorted_1
    cout << "Sort the the 1st infile and take sorted file inFileSorted_1";
    MergeSort(infile1, field, buffer, nmem_blocks, inFileSorted_1,  nsorted_segs, npasses, nios);

    char* inFileSorted_2 = new char(sizeof(infile2) + sizeof("_sorted"));
    inFileSorted_2 = strcat(infile2,"_sorted");
        
    // Sort the the 2nd infile and take sorted file inFileSorted_2
    cout << "Sort the the 2nd infile and take sorted file inFileSorted_2";
    MergeSort(infile2, field, buffer, nmem_blocks, inFileSorted_2,  nsorted_segs, npasses, nios);

    //Now we have two sorted files and we can continue to the merge Join

    // buffer is a place in memory with size assigned by the user in witch
    // blocks are stored
    buffer = new block_t[nmem_blocks];

    

    
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
