#include "pid_manager.h"

static pids_ptr pids = NULL;

int allocate_map(void) {
	pids = (pids_ptr) malloc(sizeof(struct pid_vec));
	if (pids == NULL)
		return -1;
	pids->ids = {0};
	return 1;
}

int allocate_pid(void) {
	size_t mask = 1;
	for (int i = 0; i < BIT_VECTOR_SIZE; i++) {
		size_t candidate = (pids->ids)[i];
		int position = 0;
		while (mask != 0) {
			if (!(mask & candidate))
				return PID_MIN + i * 64 + position;
			position++;
			mask = mask << 1;
		}
		position = 0;
		mask = 1;
	}
	return -1;
}

void release_pid(int pid) {
	size_t aligned_pid = pid - PID_MIN;
	size_t cell = aligned_pid / 64;
	int offset = cell % 64;
	int mask = 1 << offset;
	cell &= ~mask;
}

int main() {

}