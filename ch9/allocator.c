#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 80
#define MAX_PROCESSES 32000

typedef struct process_memory {
	int proc_id;
	size_t mem_start;
	size_t mem_end;
}ProcMem;

static ProcMem* procs[MAX_PROCESSES];

void request_contiguous_block(bool memory[], char commands[]) {
	int proc_id = atoi(commands[1]);
	size_t size =  atoi(commands[2]);
	char allocation_method = *commands[3];
	int alloc_addr = 0;

	if (allocation_method == 'F')
		alloc_addr = allocate_first_fit(memory, size);
	else if (allocation_method == 'B')
		alloc_addr = allocate_best_fit(memory, size);
	else
		alloc_addr = allocate_worst_fit(memory, size);

	if (alloc_addr == -1) {
		printf("Error. Not enough memory\n");
		return;
	}

	ProcMem* proc_mem = malloc(sizeof(ProcMem));
	proc_mem->proc_id = proc_id;
	proc_mem->mem_start = alloc_addr;
	proc_mem->mem_end = alloc_addr + size; 
	procs[proc_id-1] = proc_mem;

}

void release_contiguous_block(bool memory[], char commands[]) {
	int proc_id = atoi(commands[1]);
	ProcMem* proc_mem = procs[proc_id-1];
	for (int i = proc_mem->mem_start; i < proc_mem-> mem_end; i++)
		memory[i] = false;
	free(proc_mem);
	procs[proc_id-1] = NULL;
}

void report_memory_status(bool memory[], int mem_size) {
	int i = 0;

	while (i < mem_size) {
		if (memory[i] == true)

	}

}

void compact_unused_memory(bool memory[]);

int main(int argc, char *argv[]) {
	int should_run = 1;
	int num_bytes;
	char buf[MAX_LINE/2 + 1];
	char commands[MAX_LINE/2 + 1];
	if (argc != 2) {
		printf("Usage: allocator NUM_OF_BYTES\n");
		return 1;
	}
	num_bytes = atoi(argv[1]);
	bool memory[num_bytes];

	while (should_run) {
		printf("allocator>");
		fflush(stdout);
		fgets(buf, MAX_LINE, stdin);

	  	char* token = strtok(buf," ");
	  	int i = 0;
	  	while (token != NULL)
	 	{
	 		strcpy(&commands[i], token);
	    	if (strcmp(token, "X") == 0) {
	    		should_run = 0;
	    		break;
	    	}
	   	 	token = strtok(NULL, " ");
	   	 	i++;
	  	}
	  	commands[i] = '\0';
	  	char* command = &commands[0];
	  	if (strcmp(command, "RQ") == 0)
	  		request_contiguous_block(memory, commands);
	  	else if (strcmp(token, "RL") == 0)
	  		release_contiguous_block(memory, commands);
	  	else if (strcmp(token, "C") == 0)
	  		compact_unused_memory(memory);
	  	else if (strcmp(token, "STAT") == 0)
	  		report_memory_status(memory, num_bytes);
	  	else {
	  		printf("Unknown command\n");
	  	}

	}
	return 0;
}