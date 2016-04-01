#Parikh Shivam
#ICSI 402 Assignment 5
#UNIX ID: sp191221

#The following rule tells make about possible suffixes
#(extensions) of file names.

.SUFFIXES: .c .o

#The following definition of CC ensures that
#gcc will be used to compile the C source files.

CC = gcc

#The following definition of CFLAGS ensures that
#the debugger can be used with the executable file (sample)
#created by running make.

CFLAGS = -g

#The following rule tells make how a ".o" file should
#be created from the corresponding ".c" file.
#Note that the "-c" option must be used here since we are
#compiling source files separately. (Note that the line
#following the ".c.o:" line begins with the "tab" character.)

.c.o:
	$(CC) $(CFLAGS) -c $<

#Dependency rule for the default target and how the
#default target is to be created. (Note that the line
#following the dependency rule begins with the "tab"
#character.)

p5all: p5a p5b_server p5b_client

p5a:     p5a.o  func1PA.o func2PA.o
	        gcc p5a.o func1PA.o func2PA.o -o p5a
	        
p5b_server:     p5b_server.o
	        gcc p5b_server.o -o p5b_server

p5b_client:     p5b_client.o
		gcc p5b_client.o -o p5b_client        


#Dependency rules for other targets. (We don't need to
#specify how these targets are created since we have already
#given a general rule for creating a ".o" file from the
#corresponding ".c" file.)

p5a.o:  globalsPA.h constantsPA.h prototypesPA.h structPA.h

func1PA.o:  externsPA.h constantsPA.h structPA.h prototypesPA.h

func2PA.o:  externsPA.h constantsPA.h prototypesPA.h structPA.h



p5b_server.o: globalsPB.h constantsPB.h prototypesPB.h structPB.h



p5b_client.o: globalsPB.h constantsPB.h structPB.h prototypesPB.h

#Target for removing unnecessary files.

clean:
	rm -f *.o core
