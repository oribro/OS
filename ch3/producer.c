#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>


int main() {
	int fd = shm_open("myMemory", O_CREAT | O_RDWR, 0666);
	ftruncate(fd, 4096);
	char* p = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Failure");
		return 1;
	}
	const char* first = "My name";
	const char* second = "is Ori";
	char* start = p;
	sprintf(p, "%s", first);
	p += strlen(first);
	sprintf(p, "%s", second);
	p += strlen(second);

	return 0;
}