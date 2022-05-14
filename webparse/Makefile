CC = g++
CFlags = -g -Wall

all: test.cpp set clean
	$(CC) $(CFlags) -o testProgram test.cpp web.h webImpl.cpp

set: set.h
	$(CC) $(CFlags) -g set.h

.PHONY: clean

clean:
	rm -f ./*.h.gch ./*~


