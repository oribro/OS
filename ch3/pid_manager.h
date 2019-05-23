#ifndef PID_MANAGER_H 
#define PID_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN_PID 300
#define MAX_PID 5000
#define BIT_VECTOR_SIZE 75

struct pid_vec {
	// The vector will support up to 4800 pids = 75 size_t variables.
	size_t ids[BIT_VECTOR_SIZE];
};

typedef struct pid_vec* pids_ptr;

int allocate_map(void);

int allocate_pid(void);

void release_pid(int pid);

int is_taken(int pid);

void print_pids(void);

#endif