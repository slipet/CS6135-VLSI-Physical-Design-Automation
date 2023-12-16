CC=g++
CFLAG=-std=gnu++0x
SOURCES=main.cpp
EXECUTABLES=../bin/hw5

all: $(SOURCES) $(EXECUTABLES)

$(EXECUTABLES): $(SOURCES)	GlobalRouting.hpp 
	$(CC) $(CFLAG) $(SOURCES) -o $@ GlobalRouting.cpp 
	
clean:
	rm -rf *.o $(EXECUTABLES)