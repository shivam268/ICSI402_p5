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
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "constantsPB.h"
#include "structPB.h"
#include "externsPB.h"
#include "prototypesPB.h"
#include "globalsPB.h"

int main(int args, char *argv[])
{

int numStudents = 0; // holds number of students
int numCourses = 0; // holds number of courses

int pid; // for calling client prog

char cmdLine[LINESIZE]; // command line copy

char backupCmdLine[LINESIZE]; // backup copy for command line

char *cmd; // holds a command



FILE *serverReadFP; // file pointer for server reader file
FILE *serverWriteFP; // file pointer for server write file

int serverReadFD; // fd for server read
int serverWriteFD; // fd for server write

int stopFlag = 0; // stop the loop 

if(args != COMMANDENTRY){ // if wrong number of entries
	printf("ERROR: Wrong Number of arguments \n"); // print an error
	exit(1); // exit the prog
}


setUpDataBase(argv[INITIALFILE]); // call function to create database with initial file




// DO FIFO
if(mkfifo("readFifo", S_IRWXU | S_IRWXG | S_IRWXO) == -1){ // make file
	if(errno = EEXIST){ // if already there
	}
	else{
		printf("ERROR for FIFO \n"); // if file isn't there and error then print error
		exit(2); // exit the prog
	}
}
if(mkfifo("writeFifo", S_IRWXU | S_IRWXG | S_IRWXO) == -1){ // make file
	if(errno == EEXIST){ // if file exists
	}
	else{ // print error
		printf("ERROR for FIFO \n");
		exit(2);// exit the prog
	}
}

if((pid = fork()) == 0){ /// if 0 then call the  client program
// call it with command file and log file names
	execlp("p5b_client", "p5b_client", argv[COMMANDFILE], argv[LOGFILE], NULL);
}

serverReadFD = open("readFifo", O_RDWR); // open file
if (serverReadFD == -1){ // if error
	printf("Error Opening a file \n"); // print an error
	exit(2); // exit the prog
}


serverWriteFD = open("writeFifo", O_RDWR); // open file
if(serverWriteFD == -1){ // if error
	printf("Error Opening a file \n"); // print an error
	exit(2); // exit the prog
}

while(stopFlag == 0){ // keep reading until flag is 0
read(serverReadFD, cmdLine , LINESIZE); // read line

strcpy(backupCmdLine, cmdLine); // make a backup

//printf("FROM SERVER: %s\n", cmdLine);

cmd = strtok(cmdLine, " \t\n"); // string tok to get command

//printf("FROM SERVER COMMAND: %s\n", cmd);
 
processFunction(cmd, backupCmdLine); // process the command to do modification

if((strcmp(cmd, "exit")) == 0){ // if exit command read in
	stopFlag = 1; // set flag to 1
}

}


makeFinal(argv[FINALFILE]); // creates a final file

return 0;
}
        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* this function will take a file name as a parameter        
	opens the file and reads the number of students there are
	then creates a data base with that information
	after done with students
	it reads the number of courses there are
	and it will create a database for all the courses
