/* Memory allocator for processes.
*  In my implementation I used an array of booleans for memory, however
*  according to the book it should be represented as contiguous holes instead of discrete bytes.
*  This should be fixed for easier allocation in a future version.
*  Another improvement is to keep the holes sorted by size which will make best fit and worst fit faster.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 80
#define MAX_PROCESSES 100

// Used to register process with it's allocated memory range.
typedef struct process_memory {
	int proc_id;
	size_t mem_start;
	size_t mem_end;
}ProcMem;

// Array of registered processes.
static ProcMem* procs[MAX_PROCESSES] = {NULL};

size_t allocate_first_fit(bool memory[], size_t mem_size, size_t size) {
	size_t i = 0;
	bool can_allocate = true;

	while (i < mem_size) {
		// Size out of memory bounds.
		if (i + size -1 >= mem_size) {
			return -1;
		}
		// Max range already allocated.
		if (memory[i + size - 1]) {
			i++;
			continue;
		}
		can_allocate = true;
		// Check if we can allocate size.
		for (int j = i; j < i + size; j++) {
			// Memory in range already allocated.
			if (memory[j]) {
				i = j;
				can_allocate = false;
				break;
			}
		}
		// Alocate memory.
		if (can_allocate) {
			for (int j = i; j < i + size; j++)
				memory[j] = true;
			return i;
		}
		i++;
	}
	return -1;
}

size_t allocate_best_fit(bool memory[], size_t mem_size, size_t size) {
	size_t i = 0;
	bool can_allocate = true;
	size_t min_addr = -1;
	size_t min_size = mem_size;
	size_t border = 0;

	// Search for the smallest block that's big enough.
	while (i < mem_size) {
		// Max range already allocated.
		if (memory[i + size - 1]) {
			i++;
			continue;
		}
		can_allocate = true;
		// Check if we can allocate size.
		for (int j = i; j < i + size; j++) {
			// Memory in range already allocated.
			if (memory[j]) {
				i = j;
				can_allocate = false;
				break;
			}
		}
		// Alocate memory.
		if (can_allocate) {
			// Find the border to check if the hole is minimal.
			int k = i + size;
			while(k < mem_size && !memory[k]) {
				k++;
			}
			border = k - 1;
			if (border - i < min_size) {
				min_size = border - i;
				min_addr = i;
			}
		}
		i++;
	}

	// Found a minimal allocation.
	if (min_addr != -1) {
		for (int j = min_addr; j < min_addr + size; j++) 
			memory[j] = true;
		return min_addr;
	}
	else
		return -1;
}

size_t allocate_worst_fit(bool memory[], size_t mem_size, size_t size) {
	size_t i = 0;
	size_t max_addr = -1;
	size_t max_size = 0;
	size_t start_addr;

	// Search for the largest hole.
	while (i < mem_size) {
		while (i < mem_size && memory[i])
			i++;
		start_addr = i;
		while (i < mem_size && !memory[i])
			i++;
		if (i - 1 - start_addr > max_size) {
			max_size = i - 1 - start_addr;
			max_addr = start_addr;
		}
	}

	if (max_addr != -1) {
		// Out of bounds allocation.
		if (max_addr + size - 1 > mem_size || size > max_size)
			return -1;
		for (int j = max_addr; j < max_addr + size; j++) {
			memory[j] = true;
		}
		return max_addr;
	}
	else
		return -1;
}

void request_contiguous_block(bool memory[], size_t mem_size, char* commands[]) {
	int proc_id = atoi(commands[1]);
	size_t size =  atoi(commands[2]);
	char allocation_method = *commands[3];
	size_t alloc_addr = 0;

	if (procs[proc_id-1] != NULL) {
		printf("Error. Process already allocated. Release first and try again\n");
		return;
	}

	if (allocation_method == 'F')
		alloc_addr = allocate_first_fit(memory, mem_size, size);
	else if (allocation_method == 'B')
		alloc_addr = allocate_best_fit(memory, mem_size, size);
	else
		alloc_addr = allocate_worst_fit(memory, mem_size, size);

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