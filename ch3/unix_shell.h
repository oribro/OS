#ifndef PID_MANAGER_H 
#define PID_MANAGER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 

#define MAX_LINE 80 /* The maximum length command */

void read_from_stdin(char* ptr);

void split(char* args[], char* str, int* argc, const char* flags);

int redirect_output(char* args[], int i);

void execute_pipeline(char* args[], int i);

#endif