*/
void setUpDataBase(char *initialFileName){
FILE *initFP; // file pointer
int tempNumStudents; // holds number of students
int tempNumCourses; // holds number of courses
int i; // loop counter
int p; // inner loop counter

int q; // loop counter for courses
int id; // holds the id for the courses
int credits; //  credits for each classes
char schedule[MAXSCHEDULE]; // chedule will be stored as a string 

char line[LINESIZE]; // holds the entire line and will use strtok
char *studentName; // holds student name


struct studentNode *tempSNode; // pointer to student node
struct studentNode *currSNode; // pointer to current student node

struct  studentCidNode *tempCidNode; // temp pointer to student class id
struct  studentCidNode *currCidNode; // current pointer to student class id

struct courseNode *tempCNode; // temp pointer for class node

if((initFP = fopen(initialFileName, "r")) == NULL){ // if file opening fails
printf("ERROR: Can't open initial file \n"); // print an error
exit(1); // exit the prog
}

fscanf(initFP, "%d", &tempNumStudents); // read the number of students

rewind(initFP); // go back up top because we want to use fgets for READING

fgets(line, LINESIZE, initFP); // ignore the first line which has number of students

/*
	run the loop for number of students times
	if the adding first student on the list make a first node in the linked list
	and also attach the classes for each students with it
	
*/
for(i = 0; i < tempNumStudents; i++){
	fgets(line, LINESIZE, initFP); // get the line
	//printf("This is LINE: %s\n", line);
	
	studentName = strtok(line, " \t"); // get the student name
	tempNumCourses = atoi(strtok(NULL, " \t")); // get the number of classes
	
	//printf("STUDENT: %s\n", studentName); 
	//printf("COURSES: %d\n", tempNumCourses);
	
	if(studentHead == NULL){ // making first student Node
		tempSNode = malloc(sizeof(struct studentNode)); // malloc space for new node
		studentHead = tempSNode; // make head point to that node, because it's first node in the array
		
		strcpy(tempSNode->name, studentName); // copy name on to node
		tempSNode->numCourses = tempNumCourses; // copy number of courses on it
		
		tempSNode->next = NULL; // set the next node to NULL
		
		// lets copy the class numbers onto the linked list
		for(p = 0; p < tempNumCourses; p++){ // create inner linked list for courses for each students
		if(tempSNode->cidHead == NULL){ // if adding first course
				tempCidNode = malloc(sizeof(struct  studentCidNode)); // make space
				tempSNode->cidHead = tempCidNode; // set the head point to it, because it's first node
				
				tempSNode->cidHead->id = atoi(strtok(NULL, " \t\n")); // get the class id from the line
				tempSNode->cidHead->flag = 0; // set to 0
				tempSNode->next = NULL; // make next pointer point to null
				
				//printf("This is TEST 1: --- %d\n", tempSNode->cidHead->id);
			}
			else{ // if there is at least one node
				currCidNode = tempSNode->cidHead; // make curr pointer to point the head node
				while(currCidNode->next != NULL){ // reach till end
					currCidNode = currCidNode->next; // point to next node each time
				}
				currCidNode->next = malloc(sizeof(struct studentCidNode)); // malloc space, creates node at end
				currCidNode = currCidNode->next; // move pointer
				
				currCidNode->id = atoi(strtok(NULL, " \t\n")); // copy the id on the node
				currCidNode->flag = 0; // set to 0
				currCidNode->next = NULL; // make next pointer point to null
				//printf("This is TEST rest: --- %d\n", currCidNode->id);
			}
		} // for loop for courses ends
		
	} // making first student node ends
	else{ // if there is already 1 student record in the linked list
		tempSNode = studentHead; // make temp point to head node
		
		while(tempSNode->next != NULL){ // reach till end
			tempSNode = tempSNode->next; // move pointer every time
		}
		
		tempSNode->next = malloc(sizeof(struct studentNode)); // malloc the space
		tempSNode = tempSNode->next; // move the pointer 
		
		tempSNode->next = NULL; // make next point to null
		
		strcpy(tempSNode->name, studentName); // copy the student name over
		tempSNode->numCourses = tempNumCourses; // copy the number of courses over
		
		
		for(p = 0; p < tempNumCourses; p++){ // create inner linked list for courses for each students
		if(tempSNode->cidHead == NULL){ // making first node in the linked list
				tempCidNode = malloc(sizeof(struct  studentCidNode)); // malloc the space
				tempSNode->cidHead = tempCidNode; // make head to point to the temp node
				
				tempSNode->cidHead->id = atoi(strtok(NULL, " \t\n")); // get the id
				tempSNode->cidHead->flag = 0; // set to 0
				tempSNode->next = NULL; // make next pointer to point to null
				
				//printf("This is TEST 1: --- %d\n", tempSNode->cidHead->id);
			}
			else{ // if there is at least one node
				currCidNode = tempSNode->cidHead; // make curr pointer to point the head 
				while(currCidNode->next != NULL){ //reach till end
					currCidNode = currCidNode->next; // move pointer
				}
				currCidNode->next = malloc(sizeof(struct studentCidNode)); // malloc the space for the node
				currCidNode = currCidNode->next; // move the pointer
				
				currCidNode->id = atoi(strtok(NULL, " \t\n")); // get the id
				currCidNode->flag = 0; // set to 0
				currCidNode->next = NULL; // make next point to null
				//printf("This is TEST rest: --- %d\n", currCidNode->id);
			}
		} // for loop for courses ends
		
	}
	
} // for loop ends

fscanf(initFP, "%d", &tempNumCourses); // read the number of courses there are

// run for number of courses times
for(q = 0; q < tempNumCourses; q++){
	fscanf(initFP, "%d %d %s", &id, &credits, schedule); // read the id, number of credits and schedule
	
	if(courseHead == NULL){ // if adding first node
		tempCNode = malloc(sizeof(struct courseNode)); // malloc the space
		courseHead = tempCNode; // make head point to it
		
		tempCNode->courseId = id; // copy the id
		tempCNode->numCredits = credits; // copy the number of credits
		strcpy(tempCNode->schedule, schedule); //copy the schedule
		
		tempCNode->next = NULL; // make next point to null
	}
	else{ // if there is at least one node
		tempCNode = courseHead; // make temp point to head
		while(tempCNode->next != NULL){ // reach till end
			tempCNode = tempCNode->next; // move the point to next node
		}
		 
		tempCNode->next = malloc(sizeof(struct courseNode)); // malloc the space for new node
		
		tempCNode = tempCNode->next; // move the pointer
		
		tempCNode->courseId = id; // copy the id
		tempCNode->numCredits = credits; // copy the number of credits
		strcpy(tempCNode->schedule, schedule);// copy the schedule
		
		tempCNode->next = NULL; // make next point to null
		
	}
	
	
}

}




