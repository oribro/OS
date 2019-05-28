#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 

#define MAX_LINE 80 /* The maximum length command */
int main(void) {
	char *args[MAX_LINE/2 + 1] = {0}; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		char c;
		char buf[MAX_LINE/2 + 1];
		char history[MAX_LINE/2 + 1];
		char* str = buf;
		int argc = 0;
		while ((c = getchar()) != '\n') {
			*str = c;
			str++;
		}
		*str = '\0';
		str = strtok(buf, " ");
		while (str != NULL) {
			if (args[argc] == NULL)
				args[argc] = (char*) malloc(sizeof(char) * (MAX_LINE/2 + 1));
			strcpy(args[argc], str);
			argc++;
			str = strtok(NULL, " ");
		}
		
		args[argc] = NULL;

		// History feature
		if (strcmp(buf, "!!") == 0) {
			if (*history == '\0') {
				printf("No commands in history\n");
				continue;
			}
			else {
				char* token;
				token = strtok(history, " ");
				argc = 0;
				while (token != NULL) {
					if (args[argc] == NULL)
						args[argc] = (char*) malloc(sizeof(char) * (MAX_LINE/2 + 1));
					strcpy(args[argc], token);
					argc++;
					token = strtok(NULL, " ");
				}
				args[argc] = NULL;
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
					fd = open(args[i+1], O_RDWR | O_CREAT, 0777);
					dup2(fd, STDOUT_FILENO);
					args[i] = NULL;
					args[i+1] = NULL;
					break;
				}
				else if (*args[i] == '|' && args[i+1] != NULL) {
					int fd[2];
					int err;
					if (pipe(fd) == -1) {
						fprintf(stderr, "Error");
						return 1;
					}
					pid_t cpid = fork();
					if (cpid == -1) {
						fprintf(stderr, "Error");
						return 1;
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