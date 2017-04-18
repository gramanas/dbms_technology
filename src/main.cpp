#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

#include "./dbtproj.h"
#include "./fileHandling.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc == 1) {
        cout << "Type -h for quick help." << endl;
    } else if ( strcmp(argv[1], "help") == 0 ||
                strcmp(argv[1], "-h") == 0 ||
                strcmp(argv[1], "--help") == 0) {
        cout << "Usage:\n" << endl;
        cout << "create filename N -- Creates a random binary file named \"filename\" with N blocks." << endl;
        cout << "print filename [[r]/[b] N]  -- If no arguments are given prints whole file. " << endl;
        cout << "                               r and then a number prints record N" << endl;
        cout << "                               b and then a number pritns the whole block N" << endl;
    } else if (strcmp(argv[1], "create") == 0) {
        createRandFile(argv[2], atoi(argv[3]));
    } else if (strcmp(argv[1], "print") == 0) {
        if (argc == 3) {
            printFile(argv[2], 0, false, 0, false);
        } else if (strcmp(argv[3], "r") == 0) {
            printFile(argv[2], (uint)atoi(argv[4]), true, 0, false);
        } else if (strcmp(argv[3], "b") == 0) {
            printFile(argv[2], 0, false, (uint)atoi(argv[4]), true);
        }
    } else {
        cout << "Type -h for quick help." << endl;
    }
    return 0;
}
