CC = g++
CFLAGS = -g -c -std=c++11
.C.o:
	$(CC) $(CFLAGS) $< -o $@

csim: csim.o Cache.o Simulate.o Set.o Line.o

Cache.o: Cache.h Line.h Set.h 

csim.o: Cache.h Simulate.h Line.h Set.h

Simulate.o: Cache.h Simulate.h Line.h Set.h

Set.o: Set.h Line.h

Line.o: Line.h

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f csim
