CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g
OPENMP = -fopenmp
THREAD = -pthread

%: %.c
	$(CC) $(CFLAGS) $< -o $@

%: %.java
	javac $< 
	java $@

pthreads: pthreads.c
	$(CC) $(THREAD) $(OPENMP) $(CFLAGS) $< -o $@

ex: ex.c
	$(CC) $(THREAD) $(CFLAGS) $< -o $@

clean:
	ls | grep -v -E "\.|Makefile" | xargs rm -f *.o *.class

