#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>



int main(int argc, char* argv[]) {
	pid_t pid;
	struct timeval current;
	char* op = argv[1];
	if (argc == 1) {
		printf("Please enter function name to execute");
		return 1;
	}
	int fd = shm_open("myMemory", O_CREAT | O_RDWR, 0666);
	ftruncate(fd, 4096);
	char* p = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		shm_unlink("myMemory");
		printf("Failure");
		return 1;
	}
	pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Error");
		return 1;
	}
	else if (pid == 0) {
		gettimeofday(&current, NULL);
		sprintf(p, "%lf", current.tv_usec / pow(10, 6) + current.tv_sec);
		if (argc == 2)
			execlp(op,op,NULL);
		else if (argc == 3)
			execlp(op,op, argv[2], NULL);
		else if (argc == 4)
			execlp(op,op, argv[2], argv[3], NULL);
	}
	else {
		wait(NULL);
		double start = 0;
		sscanf(p, "%lf", &start);
		printf("Start %lf\n", start);
		gettimeofday(&current, NULL);
		double end = current.tv_usec / pow(10, 6) + current.tv_sec;
		printf("End %lf\n", end);
		double result = (end - start) ;
		printf("Elapsed time: %lf seconds\n", result);
		shm_unlink("myMemory");

	}
	return 0;

}