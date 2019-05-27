#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 

#define MAX_LINE 80 /* The maximum length command */
int main(void) {
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	char* history = NULL;
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		char c;
		char buf[MAX_LINE/2 + 1];
		char* str = buf;
		int argc = 0, j = 0;
		while ((c = getchar()) != '\n') {
			if (c == ' ') {
				buf[j] = '\0';
				strcpy(args[argc], str);
				str += j + 1;
				argc++;
			}
			else {
				buf[j] = c;
			}
			j++;
		}
		buf[j] = '\0';
		if (args[argc] == NULL)
			args[argc] = (char*) malloc(sizeof(char) * (MAX_LINE/2 + 1));
		strcpy(args[argc], str);
		argc++;
		
		args[argc] = NULL;

		// History feature
		if (strcmp(buf, "!!") == 0) {
			if (history == NULL) {
				printf("No commands in history\n");
				continue;
			}
			else {
				char* history_ptr = history;
				argc = 0;
				while (*history_ptr != '\0') {
					printf("%s ", history_ptr);
					strcpy(args[argc], history_ptr);
					history_ptr += strlen(history_ptr) + 1;
					argc++;
				}
				args[argc] = NULL;
				printf("\n");
			}
		}
		else {
			history = (char*) malloc(sizeof(char) * (MAX_LINE/2 + 1));
			strcpy(history, buf);
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
					args[1] = NULL;
					args[2] = NULL;
					break;
				}
			}
			if ((ret = execvp(args[0], args)) == -1)
				printf("Error: No such command\n");
			if (fd != -1)
				close(fd);
		}
		else {
			int should_wait = 1;
			for (int i = 0; i < j; i++) {
				if (buf[i] == '&')
					should_wait = 0;
			}
			if (should_wait)
				wait(NULL);

		}
	}

	// Release allocated memory
	int i = 0;
	while (args[i] != NULL) {
		free(args[i]);
		i++;
	}
	free(history);
	return 0;
}	