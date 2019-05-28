#include "unix_shell.h"

void read_from_stdin(char* ptr) {
	char c;
	while ((c = getchar()) != '\n') {
		*ptr = c;
		ptr++;
	}
	*ptr = '\0';
}

/* Splits a string str into an array of strings of length argc according to flags */ 
void split(char* args[], char* str, int* argc, const char* flags) {
	char* ptr = str;
	ptr = strtok(str, flags);
	while (ptr != NULL) {
		if (args[*argc] == NULL)
			args[*argc] = (char*) malloc(sizeof(char) * (MAX_LINE/2 + 1));
		strcpy(args[*argc], ptr);
		(*argc)++;
		ptr = strtok(NULL, flags);
	}
	args[*argc] = NULL;
}

/* Operator '>' implementation */
int redirect_output(char* args[], int i) {
	int fd = open(args[i+1], O_RDWR | O_CREAT, 0777);
	if (fd == -1) {
		fprintf(stderr, "Error openning file");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	args[i] = NULL;
	args[i+1] = NULL;
	return fd;
}

/* Operator '|' implementation */
void execute_pipeline(char* args[], int i) {
	int fd[2];
	int err;
	if (pipe(fd) == -1) {
		fprintf(stderr, "Error");
		exit(1);
	}
	pid_t cpid = fork();
	if (cpid == -1) {
		fprintf(stderr, "Error");
		exit(1);
	}
	else if (cpid == 0) {
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		if ((err = execlp(args[i+1], args[i+1],args[i+2], NULL)) == -1)
			printf("Error: No such command\n");
	}
	else {
		close(fd[0]);		
		dup2(fd[1], STDOUT_FILENO);
		if ((err = execlp(args[0], args[0], NULL)) == -1)
			printf("Error: No such command\n");
	}
}

int main(void) {
	char *args[MAX_LINE/2 + 1] = {0}; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		char buf[MAX_LINE/2 + 1];
		char history[MAX_LINE/2 + 1];
		int argc = 0;
		read_from_stdin(buf);
		split(args, buf, &argc, " ");

		// History feature
		if (strcmp(buf, "!!") == 0) {
			if (*history == '\0') {
				printf("No commands in history\n");
				continue;
			}
			else {
				argc = 0;
				split(args, history, &argc, " ");
				printf("%s\n", args[0]);
			}
		}
		else {
			memcpy(history, buf, sizeof(char) * (MAX_LINE/2 + 1));
		}
		
		pid_t pid = fork();
		if (pid == -1) {
			fprintf(stderr, "Error");
			return 1;
		}
		else if (pid == 0) {
			int ret;
			int fd = -1;
			for (int i = 0; i < argc; i++) {
				if (*args[i] == '>' && args[i+1] != NULL) {
					fd = redirect_output(args, i);
					break;
				}
				else if (*args[i] == '|' && args[i+1] != NULL) {
					execute_pipeline(args, i);
					return 0;
				}
			}
			if ((ret = execvp(args[0], args)) == -1)
				printf("Error: No such command\n");
			if (fd != -1)
				close(fd);
			for (int i = 0; i < argc; i++)
				free(args[i]);
		}
		else {
			int should_wait = 1;
			for (int i = 0; i < argc; i++) {
				if (*args[i] == '&')
					should_wait = 0;
			}
			if (should_wait)
				wait(NULL);

		}
	}

	return 0;
}	