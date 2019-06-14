#include "list.h"
#include "cpu.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static Node* head[10];

void add(char *name, int priority, int burst) {
	Task* newTask = malloc(sizeof(Task));
	newTask->name = name;
	newTask->priority = priority;
	newTask->burst = burst;
	insert(&head[priority-1], newTask);
}


void schedule() {
	Node* toRun;
	int numTasks = 0;
	float turnaround = 0;
	float waiting = 0;
	int prevWaiting = 0;
	float sumTurn = 0;
	float sumWait = 0;

	for (int i = 9; i >= 0; i--) {
		toRun = head[i];
		while (toRun != NULL) {
			run(toRun->task, toRun->task->burst);
			delete(&head[i], toRun->task);
			numTasks++;
			turnaround += toRun->task->burst; 
			waiting += prevWaiting;
			prevWaiting = toRun->task->burst;
			sumTurn += turnaround;
			sumWait += waiting;
			toRun = toRun->next;
		}
	}
	printf("Average turnaround time: %f\n", sumTurn / numTasks);
	printf("Average waiting time: %f\n", sumWait / numTasks);
}