# ICSI402_p5
ZIP and Unzip program

This is the fifth and final project for CSI402 at Ualbany. As always test cases are provided thanks to proffesor Ravi. This particular project contains two parts. 

The first program is a zip/unzip tool that will zip ( but not compress in anyway) multiple files together or extract files from a created zip file. It is further exercise into binary files. It takes in any number of files. Usage for the program is
p5a flag ArchiveName FileName(s)

The flag must be either -c or -x. The -c flag is used for zipping up multiple files together into the named archive file. The -x flag is used to extract the files from the given archive name. Note that when extracting the new files will be created in the current directory. 


--------------------------------

The second program for this project was a client/server process that communicated through the use of a FIFO. This program ventures on to the unix process for creating new processes and the usage of FIFOs. The command to run this program is :
p5b_main_client initial_database final_datbase command_file log_file
 where the initial database contains the unmodified database and the final database contains the name of the file to store the modified database. The command file is the name for where the command file is stored, the log file is the same but for the log file. 
 The client program reads through the list of commands which is then sent to the server process. The server process executes the commands on the database it has stored. It will then modify the new database file appropriately and send the status of the command as well as any further information from the command to the client process. The client process will then create a log file with that information. The following commands are accepted by the program:

addc name class - This commands takes in the name of the person as well as the number for the class they are registering. It will add the given student to the correct class.
drpc name class - This will remove the given student from the given class
wdrw name - This will remove the given student from all of his classes
tcre name - This will print the total number of credits the given student is taking
newc number credits time - This will add the given class to the class database with the given information
csch number time - This will change the schdeule for the given class number to the new time
ccre number credits - This will change the number of credits for the given class to the given credits
gsch number - This will give the schedule for the given class number
gcre number - This will give the credits the given class is worth
exit - This signals to the server the commands are over and to write to the final database any changes and then exit the process. NOTE that exit is necessary at the end of your command file in order to not have a process that hangs. 
----------------------------------------

There are multiple test files included for both sections thanks for professor Ravi.
