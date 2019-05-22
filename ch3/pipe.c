#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main() {
	int fd[2];
	int fd2[2];
	const char* parentmsg = "Hello son";
	const char* childmsg = "Hi father";
	char readmsg[20];
	if (pipe(fd) == -1 || pipe(fd2) == -1) {
		fprintf(stderr, "Pipe Failure");
		return 1;
	}
	pid_t pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Fork Failure");
		return 1;
	}

	else if (pid == 0) {
		memset(readmsg, 0, sizeof(readmsg));
		int bytesw = write(fd2[1], childmsg, 10);
		if (bytesw == -1) {
			fprintf(stderr, "Write Failure");
			return 1;
		}
		int bytesr = read(fd[0], readmsg, 10);
		if (bytesr == -1) {
			fprintf(stderr, "Read Failure");
			return 1;
		}
		printf("%s\n", readmsg);
		close(fd[0]);
		close(fd[1]);
		close(fd2[0]);
		close(fd2[1]);

	}

	else {
		memset(readmsg, 0, sizeof(readmsg));
		int bytesr = read(fd2[0], readmsg, 10);
		if (bytesr == -1) {
			fprintf(stderr, "Read Failure");
			return 1;
		}
		printf("%s\n", readmsg);
		int bytesw = write(fd[1], parentmsg, 10);
		if (bytesw == -1) {
			fprintf(stderr, "Write Failure");
			return 1;
		}
		close(fd[0]);
		close(fd[1]);
		close(fd2[0]);
		close(fd2[1]);

	}

	return 0;
}