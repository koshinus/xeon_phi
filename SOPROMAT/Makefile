CC=icc
CFLAGS=-c -Wall
LDFLAGS=-std=c99 -fopenmp -ls -O3
SOURCES=main.c simple_omp.c hard_omp.c func.h
OBJECTS=$(SOURCES:.c=.o .h=.o)
EXECUTABLE=main.x

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -o $@ $<
module load mpi/openmpi-x86_64 
