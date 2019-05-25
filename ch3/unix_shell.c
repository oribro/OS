#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command */
int main(void) {
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
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
		strcpy(args[argc], str);
		argc++;
		
		args[argc] = NULL;
		
		pid_t pid = fork();
		if (pid == -1) {
			fprintf(stderr, "Error");
			return 1;
		}
		else if (pid == 0) {
			execvp(args[0], args);
		}
		else {
			int should_wait = 1;
			for (int i = 0; i < argc; i++) {
				if (*(args[i]) == '&')
					should_wait = 0;
			}
			if (should_wait)
				wait(NULL);
		}
	}

/**
* After reading user input, the steps are:
* (1) fork a child process using fork()
* (2) the child process will invoke execvp()
* (3) parent will invoke wait() unless command included &
*/
	return 0;
}	