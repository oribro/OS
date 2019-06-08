#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


void* output_primes_le_than(void* num);

int main(int argc, char* argv[]) {
	pthread_t tid;
	int num;

	sscanf(argv[1], "%d", &num);

	pthread_create(&tid, NULL, output_primes_le_than, &num);
	pthread_join(tid, NULL);
	return 0;
}

void* output_primes_le_than(void* num) {
	int* num_ptr = num;
	int is_prime;

	for (int i = 2; i <= *num_ptr; i++) {
		is_prime = 0;
		for (int j = 2; j < i; j++) {
			if (i % j == 0)
				is_prime = 1;
		}
		if (!is_prime)
			printf("%d ", i);
	} 
	pthread_exit(0);
}