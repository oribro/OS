CC = gcc 
CFLAGS = -Wall -Wextra -pedantic-errors -g
LINK_SHARED = -lrt
SHARED_SRC = producer.c consumer.c time.c
SHARED_EXE = $(SHARED_SRC:.c=)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

shared: $(SHARED_EXE)
	
producer: producer.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

consumer: consumer.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

time: time.c
	$(CC) $(CFLAGS) $< $(LINK_SHARED) -o $@

clean:
	ls | grep -v -E "\.|Makefile" | xargs rm
