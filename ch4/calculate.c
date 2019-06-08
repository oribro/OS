#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

float avg;
int min;
int max;

struct arr_struct {
	int n;
	int* arr;
};

void* calcMin(void* arr_s);
void* calcMax(void* arr_s);
void* calcAvg(void* arr_s);

int main(int argc, char* argv[]) {
	pthread_t tid[3];
	int arr[argc-1];
	struct arr_struct* arr_s;

	for (int i = 1; i < argc; i++)
		sscanf(argv[i], "%d", &arr[i-1]);

	arr_s = malloc(sizeof(struct arr_struct));
	arr_s->n = argc - 1;
	arr_s->arr = arr;
	pthread_create(&tid[0], NULL, calcMin, arr_s);
	pthread_create(&tid[1], NULL, calcMax, arr_s);
	pthread_create(&tid[2], NULL, calcAvg, arr_s);

	for (int i = 0; i < 3; i++)
		pthread_join(tid[i], NULL);

	printf("min: %d max: %d avg: %f\n", min, max, avg);

	free(arr_s);
	return 0;
}

void* calcMin(void* arr_s) {
	struct arr_struct* arr_p = arr_s;
	int temp = INT_MAX;
	int* arr = arr_p->arr;

	for (int i = 0; i < (int) arr_p->n; i++) {
		if (arr[i] < temp)
			temp = arr[i];
	}
	min = temp;
	pthread_exit(0);
}

void* calcMax(void* arr_s) {
	struct arr_struct* arr_p = arr_s;
	int temp = INT_MIN;
	int* arr = arr_p->arr;

	for (int i = 0; i < (int) arr_p->n; i++) {
		if (arr[i] > temp)
			temp = arr[i];
	}
	max = temp;
	pthread_exit(0);
}

void* calcAvg(void* arr_s) {
	struct arr_struct* arr_p = arr_s;
	int sum = 0;
	int* arr = arr_p->arr;
	
	for (int i = 0; i < arr_p->n; i++) {
		sum += arr[i];
	}
	avg = sum / arr_p->n;
	pthread_exit(0);
}