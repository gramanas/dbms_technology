#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <list>
#include <algorithm>

#include "./dbtproj.h"

using namespace std;

void MergeSort (char *infile, unsigned char field,
                block_t *buffer, unsigned int nmem_blocks,
                char *outfile, unsigned int *nsorted_segs,
                unsigned int *npasses, unsigned int *nios) {
    int file = open(infile, O_RDONLY, S_IRWXU);
    if (file == -1) {
        cerr << "No such file." << endl;
        exit(1);
    }


    //TODO
    close(file);
    
}
//   field: which field will be used for sorting: 0 is for recid, 1 is for num, 2 is for str and 
//   3 is for both num and str
 
void EliminateDuplicates (char *infile, unsigned char field,
                          block_t *buffer, unsigned int nmem_blocks,
                          char *outfile, unsigned int *nunique, unsigned int *nios) {
    int file = open(infile, O_RDONLY, S_IRWXU);
    if (file == -1) {
        cerr << "No such file." << endl;
        exit(1);
    }

    cout << outfile << endl;

    int out = creat(outfile, S_IRWXU);
    if (out == -1) {
        cerr << "Can't create output file. Permissions?" << endl;
        exit(1);
    }
    
    buffer = new block_t[nmem_blocks];
    std::list<uint> list;
    
    for (uint i = 0; i < nmem_blocks; i++) {
        read(file, &buffer[i], sizeof(block_t));
        for (uint j = 0; j < buffer[i].nreserved; j++) { 
            switch (field) {
            // case 0:
                
            //     break;
            case 1:
                // if num exists in list
                if (find(list.begin(), list.end(), buffer[i].entries[j].num) != list.end()) {
                    buffer[i].entries[j].valid = false;
                } else {
                    list.push_back(buffer[i].entries[j].num);
                }
                break;
            // case 2:
                
            //     break;
            // case 3:
                
            //     break;
            default:
                cerr << "wrong filed" << endl;
                exit(1);
            }
        }
        write(out, &buffer[i], sizeof(block_t));
    }
    close(file);
    close(out);
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
