P=files
OBJECTS=
CFLAGS= -g -Wall -O3
LDLIBS=
CC=gcc

$(P): $(OBJECTS)

clean:
	rm -rf files files.dSYM 
