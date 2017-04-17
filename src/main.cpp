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
        cout << "print filename -- Prints \"filename\"." << endl;
    } else if (strcmp(argv[1], "create") == 0) {
        createRandFile(argv[2], atoi(argv[3]));
    } else if (strcmp(argv[1], "print") == 0) {
        printFile(argv[2]);
    } else {
        cout << "Type -h for quick help." << endl;
    }
    return 0;
}
