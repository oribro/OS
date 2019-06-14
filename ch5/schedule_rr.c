#include "list.h"
#include "cpu.h"
#include <string.h>
#include <stdlib.h>

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
	int* curBurst;
	int slice;

	toRun = head;
	while (head != NULL) {
		curBurst = &toRun->task->burst;
		if (*curBurst < QUANTUM)
			slice = *curBurst;
		else
			slice = QUANTUM;

		run(toRun->task, slice);
		
		*curBurst -= slice;
		if (*curBurst == 0)
			delete(&head, toRun->task);

		toRun = toRun->next;
		if (toRun == NULL)
			toRun = head;
	}
}