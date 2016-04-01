/*
Parikh Shivam
ICSI 402
Assignment 5
UNIX ID: sp191221
*/ 
        
        struct studentCidNode{ // holds students courses
        	int id; // course id
        	int flag; // if 1 then course is deleted for student
        	struct studentCidNode *next; // point to next one
        };
        
        struct studentNode{ // holds student records
        	char name[MAXNAME]; // holds name
        	int numCourses; // number of courses
        	struct studentCidNode *cidHead; // pointer to courses
        	struct studentNode *next; // pointer to next
        };
        
        struct courseNode{ // holds course info
        	int courseId; // holds course id
        	int numCredits; // number of credits
        	int deleteThis; // delete the current course node
        	char schedule[MAXSCHEDULE]; // shedule for the course
        	struct courseNode *next; // pointer to next
        };
