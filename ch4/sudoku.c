#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct cell{
	int row;
	int col;
}Cell;

int valid_sets[27] = {0};
int sudoku[9][9] = {
		{6,2,4,5,3,9,1,8,7},
		{5,1,9,7,2,8,6,3,4},
		{8,3,7,6,1,4,2,9,5},
		{1,4,3,8,6,5,7,2,9},
		{9,5,8,2,4,7,3,6,1},
		{7,6,2,3,9,1,4,5,8},
		{3,7,1,9,5,6,8,4,2},
		{4,9,6,1,8,2,5,7,3},
		{2,8,5,4,7,3,9,1,6}
	};

void* validate_row(void* cell);
void* validate_column(void* cell);
void* validate_square(void* cell);

int main() {
	// Cell to start from
	Cell* cell = malloc(sizeof(Cell));
	pthread_t tid[27];

	// Init board with random vals
	srand(time(NULL));
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			sudoku[i][j] = rand() % 9 + 1;
			printf("%d ", sudoku[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	// Validate rows.
	for (int i = 0; i < 9; i++) {
		cell->row = i;
		cell->col = 0;
		pthread_create(&tid[i], NULL, validate_row, cell);
	}

	// Validate columns.
	for (int i = 0; i < 9; i++) {
		cell->row = 0;
		cell->col = i;
		pthread_create(&tid[i+9], NULL, validate_column, cell);
	}

	// Validate 3x3 squares.
	int tcount = 0;
	for (int i = 0; i < 9; i+=3) {
		for (int j = 0; j < 9; j+=3) {
			cell->row = i;
			cell->col = j;
			pthread_create(&tid[tcount+18], NULL, validate_square, cell);
			tcount++;
		}
	}

	// Collect results.
	for (int i = 0; i < 27; i++) {
		pthread_join(tid[i], NULL);
	}

	// Determine if suduko is valid.
	int invalid_count = 0;
	for (int i = 0; i < 27; i++) {
		if (!valid_sets[i]) {
			if (!invalid_count) {
				printf("Sudoku is invalid\n");
				printf("Invalid sets: ");
			}
			else {
				printf("%d ", i);
			}
			invalid_count++;
		}
	}
	printf("\n");

	if (!invalid_count)
		printf("Hurray! Sudoku is valid\n");

	free(cell);

	return 0;
}

void* validate_row(void* cell) {
	Cell* pCell = cell;
	int found[9] = {0};
	int row = pCell->row;
	int num;
	static int tcount = 0;

	for (int i = 0; i < 9; i++) {
		num = sudoku[row][i];
		// Check if we already saw that num.
		if (found[num-1]) {
			// Invalid row
			pthread_exit(0);
		}
		found[num-1] = 1;
	}

	// Success, sudoku is valid
	valid_sets[tcount] = 1;
	tcount++;
	pthread_exit(0);
}

void* validate_column(void* cell) {
	Cell* pCell = cell;
	int found[9] = {0};
	int col = pCell->col;
	int num;
	static int tcount = 0;

	for (int i = 0; i < 9; i++) {
		num = sudoku[i][col];
		// Check if we already saw that num.
		if (found[num-1]) {
			// Invalid col
			pthread_exit(0);
		}
		found[num-1] = 1;
	}

	// Success, sudoku is valid
	valid_sets[tcount+9] = 1;
	tcount++;
	pthread_exit(0);
}

void* validate_square(void* cell) {
	Cell* pCell = cell;
	int found[9] = {0};
	int col = pCell->col;
	int row = pCell->row;
	int num;
	static int tcount = 0;

	for (int i = row; i < row+3; i++) {
		for (int j = col; j < col+3; j++) {
			num = sudoku[i][j];
			// Check if we already saw that num.
			if (found[num-1]) {
				// Invalid square
				pthread_exit(0);
			}
			found[num-1] = 1;
		}
	}

	// Success, sudoku is valid
	valid_sets[tcount+18] = 1;
	tcount++;
	pthread_exit(0);
}