/*
Parikh Shivam
ICSI 402
Assignment 5
UNIX ID: sp191221
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include "constantsPB.h"
#include "globalsPB.h"
#include "structPB.h"
#include "externsPB.h"
#include "prototypesPB.h"


int main(int args, char *argv[])
{

FILE *cmdFP; // command file pointer
int serverReadFPclient; // fd for server read file
int serverWriteFPclient; // fd for server write file
char cmdLine[LINESIZE]; // holds command line

char *command; // holds command after string tok

///////////////////
serverReadFPclient = open("readFifo", O_RDWR); // open with permission
if (serverReadFPclient == -1){ //if error
	printf("Error Opening a file \n"); // print an error
	exit(2);// exit
}


serverWriteFPclient = open("writeFifo", O_RDWR); // open with permission
if(serverWriteFPclient == -1){ // if error
	printf("Error Opening a file \n"); // print an error
	exit(2); // exit
}
////////////////////

if((cmdFP=fopen(argv[1], "r")) == NULL){ // open for reading
	printf("ERROR: Can't open readFifo file \n"); // print an error
	exit(1); // exit the prog
}


// read commands until the end of file
while((fgets(cmdLine, LINESIZE, cmdFP)) != NULL){

//printf("FROM CLIENT: %s\n", cmdLine);

write(serverReadFPclient, cmdLine, LINESIZE); // write to the file


}

write(serverReadFPclient, "exit", LINESIZE); // write onto file

return 0;
}





        

        

        

        
        