void processFunction(char *cmdName, char *line){
	int classNum; // class number
	int creditNum; // credit number
	char schedule[MAXSCHEDULE]; // holds schedule
	char *temp; // temp for any string
	
	if(strcmp(cmdName, "addc") == 0){ // if addc is the command
		temp = strtok(NULL, " \t\n"); // get the name
		classNum = atoi(strtok(NULL, " \t\n")); // get class num
		addc(temp, classNum); // call function
	}
	else if(strcmp(cmdName, "drpc") == 0){ // if drpc is the command
		temp = strtok(NULL, " \t\n"); // strtok to get string 
		
		classNum = atoi(strtok(NULL, " \t\n")); // get the int
		
		if(drpc(temp, classNum) == 1){ // if function returns 1 then process error
			// error
		}
		else{
			// no error
		}
		
	}
	else if(strcmp(cmdName, "wdrw") == 0){ // if wdrw is the command
		
		temp = strtok(NULL, " \t\n"); // get the string
		//printf("TEMP---: %s\n", temp);
		
		wdrw(temp); //  call the function
	}
	else if(strcmp(cmdName, "tcre") == 0){  // if tcre is the command
		
	}
	else if(strcmp(cmdName, "newc") == 0){ // if newc is the command

		temp = strtok(line, " \t\n"); // get the string
		
		classNum = atoi(strtok(NULL, " \t\n")); // number
		//printf("CLassNUm: %d\n", classNum);
		
		creditNum = atoi(strtok(NULL, " \t\n")); // get number
		//printf("CreditNUm: %d\n", creditNum);
		
		temp = strtok(NULL, " \t\n"); // get the string again
		
		if(newc(classNum, creditNum, temp) == 0){ // if no error
			// no error
		}
		else{
			// error
		}
	}
	else if(strcmp(cmdName, "csch") == 0){  // if csch is the command
		classNum = atoi(strtok(NULL, " \t\n")); // read int
		temp = strtok(NULL, " \t\n"); // get string
		if(csch(classNum, temp) == 0){ // if no error
			// success
		}
		else{
			//error
		}
		
	}
	else if(strcmp(cmdName, "ccre") == 0){  // if ccre is the command
		classNum = atoi(strtok(NULL, " \t\n")); // get the class  number
		creditNum = atoi(strtok(NULL, " \t\n")); // get the credit num
		
		if(ccre(classNum, creditNum) == 0) {//call function to change credit
			// no error
		}
		else{
			// error
		}
	}
	else if(strcmp(cmdName, "gsch") == 0){ // if gsch is the command
		
	}
	else if(strcmp(cmdName, "gcre") == 0){ // if gcre is the command
	
	}
	else if(strcmp(cmdName, "exit") == 0){  // if exit is the command
	
	}
	else{
		// Invalid
	}
}




