#include "list.h"
#include "cpu.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static Node* head;

void add(char *name, int priority, int burst) {
	Task* newTask = malloc(sizeof(Task));
	newTask->name = name;
	newTask->priority = priority;
	newTask->burst = burst;
	insert(&head, newTask);
}


void schedule() {
	Node* runner;
	Task* minTask;
	int numTasks = 0;
	float turnaround = 0;
	float waiting = 0;
	int prevWaiting = 0;
	float sumTurn = 0;
	float sumWait = 0;

	while (head != NULL) {
		runner = head;
		minTask = runner->task;

		while (runner != NULL) {
			if (runner->task->burst < minTask->burst)
				minTask = runner->task;
			runner = runner->next;
		}
		run(minTask, minTask->burst);
		delete(&head, minTask);
		numTasks++;
		turnaround += minTask->burst; 
		waiting += prevWaiting;
		prevWaiting = minTask->burst;
		sumTurn += turnaround;
		sumWait += waiting;
	}

	printf("Average turnaround time: %f\n", sumTurn / numTasks);
	printf("Average waiting time: %f\n", sumWait / numTasks);
}