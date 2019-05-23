#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 



int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Enter a number for collatz conjecture.\n");
		return 1;
	}

	int fd = shm_open("myMemory", O_CREAT | O_RDWR, 0666);
	ftruncate(fd, 4096);
	char* p = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		printf("Failure");
		return 1;
	}

	pid_t pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork Failure\n");
		return 1;
	}

	else if (pid == 0) {
		int num = 0;
		sscanf(argv[1], "%d", &num);
		if (num < 1) {
			printf("Enter a positive number.\n");
			return 1;
		}

		char buf[10];
		while (num != 1) {
			memset(buf, 0, 10);
			sprintf(buf, "%d", num);
			sprintf(p, "%d, ", num);
			p += strlen(buf) + 2;
			if (num % 2)
				num = num * 3 + 1;
			else
				num = num / 2;
		}

		sprintf(p, "%d", 1);

	}

	else {
		wait(NULL);
		printf("%s\n", p);
		shm_unlink("myMemory");
	}

	return 0;
}