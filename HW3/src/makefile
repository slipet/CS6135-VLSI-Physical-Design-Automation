CC=g++
CFLAG=-std=gnu++0x
SOURCES=main.cpp
EXECUTABLES=../bin/hw3

all: $(SOURCES) $(EXECUTABLES)

$(EXECUTABLES): $(SOURCES)	btree.hpp 
	$(CC) $(CFLAG) $(SOURCES) -o $@ btree.cpp 
	
clean:
	rm -rf *.o $(EXECUTABLES)