// function will perform the part to add new class
// if flag is 0 then it has no error
// if flag is 1 then it has error
int newc(int par1, int par2, char *schedule){
	int flag = 0;
	if((newClassCheck(par1) == 0) && par2>0){ // if it already not there
	struct courseNode *tempCNode;

	if(courseHead == NULL){ // if adding first node
		tempCNode = malloc(sizeof(struct courseNode)); // malloc the space
		courseHead = tempCNode; // make head point to it
		
		tempCNode->courseId = par1; // copy the id
		tempCNode->numCredits = par2; // copy the number of credits
		strcpy(tempCNode->schedule, schedule); //copy the schedule
		
		tempCNode->next = NULL; // make next point to null
	}
	else{ // if there is at least one node
		tempCNode = courseHead; // make temp point to head
		while(tempCNode->next != NULL){ // reach till end
			tempCNode = tempCNode->next; // move the point to next node
		}
		 
		tempCNode->next = malloc(sizeof(struct courseNode)); // malloc the space for new node
		
		tempCNode = tempCNode->next; // move the pointer
		
		tempCNode->courseId = par1; // copy the id
		tempCNode->numCredits = par2; // copy the number of credits
		strcpy(tempCNode->schedule, schedule);// copy the schedule
		
		tempCNode->next = NULL; // make next point to null
		
	}
	return flag;
}
else{
 // course already exsits
 flag = 1;
 return flag;
}
}

// function returns 0 if course doesn't exsit
// 							1 if course exists
int newClassCheck(int courseNumber){
	struct courseNode *tempCNode; //temp node
	int flag = 0; // flag to set 0 ot 1 
	
	//struct courseNode *courseHead
	
	tempCNode = courseHead; // temp points to head
	
	while(tempCNode != NULL){ // go till the end
		if(tempCNode->courseId == courseNumber){ //if there is a match
			flag = 1; // course is already there 
		}
		tempCNode = tempCNode->next; // move pointer
	}
	
	return flag; // return pointer
}

// makes final data base files
void makeFinal(char *fileName){


int i = 0; // for loop counter
int p = 0; // for loop counter
	FILE *finalFP; // file pointer
	int numStudents = 0; // number of students
	int numCourses = 0; // number of courses
	
	struct studentNode *tempSNode; //temp pointer
	
	struct studentCidNode *tempCidNode; // temp pointer
	//struct courseNode *courseHead
	struct courseNode *tempCNode = courseHead; // temp course node
	
	
if((finalFP = fopen(fileName, "w")) == NULL){ // if file opening fails
printf("ERROR: Can't open initial file \n"); // print an error
exit(1); // exit the prog
}

tempSNode = studentHead; // set to head pointer
tempCNode = courseHead; // set to head pointer
	
	while(tempSNode != NULL){ // stop if null 
		numStudents ++; // increment the number of students
		tempSNode = tempSNode->next; // move pointer
	}
	
	while(tempCNode != NULL){ // stop if null
		numCourses ++; // increment number of courses
		tempCNode = tempCNode->next; // move pointer
	}
	
	fprintf(finalFP,"%d\n", numStudents); // print number of students
	
	// print the student info
	
	
	tempSNode = studentHead; // set to head pointer
	tempCidNode = tempSNode->cidHead ; // set to head pointer
	//	struct studentCidNode *tempCidNode; // temp pointer
	for(i = 0; i< numStudents; i++){
		fprintf (finalFP, "%s", tempSNode->name); // print name
		fprintf (finalFP, " %d ", tempSNode->numCourses); // print number of courses
		
		
		while(tempCidNode != NULL){ // loop till end
			if(tempCidNode->flag == 0){ // if flag is 0
			fprintf (finalFP, " %d ", tempCidNode->id); // then print
			}
			tempCidNode = tempCidNode->next; // move pointer
		}
		fprintf (finalFP, "\n"); // print new line
		tempSNode = tempSNode->next; //move pointer
		if(tempSNode != NULL){ // if it's not null
		tempCidNode = tempSNode->cidHead; //get the pointer
		}
	}
	
	
	
	fprintf(finalFP, "\n"); // print new line
	fprintf(finalFP,"%d\n", numCourses); // print number of courses
	// print course info
	printCourseInfo(finalFP); // call this method to print all the course info
}

