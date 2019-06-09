#include "pid_manager.h"

static pids_ptr pids = NULL;

int allocate_map(void) {
	pids = (pids_ptr) malloc(sizeof(struct pid_vec));
	if (pids == NULL)
		return -1;
	memset(pids->ids, 0, BIT_VECTOR_SIZE);
	return 1;
}

int allocate_pid(void) {
	size_t mask = 1;
	for (int i = 0; i < BIT_VECTOR_SIZE; i++) {
		size_t* candidate = &(pids->ids)[i];
		int position = 0;
		while (mask != 0) {
			if (!(mask & *candidate)) {
				*candidate |= mask;
				return MIN_PID + i * 64 + position;
			}
			position++;
			mask = mask << 1;
		}
		position = 0;
		mask = 1;
	}
	return -1;
}

void release_pid(int pid) {
	if (pid < MIN_PID || pid > MAX_PID) {
		fprintf(stderr, "Ilegal pid num");
		return;
	}
	size_t aligned_pid = pid - MIN_PID;
	size_t cell = aligned_pid / 64;
	int offset = aligned_pid % 64;
	size_t mask = 1UL << offset;
	(pids->ids)[cell] &= ~mask;
}

int is_taken(int pid) {
	size_t aligned_pid = pid - MIN_PID;
	size_t cell = aligned_pid / 64;
	int offset = aligned_pid % 64;
	size_t mask = 1UL << offset;
	return (mask & (pids->ids)[cell]) != 0;
}

void print_pids(void) {
	printf("The following pids are in use:\n");
	for (int i = MIN_PID; i <= MAX_PID; i++) {
		if (is_taken(i))
			printf("%d, ", i);
	}
	printf("\n");
}
