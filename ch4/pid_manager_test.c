#include "../ch3/pid_manager.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 10

void* run(void*);

int main() {
	if (allocate_map() == -1) {
		fprintf(stderr, "error");
		exit(1);
	}

	pthread_t tid[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&tid[i], NULL, run, NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(tid[i], NULL);
	}
	return 0;
}

void* run(void* unused) {
	int pid;
	if ((pid = allocate_pid()) == -1) {
		fprintf(stderr, "error");
		pthread_exit(0);
	}
	srand(time(NULL)); 
	int sleep_time = rand()%20 + 1; 
	sleep(sleep_time);
	release_pid(pid);
	pthread_exit(0);
}