// this takes a file pointer and prints course info on it
void printCourseInfo(FILE *fp){
struct courseNode *cNode = courseHead; // point to couse head

while(cNode != NULL){ // stop if null
	fprintf(fp, "%d ", cNode->courseId); // print course id
	fprintf(fp, "%d ", cNode->numCredits); // print number credits
	fprintf(fp, "%s\n", cNode->schedule); // print schedule
	
	cNode = cNode->next; // move pointer
}
}

// this function will withdraw student from all classes
void wdrw(char *studentName){
	struct studentNode *tempStudentNode; // make temp node
	
	tempStudentNode =  findStudent(studentName); // call function that returns node
	
	if(tempStudentNode != NULL){ // if it's not null
		tempStudentNode->numCourses = 0; // change the number of classes to 0
		tempStudentNode->cidHead = NULL; // set the linked list of classes to NULL
		
	}
	else{
		//error -- No such student
	}
}


// return the student node if found
// or return null
struct studentNode* findStudent(char *studentName){
	struct studentNode *tempStudentNode; // temp pointer
	
	tempStudentNode = studentHead; // set the temp to head
	
	while(tempStudentNode != NULL){ // reach till end
		if(strcmp(studentName, tempStudentNode->name)==0){	 // if match found
			return tempStudentNode; // return that node
		}
		tempStudentNode = tempStudentNode->next; // move pointer
	}
	return NULL; // else return null
}

//returns node if found
// or return NULL
struct courseNode* findCourse(int course){
	struct courseNode *tempCourseNode; // temp pointer
	
	tempCourseNode = courseHead; // make it point to head
	
	while(tempCourseNode != NULL){ // if not null
		if(course == tempCourseNode->courseId){ // if match
			return tempCourseNode; // return
		}
		tempCourseNode = tempCourseNode -> next; // move pointer
	}
	return NULL; // return null at the end
}

// this function takes student name and course id
// finds student and and checks if course exists, if it does delete that
// returns 0 if success else 1
int drpc(char *sName, int courseId){
	int flag = 0; // flag
	struct studentNode *tempStudent; //temp student node
	struct studentCidNode *tempCidNode; // temp Cid node
	
	if((tempStudent = findStudent(sName)) != NULL){ // if there is a student exists
		tempCidNode = tempStudent->cidHead; // point to head of cid head
		while(tempCidNode != NULL){ // if there is a list of courses
			if(courseId == tempCidNode->id){ // if there is a match
				tempStudent->numCourses--; // decrement the number of files
				tempCidNode->flag = 1; // set flag for that class to 1
				return 0; // return 0
			}
			else{ // if class doesn't exist
				flag = 1; // set flag to 1
			}
			tempCidNode = tempCidNode->next; // move pointer
			
		}
		
		if(flag == 1){ // if flag is 1
			return 1; // return 1
		}
		
	}else{ // if student doesn't exist
		return 1; // return 1
	}
}


// return 0 if success 1 otherwise
// taks course id and schedule as a parameter
// if finds course changes the schedule
int csch(int cId, char *schedule){
	struct courseNode *tempCNode; // temp node
	
	if((tempCNode = findCourse(cId)) != NULL){ // if not null
		strcpy(tempCNode->schedule, schedule); // change to new schedule
		return 0; // return 0
	}
	else{ // otherwise
		return 1; // return 1
	}
}


// return 0 if success 1 otherwise
// taks course id and schedule as a parameter
// if finds course changes the credits
int ccre(int cId, int cr){
	struct courseNode *tempCNode; // temp node
	
	if(cr  < 0 || cr == 0){ // if it's neg
		return 1; // return 1
	}
	
	if((tempCNode = findCourse(cId)) != NULL){ // if not null
		tempCNode->numCredits = cr; // change to new schedule
		return 0; // return 0
	}
	else{ // otherwise
		return 1; // return 1
	}
}


