#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asgn2_helper_funcs.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char **argv) {
	char buf[4097];	
	 int bytes_read = 0;
	char* portc;
	if (argc > 2) {
	printf("Hello");
    	// print_usage(argv[0]);
    	return 1;
  	}
  	portc = argv[1];
  	int port = atoi(portc);
  	if (port < 1 || port > 65535) {
            fprintf(stderr, "Invalid Port\n");
            return 1;
        }
  	int sockfd = 1;
  	Listener_Socket sok;
  	sockfd = listener_init(&sok, port);
  	if (sockfd == 1) {
            fprintf(stderr, "Invalid Port\n");
            return 1;
        }
       // int listened = 0;
        while(1) {
        int listened = listener_accept(&sok);
        do {
            bytes_read = read(listened, buf, 4097);
            // printf("%d", bytes_read);
            if (bytes_read < 0) {
                fprintf(stderr, "Invalid Command\n");
                return 1;
            } else if (bytes_read > 0) {
                int bytes_written = 0;
                do {
                    int bytes = write(listened, buf + bytes_written, bytes_read - bytes_written);
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command\n");
                    }
                    bytes_written += bytes;
                } while (bytes_written < bytes_read);
            }
        } while (bytes_read > 0);
        close(listened);
        }

    return 0;
}
