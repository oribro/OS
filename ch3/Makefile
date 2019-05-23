CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g
LINK_SHARED = -lrt
SHARED_SRC = producer.c consumer.c time.c collatz_conjecture.c
SHARED_EXE = $(SHARED_SRC:.c=)
HEADERS = pid_manager.h

%: %.c
	$(CC) $(CFLAGS) $< -o $@

shared: $(SHARED_EXE)
	
producer: producer.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

consumer: consumer.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

time: time.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

collatz_conjecture: collatz_conjecture.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

pid_manager: pid_manager.o
	$(CC) $(CFLAGS) $< -o $@
	
pid_manager.o: pid_manager.c pid_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	ls | grep -v -E "\.|Makefile" | xargs rm -f *.o

