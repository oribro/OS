#include <sys/types.h>
#include <stdio.h>
#include<sys/wait.h>
#include <unistd.h>
#define SIZE 5
int main()
{
pid_t pid;
pid = fork();
if (pid > 0) {
	sleep(10);
}
	return 0;
}