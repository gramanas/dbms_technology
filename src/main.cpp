#include <stdio.h>
#include <iostream>


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>

#include "dbproj.h"

using namespace std;

static int right_number_argc = 4;


/** ========= Files managments: The above fuctions helps us to create random files[Records and Blocks] ===========**/
void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

bool randomBool(){
	return rand() % 2 == 1;
}

void printFile(char* filename){

	int file = open(filename, O_RDONLY, S_IRWXU);

	block_t block;
	for(int i = 0; i < block.nreserved; i++){
		if(block.entries[i].valid){
			cout << "B_ID : " << block.blockid << endl;
			cout << "R_ID : " << block.entries[i].recid << endl;
			cout << "R_NUM : " << block.entries[i].num  << endl;
			cout << "R_STR : " << block.entries[i].str  << endl;
		}
	}
	close(file);
}


void createRandFile(char* filename, int blockNum){

	mode_t modeFile = S_IRWXU;
	int file = creat(filename,modeFile);

	record_t record;
	int record_id = 0;

	block_t block;

	srand(time(NULL));

	char* temp_str;
	temp_str = (char*)malloc(STR_LENGTH);
	if (temp_str == NULL) exit (1);

	for(int i = 0; i < blockNum; i++){ //Make blockNum blocks and put records inside
		block.blockid = i; 

		for(int j = 0; j < MAX_RECORDS_PER_BLOCK; j++){ // Put records in blocks 

				record.recid = record_id += 1;

				gen_random(temp_str,STR_LENGTH - 1);
				strcpy(record.str, temp_str);

				record.num = rand() % 20000; //Give to the record an random number
				record.valid = randomBool();
				memmove(&block.entries[j], &record, sizeof(record_t));

		}
		
		block.nreserved = MAX_RECORDS_PER_BLOCK;
		block.valid = true;
		write(file, &block, sizeof(block_t));
	}
	close(file);
}



/**======================= END OF FILE MANAGMENT ================= **/


int main(int argc, char** argv){
	
	if(argc != right_number_argc){
		cout << " Please type a right input files";
		return 0;
	}
	
	char* infile1;
	infile1 = argv[1]; 
	char* infile2;
	infile2 =  argv[2];
	char* outfile;
	outfile = argv[3];

	cout << "================ FILE 1 (START) ======================" << endl;
	createRandFile(infile1,10);
	printFile(infile1);
	cout << "================ FILE 1 (STOP) ======================" << endl;



}


    Contact GitHub API Training Shop Blog About 

