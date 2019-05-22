#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>
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
	int fd[2];
	char buf[16];
	int bytes;
	if (pipe(fd) == -1) {
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
		sprintf(buf, "%lf", current.tv_usec / pow(10, 6) + current.tv_sec);
		printf("%s", buf);
		bytes = write(fd[1], buf, strlen(buf)+1);
		if (bytes == -1) {
			fprintf(stderr, "Write error");
			return 1;
		}
		if (argc == 2)
			execlp(op,op,NULL);
		else if (argc == 3)
			execlp(op,op, argv[2], NULL);
		else if (argc == 4)
			execlp(op,op, argv[2], argv[3], NULL);
		close(fd[0]);
		close(fd[1]);
	}
	else {
		wait(NULL);
		double start = 0;
		bytes = read(fd[0], buf, 16);
		if (bytes == -1) {
			fprintf(stderr, "Read error");
			return 1;
		}
		sscanf(buf, "%lf", &start);
		printf("Start %lf\n", start);
		gettimeofday(&current, NULL);
		double end = current.tv_usec / pow(10, 6) + current.tv_sec;
		printf("End %lf\n", end);
		double result = (end - start) ;
		printf("Elapsed time: %lf seconds\n", result);
		close(fd[0]);
		close(fd[1]);
	}
	return 0;

}