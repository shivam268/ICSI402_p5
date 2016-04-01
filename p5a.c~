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
#include "globalsPA.h"
//#include "struct.h"
#include "prototypesPA.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
///							     ///
///		              MAIN METHOD STARTS	     ///
///							     ///
////////////////////////////////////////////////////////////////


int main(int args, char *argv[])
{

int i; // for loop counter

  if(args < MINARGUMENTS){ // if entered argument is not equal to or greater than required number then
  	printf("ERROR: NUMBER of COMMAND ENTRY MISMATCH \n"); // print message
  	exit(1); // exit the program
  }
    
  // find out the flag, if invalid flag then print error and exit
  if(strcmp(argv[1], "-c")==0){ // if the flag is c
    if(args == MINARGUMENTS){ // if there are only 3 arguments then error, because there must be at least one file to zip
      printf("ERROR: wrong arguments for C flag \n"); // print error
      exit(1); // exit the program
    }
    
    fileNum = args - MINARGUMENTS; //file number
    
    
			if( (archiveFileP = fopen(argv[ARCHIVEARGUMENT], "w")) == NULL ){ // create a file for writing- if fails
			printf("error opening the for writing - Archive File \n"); //print error
			exit(1); // exit the program
			}
			
			fwrite(&fileNum, 1, 1, archiveFileP);	 // write the first byte to archive file which is number of files to compress
			
			
		for(i = 0; i < args-MINARGUMENTS; i++){ // process the files for compression 1
			processCompression1(argv[i+MINARGUMENTS]); // start from argument 4
		}	
		
		for(i = 0; i < args-MINARGUMENTS; i++){ // process the files for compression 2
			processCompression2(argv[i+MINARGUMENTS]); // start from argument 4
		}	
  }
  else if(strcmp(argv[1], "-x")==0){ // if the flag is x
    if(args != MINARGUMENTS){ // there must be only 3 arguments for X flag
      printf("ERROR: wrong arguments for X flag \n"); // print error
      exit(1); // exit the program
    }
    
    processExtraction(argv[ARCHIVEARGUMENT]); // pass the file for extraction
    
    
    extractFileCreation(); // crate needed files for extraction
    
  }
  else{
    printf("ERROR: INVALID FLAG ENTRY \n"); // print an error
    exit(1); // exit program
  }
    
    return 0; // return 0
}


///////////////////////////////////////////////////////////////
///						            ///
///			     MAIN METHOD ENDS	            ///
///				       			    ///
///////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

