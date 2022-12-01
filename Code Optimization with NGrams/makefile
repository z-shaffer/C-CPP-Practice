CC = g++ 

#Use these flags for debugging
#CFLAGS = $(PFLAG) -c -std=c++11 -g

#Use these flags when the code is correct
CFLAGS = $(PFLAG) -c -std=c++11 -O2

LFLAGS = $(PFLAG)

.C.o:
	$(CC) $(CFLAGS) $< -o $@

#You can modify this makefile to support more 
#ngram versions.  Don't forget to also add the name
#of the target to the clean command.
#You will also need to change main.C.

ngramfast:  NgramFast.o libngram.a main.C
	$(CC) $(CFLAGS) -DNGRAMFAST main.C -o main.o
	$(CC) $(LFLAGS) NgramFast.o main.o libngram.a -o ngramfast

ngramslow: NgramSlow.o libngram.a main.C
	$(CC) $(CFLAGS) -DNGRAMSLOW main.C -o main.o
	$(CC) $(LFLAGS) NgramSlow.o main.o libngram.a -o ngramslow

ngramstl: NgramSTL.o libngram.a main.C
	$(CC) $(CFLAGS) -DNGRAMSTL main.C -o main.o
	$(CC) $(LFLAGS) NgramSTL.o main.o libngram.a -o ngramstl

NgramFast.o: NgramFast.h WordList.h

NgramSlow.o: NgramSlow.h WordList.h

NgramSTL.o: NgramSTL.h WordList.h

clean:
	-rm -f *.o ngramfast ngramslow ngramstl gmon.out 

