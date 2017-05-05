#include <iostream>
#include <sys/stat.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

#include "./dbtproj.hpp"
#include "./fileHandling.hpp"

using namespace std;

int main(int argc, char** argv) {
    if (argc == 1) {
        cout << "Type -h for quick help." << endl;
    } else if ( strcmp(argv[1], "help") == 0 ||
                strcmp(argv[1], "-h") == 0 ||
                strcmp(argv[1], "--help") == 0) {
        cout << "Usage:\n" << endl;
        cout << "create filename N -- Creates a random binary file named \"filename\" with N blocks." << endl;
        cout << "clear filename -- deletes invalid records fromo binary file named \"filename\"" << endl;
        cout << "print filename [c/[[r]/[b] N]]  -- If no arguments are given prints whole file. " << endl;
        cout << "                               r and then a number prints record N" << endl;
        cout << "                               b and then a number pritns the whole block N" << endl;
        cout << "                               c to count the valid entries" << endl;
        cout << "sort infile field #memory_blocks outfile" << endl;
        cout << "                               External Merge Sort of file in filed using #memory_blocks" << endl;
        cout << "                               The field can be [record, number, string, numstr]" << endl;
        cout << "                               #memory_blocks is an integer" << endl;
        cout << "dupl infile field #memory_blocks outfile" << endl;
        cout << "                               This eliminates duplciates in the field specified" << endl;
        cout << "                               The field can be [record, number, string, numstr]" << endl;
        cout << "                               #memory_blocks is an integer" << endl;
    } else if (strcmp(argv[1], "create") == 0) {
        createRandFile(argv[2], atoi(argv[3]));
    } else if (strcmp(argv[1], "clear") == 0){
        clearFile(argv[2]);
    } else if (strcmp(argv[1], "print") == 0) {
        if (argc == 3) {
            printFile(argv[2], 0, false, 0, false);
        } else if (strcmp(argv[3], "r") == 0) {
            printFile(argv[2], (uint)atoi(argv[4]), true, 0, false);
        } else if (strcmp(argv[3], "b") == 0) {
            printFile(argv[2], 0, false, (uint)atoi(argv[4]), true);
        } else if (strcmp(argv[3], "c") == 0) {
            cout << "Valid records: " << countValid(argv[2]) << endl;
        }
    } else if (strcmp(argv[1], "dupl") == 0) {
        uint nunique, nios;
        char field = 4; // wrong field in case user enters wrong name
        block_t *buffer = new block_t[20];
        if (strcmp(argv[3], "record") == 0) {
            field = '0';
        } else if (strcmp(argv[3], "number") == 0) {
            field = '1';
        } else if (strcmp(argv[3], "string") == 0) {
            field = '2';
        } else if (strcmp(argv[3], "numstr") == 0) {
            field = '3';
        }
        EliminateDuplicates(argv[2], field, buffer, (uint)atoi(argv[4]), argv[5], &nunique, &nios);
        cout << "# of unique " << argv[3] << "s in " << argv[5] << ":\n\t" << nunique << "\n\n";
        cout << "# of I/Os in " << argv[5] << ":\n\t" << nios << "\n\n";
    } else if (strcmp(argv[1], "sort") == 0) {
        uint nunique, nios, third;
        MergeSort(argv[2], 'a', new block_t, (uint)atoi(argv[4]), argv[5], &nunique, &nios, &third);
    }
    else {
        cout << "Type -h for quick help." << endl;
    }
    return 0;
}
