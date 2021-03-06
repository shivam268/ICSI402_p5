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
#include "constantsPA.h"
#include "structPA.h"
#include "externsPA.h"
#include "prototypesPA.h"



// process 1 makes a header file for the compression file
// take a file name as a parameter
void processCompression1(char *fileName){

unsigned char length; // holds the length of the file name
unsigned int size; // size of the file
length = strlen(fileName); // finds the length

			size = fTell(fileName); // get the size of the file
			
			
			//printf("The string length: %d", length);
			//printf(" This is argument: %s", fileName);
			//printf(" FileSize %d\n", size);  
			
			
	fwrite(&length, 1, 1, archiveFileP); // write the file name length
	fwrite(fileName, 1, length, archiveFileP); // write the file name
	fwrite(&size, 1, 4, archiveFileP); // write the file size
			
}


// this process will take a file name as a parameter
// and copies the file content onto the compress file
void processCompression2(char *fileName){
FILE *fp; // file pointer
int num = 1; // tracker of number of bytes
char buffer[BUFFERSIZE]; // buffer of size of 1000

if((fp=fopen(fileName, "r") )==NULL){ // if open fails
printf("Error opening file \n"); //  print error
exit(1); // exit the prog
}
  
  while(num != 0){ // while program doesn't read 0 bytes
  	num = fread(buffer, 1, BUFFERSIZE, fp); // read 1000 bytes and keep of track of bytes read
  	fwrite(buffer, 1, num, archiveFileP); // write the number of bytes read
  }
}


// this function take a file name as a parameter and it returns an unsigned int
// return the file size
unsigned int fTell(char *fileSize){
	FILE *fp; // file pointer for the working file
    unsigned int len; // holds the length

   if((fp = fopen(fileSize, "r"))==NULL){ // if error in opening file
      printf("Error opening file"); // print an error
      exit(1); // exit the prog
   }
   fseek(fp, 0, SEEK_END); // seek till end
 
   len = ftell(fp); // get the length
   fclose(fp); // close the file
   
   return len; // return the  length of the file
}
        
        
        
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this function will take an achive file name as a parameter
// creates a file information data base so we can read enough content to create files
void processExtraction(char *archFile){
int i; // for loop counter
unsigned char nFiles; // holds number of files

char *temp; // temp character pointer
unsigned int sizeFile; // holds size of file

if((xArchiveFileP=fopen(archFile, "r") )==NULL){ // if open fails
printf("Error opening file \n"); // print an error
exit(1); // exit the prog
}

fread(&nFiles,1,1,xArchiveFileP); // read the size of file

numberFiles = nFiles; // copy the number to temp

for(i = 0; i < numberFiles; i++){ // num for number of files times

fread(&nFiles,1,1,xArchiveFileP); // read the size of file name


temp = malloc(nFiles); // malloc space for file name

fread(temp, nFiles, 1,xArchiveFileP); // read the file name



fread(&sizeFile, 4, 1,xArchiveFileP); // read the size of file


addFileInfo(nFiles, temp, sizeFile); // process info to be added onto struct array

}

}       
        

// takes size of file name, file name, file size in bytes
// adds info on to an array
void addFileInfo(unsigned char nameSizeP1, char *fileNameP2, unsigned int  fileSizeP3){


fileStore[arrayIndex].nameSize = nameSizeP1; //copy the file name size on it

fileStore[arrayIndex].fileName = malloc(nameSizeP1); // create enough space for file name
strcpy(fileStore[arrayIndex].fileName, fileNameP2); // copy file name on it

fileStore[arrayIndex].fileSize = fileSizeP3; // copy file size 

arrayIndex ++; // increment an array index

}      
  
// this will go through the file info storage and create files for extractions
// this will also copy the content to it from compressed file

void extractFileCreation(){
int i; // for loop counter
int count = 0; // for copying the file content
FILE *file; // file pointer
char buffer[1]; // write file byte by byte
int numRead = 0; // keep track of bytes read

for(i = 0; i < numberFiles; i++){ // run for number of files there are
	
	count = 0; // reset count to 0
	numRead = 0; // reset numRead to 0
	
	if((file = fopen(fileStore[i].fileName, "w")) == NULL){ // create a file for writing- if fails
		printf("error opening the for writing \n"); //print an error
		exit(1); // exit the program
	}
			
		
	while(count != fileStore[i].fileSize){ // read byte untill last one
		numRead = fread(buffer, 1, 1, xArchiveFileP); //read 1 byte
		fwrite(buffer, 1, 1, file); // wite 1 byte
		count++; // increment count
	}
	
	
	
}
}  
        
        
