#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>

int main() {
	int fd = shm_open("myMemory",  O_RDWR, 0666);
	char* p = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Failure");
		return 1;
	}
	printf("%s", p);
	p++;
	shm_unlink("myMemory");
	return 0;
}