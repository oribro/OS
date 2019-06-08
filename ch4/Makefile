CC = gcc
CPP = g++
CFLAGS = -Wall -Wextra -pedantic-errors -g
OPENMP = -fopenmp
THREAD = -pthread

%: %.c
	$(CC) $(THREAD) $(CFLAGS) $< -o $@

%: %.java
	javac $< 
	java $@

pthreads: pthreads.c
	$(CC) $(THREAD) $(OPENMP) $(CFLAGS) $< -o $@

ex: ex.c
	$(CC) $(THREAD) $(CFLAGS) $< -o $@

calculate: calculate.c
	$(CC) $(THREAD) $(CFLAGS) $< -o $@

pi: pi.cpp
	$(CPP) $(THREAD) $(OPENMP) $< -o $@

clean:
	ls | grep -v -E "\.|Makefile" | xargs rm -f *.o *.class

