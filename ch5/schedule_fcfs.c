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
	Node* toRun;
	int numTasks = 0;
	float turnaround = 0;
	float waiting = 0;
	int prevWaiting = 0;
	float sumTurn = 0;
	float sumWait = 0;

	while (head != NULL) {
		toRun = head;
		while (toRun->next != NULL) {
			toRun = toRun->next;
		}
		run(toRun->task, toRun->task->burst);
		delete(&head, toRun->task);
		numTasks++;
		turnaround += toRun->task->burst; 
		waiting += prevWaiting;
		prevWaiting = toRun->task->burst;
		sumTurn += turnaround;
		sumWait += waiting;
	}

	printf("Average turnaround time: %f\n", sumTurn / numTasks);
	printf("Average waiting time: %f\n", sumWait / numTasks);
}