// this function will check for valid course id
// if valid checks if student exists or not
// if student is not there and there and creates new student
// else adds onto the list of existing student
void addc(char *sName, int classNum){
struct courseNode *tempCNode; // temp course node
struct studentNode *tempSNode; // temp student node
struct studentNode *currSNode; // current student node

struct studentCidNode *tempCidNode; // temp cid node
struct studentCidNode *currCidNode; // current cid node


if((tempCNode = findCourse(classNum)) != NULL){ // if valid course

	
	if((tempSNode = findStudent(sName)) != NULL){ // student exists
	
		//there is student and course add to students list
		tempCidNode = tempSNode->cidHead; //point to the courses list
		tempSNode->numCourses++; // increment courses
		if(tempCidNode == NULL){
			// first node
			currCidNode = malloc(sizeof(struct studentCidNode)); // malloc node
			tempSNode->cidHead = currCidNode; // make head point to that
			
			currCidNode->next = NULL; // make next point to null
			
			currCidNode->id = classNum; // copy id over
			currCidNode->flag = 0; // set flag to 0
		}
		else{// there is course in the list
			while(tempCidNode->next != NULL){ // go to last node
				tempCidNode = tempCidNode->next; // move pointer
			}
			tempCidNode->next = malloc(sizeof(struct studentCidNode)); // malloc space
			tempCidNode = tempCidNode->next; // move to next one
			tempCidNode->next = NULL; // set next to null
			
			 tempCidNode->id = classNum; //copy id over
			 tempCidNode->flag = 0;  // set flag to 0
		}
	}
	else{
		// valid course but new student
		
		
		if(studentHead == NULL){ // if null
			tempSNode = malloc(sizeof(struct studentNode)); //malloc space
			studentHead = tempSNode; // head points to temp
			
			strcpy(tempSNode->name, sName); // copy name over
			tempSNode->numCourses++; // increment number of courses
			tempSNode->next = NULL; // make next point to null
			
			
			//////
		tempCidNode = tempSNode->cidHead; //point to the courses list
		if(tempCidNode == NULL){
			// first node
			currCidNode = malloc(sizeof(struct studentCidNode)); // malloc node
			tempSNode->cidHead = currCidNode; // make head point to that
			
			currCidNode->next = NULL; // make next point to null
			
			currCidNode->id = classNum; // copy id over
			currCidNode->flag = 0; // set flag to 0
		}
		else{// there is course in the list
			while(tempCidNode->next != NULL){ // go to last node
				tempCidNode = tempCidNode->next; // move pointer
			}
			tempCidNode->next = malloc(sizeof(struct studentCidNode)); // malloc space
			tempCidNode = tempCidNode->next; // move to next one
			tempCidNode->next = NULL; // set next to null
			
			 tempCidNode->id = classNum; //copy id over
			 tempCidNode->flag = 0;  // set flag to 0
		}
			//////
			
			
			
			
		}
		else{
			tempSNode = studentHead; // point to head
			while(tempSNode->next != NULL){ // if not null
				tempSNode=tempSNode->next; // move pointer
			}
			tempSNode->next = malloc(sizeof(struct studentNode)); // make space
			tempSNode = tempSNode->next; // move pointer
			tempSNode->next = NULL; // make next to null
			strcpy(tempSNode->name, sName); // copy name
			tempSNode->numCourses++; // increment number of courses
			
			
			//////
		tempCidNode = tempSNode->cidHead; //point to the courses list
		if(tempCidNode == NULL){
			// first node
			currCidNode = malloc(sizeof(struct studentCidNode)); // malloc node
			tempSNode->cidHead = currCidNode; // make head point to that
			
			currCidNode->next = NULL; // make next point to null
			
			currCidNode->id = classNum; // copy id over
			currCidNode->flag = 0; // set flag to 0
		}
		else{// there is course in the list
			while(tempCidNode->next != NULL){ // go to last node
				tempCidNode = tempCidNode->next; // move pointer
			}
			tempCidNode->next = malloc(sizeof(struct studentCidNode)); // malloc space
			tempCidNode = tempCidNode->next; // move to next one
			tempCidNode->next = NULL; // set next to null
			
			 tempCidNode->id = classNum; //copy id over
			 tempCidNode->flag = 0;  // set flag to 0
		}
			//////
			
		}
	}
}
else{
}

}






