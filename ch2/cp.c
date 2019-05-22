#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<errno.h> 
extern int errno; 

int main() {
	const char* prompt_message = "Please enter file to copy.";
	char src_file[10];
	char dest_file[7];
	ssize_t written = write(1, prompt_message, strlen(prompt_message));
	if (written == -1)
		return -1;
	ssize_t bytes_read; 
	while ((bytes_read = read(0, src_file, 10)) == 0){
		char* a = "Waiting";
		write(1, a, 1);
	}
	if (bytes_read == -1)
		return -1;
	int fd = access(src_file, F_OK);
    printf("Src file: %s\n", src_file);
	char a[10];
	sprintf(a, "%ld", bytes_read);
	write(1, a, 4);
	const char* not_found = "File not found.";
	if (fd == -1) {
		// print which type of error have in a code 
        printf("Error Number % d\n", errno);  
		written = write(2, not_found, strlen(not_found));
		if (written == -1)
			return -1;
		exit(-1);
	}
	// const char* no_permissions = "Unsufficient permissions.";
	// fd = access(src_file, X_OK);
	// if (fd == -1) {
	// 	written = write(2, no_permissions, strlen(no_permissions));
	// 	if (written == -1)
	// 		return -1;
	// 	exit(-1);
	// }
	const char* prompt_message2 = "Please enter the destination file.";
	written = write(1, prompt_message2, strlen(prompt_message2));
	if (written == -1)
		return -1;
	// int f = fseek(0,0,SEEK_END);
	// if (f == -1)
	// 	return -1;
	bytes_read = 0;
	while ((bytes_read = read(0, dest_file, 7)) < 2){
		char* a = "Waiting";
		write(1, a, 1);
	}
	if (bytes_read == -1)
		return -1;
	int fd_src = open(src_file, O_RDONLY);
	if (fd_src == -1) {
		char* a = "Can't open source";
		write(1, a, strlen(a));
		return -1;
	}
	int fd_dest = open(dest_file, O_WRONLY|O_CREAT|O_TRUNC);
	if (fd_dest == -1){
		char* a = "Can't open dest";
		write(1, a, strlen(a));
		return -1;
	}
	char* line = NULL;
	FILE* file = fdopen(fd_src, "r");
	if (file == NULL)
		return -1;
	ssize_t read;
	size_t len = 0;
	while ((read = getline(&line, &len, file)) != -1) {
		fd = write(fd_dest, line, strlen(line));
		if (fd == -1)
			return -1;
	}
	fd = close(fd_src);
	if (fd == -1)
		return -1;
	fd = close(fd_dest);
	if (fd == -1)
		return -1;
	const char* finished = "File copy process complete.";
	fd = write(1, finished, strlen(finished));
	if (fd == -1)
		return -1;

	return 0;
}
