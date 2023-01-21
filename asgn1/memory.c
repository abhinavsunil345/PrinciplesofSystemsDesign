#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// final
int main() {
    int fd;
    char str[800];
    char str2[8000];
    char str3[8000];
    char buf[10000];
    char buf1;
    int count = 0;
    int sp = 0;
    int bytes_read = 0;
    // char* token;
    while (read(0, &buf1, 1) == 1) {
        if (count > 4096) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        if (buf1 == '\n') {
            str3[count] = '\0';
            sp++;
            break;
        }

        str3[count] = buf1;
        count++;
    }
    if (sp != 1) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }
    char *token = strtok(str3, " \t");

    if (token == NULL) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    strcpy(str, token);
    token = strtok(NULL, " \t");
    if (token == NULL) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }
    strcpy(str2, token);
    // printf("%s\n", str);
    // printf("%s\n", str3);
    // printf("%s\n", str2);

    if (strcmp(str, "get") != 0 && strcmp(str, "set") != 0) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }

    if (strcmp(str, "get") == 0) {
        int getcheck = read(0, &buf1, 1);
        if (getcheck != 0) {
            fprintf(stderr, "Invalid Command\n");
            return 1;
        }
        fd = open(str2, O_RDWR);
        if (fd == -1) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return 1;
        }
        do {
            bytes_read = read(fd, buf, 10000);
            // printf("%d", bytes_read);
            if (bytes_read < 0) {
                fprintf(stderr, "Invalid Command\n");
                return 1;
            } else if (bytes_read > 0) {
                int bytes_written = 0;
                do {
                    int bytes = write(1, buf + bytes_written, bytes_read - bytes_written);
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command\n");
                    }
                    bytes_written += bytes;
                } while (bytes_written < bytes_read);
            }
        } while (bytes_read > 0);
        close(fd);
    }
    if (strcmp(str, "set") == 0) {

        fd = open(str2, O_RDWR | O_TRUNC);
        //	printf("%d", fd);
        if (fd == -1) {
            fd = open(str2, O_RDWR | O_CREAT | O_TRUNC, 0777);
            if (read(fd, &buf1, 1) == -1) {
                fprintf(stderr, "Invalid Command\n");
                return 1;
            }
        }

        do {
            bytes_read = read(0, buf, 10000);
            // printf("%d", bytes_read);
            if (bytes_read < 0) {
                fprintf(stderr, "Invalid Command\n");
                return 1;
            } else if (bytes_read > 0) {
                int bytes_written = 0;
                do {
                    int bytes = write(fd, buf + bytes_written, bytes_read - bytes_written);
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command\n");
                    }
                    bytes_written += bytes;
                } while (bytes_written < bytes_read);
            }
        } while (bytes_read > 0);

        //while (read(0, &buf, 4096) == 4096) {
        //	write(fd, &buf, 4096);
        //}
        fprintf(stdout, "OK\n");
        close(fd);
    }

    return 0;
}
