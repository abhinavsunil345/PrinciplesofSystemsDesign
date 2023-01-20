#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main (){
	int fd;
	char str [800];
	char str2 [800];
	char buf;
	// char* token;
	scanf("%s", str);
	scanf("%s", str2);
	if (strcmp(str, "get") != 0 && strcmp(str, "set") != 0) {
		fprintf( stderr, "Invalid Command\n");
		exit(1);
	}
	if (strcmp(str, "get") == 0) { 
		fd = open (str2, O_RDWR);
	if (fd == -1) {
		fprintf( stderr, "Invalid Command\n");
		exit(1);
	}
	while (read(fd, &buf, 1) == 1) { 
		printf("%c", buf);
	}
	close(fd);
}
if (strcmp(str, "set") == 0) { 
		fd = open (str2, O_RDWR);
	//	printf("%d", fd);
	if (fd == -1) {
		fd = open(str2, O_RDWR|O_CREAT|O_TRUNC, 0777);
	}
	while (read(0, &buf, 1) == 1) { 
		write(fd, &buf, 1);
	}
	close(fd);
}

	return 0;
}

