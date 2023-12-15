CC=g++
CFLAG=-std=gnu++0x
SOURCES=hw2.cpp
EXECUTABLES=../bin/hw2

all: $(SOURCES) $(EXECUTABLES)

$(EXECUTABLES): $(SOURCES)	linked_list.h 
	$(CC) $(CFLAG) $(SOURCES) -o $@ linked_list.cpp 
	
clean:
	rm -rf *.o $(EXECUTABLES)