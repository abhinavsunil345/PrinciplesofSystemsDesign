#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// final
int main(int argc, char **argv) {

    // parse command line
    //
    //

    int size = 0;
    int sizefl = 0;
    int filo = 0;
    int LRU = 0;
    int clock = 0;
    int c = 0;
    int evicflag = 0;

    while ((c = getopt(argc, argv, "FLCN:")) != -1) {
        switch (c) {
        case 'N':
            sizefl = 1;
            size = atoi(optarg);
            break;
        case 'F': filo = 1; break;
        case 'L': LRU = 1; break;
        case 'C': clock = 1; break;
        default: /* '?' */ exit(EXIT_FAILURE);
        }
    }

    if (filo == 0 && LRU == 0 && clock == 0) {
        fprintf(stderr, "No Cache Strategy Declared\n");
        exit(EXIT_FAILURE);
    }
    if (sizefl == 0) {
        fprintf(stderr, "No Size Entered \n");
        exit(EXIT_FAILURE);
    }

    // fprintf(stderr, "size: %d filo: %d LRU: %d Clock: %d\n", size, filo, LRU, clock);

    char **cache = malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++) {
        cache[i] = malloc(2096 * sizeof(char *));
    }
    char buf[2096];
    int count = 0;
    int head = 0;
    int matchfl = 0;
    char **evicted = malloc(1000 * sizeof(char *));
    for (int i = 0; i < 1000; i++) {
        evicted[i] = malloc(2096 * sizeof(char *));
    }
    int evictedcount = 0;
    int misses = 0;
    int comp = 0;
    int cap = 0;
    if (filo == 1) {
        while (fgets(buf, 2096, stdin) != NULL) {
            //read(0, buf, 1024);
            //fgets(buf, 1024, stdin);
            buf[strcspn(buf, "\n")] = 0;
            //fgets(buf, 1024, stdin);

            // check for matching, set match flag to 1 if found match

            for (int j = 0; j < count; j++) {
                if (strcmp(buf, cache[j]) == 0) {
                    //  printf("buf:%s cache:%s\n", buf, cache[j]);
                    matchfl = 1;
                    break;
                }
            }

            // check match flag, if match print match do nothing
            if (matchfl == 1) {
                printf("HIT\n");
            }
            // else if not matching print miss and insert into cache
            else {
                printf("MISS\n");
                for (int i = 0; i < evictedcount; i++) {
                    if (strcmp(evicted[i], buf) == 0) {
                        evicflag = 1;
                        cap++;
                        break;
                    }
                }
                misses++;

                // if cache is full lifo pop and push using circular array

                if (count == size) {
                    if (evicflag == 0) {
                        strcpy(evicted[evictedcount], cache[head]);
                        evictedcount++;
                    }
                    evicflag = 0;
                    strcpy(cache[head], buf);
                    head++;
                    if (head == size) {
                        head = 0;
                    }
                    // printf("count :%d value:%s\n", count, cache[count]);
                }

                else {
                    strcpy(cache[count], buf);
                    // printf("count :%d value:%s\n", count, cache[count]);
                    count++;
                }
            }

            // printf("Got here\n");
            // reset match flag

            matchfl = 0;
            /* printf("Queue:");
    for (int i = 0; i < count; i++) {
    	printf("%s ", cache[i]);
    }
    printf("\n");*/
        }
        comp = misses - cap;
        fprintf(stdout, "%d %d\n", comp, cap);
    }

    ///
    ///
    ///
    ///

    if (clock == 1) {
        int check[size];
        while (fgets(buf, 2096, stdin) != NULL) {
            //read(0, buf, 1024);
            //fgets(buf, 1024, stdin);
            buf[strcspn(buf, "\n")] = 0;
            //fgets(buf, 1024, stdin);

            // check for matching, set match flag to 1 if found match

            for (int j = 0; j < count; j++) {
                if (strcmp(buf, cache[j]) == 0) {
                    //  printf("buf:%s cache:%s\n", buf, cache[j]);
                    check[j] = 1;
                    matchfl = 1;
                    break;
                }
            }

            // check match flag, if match print match do nothing
            if (matchfl == 1) {
                printf("HIT\n");
            }
            // else if not matching print miss and insert into cache
            else {
                printf("MISS\n");
                for (int i = 0; i < evictedcount; i++) {
                    if (strcmp(evicted[i], buf) == 0) {
                        evicflag = 1;
                        cap++;
                        break;
                    }
                }
                misses++;

                // if cache is full lifo pop and push using circular array

                if (count == size) {
                    int d = 0;
                    while (d == 0) {
                        if (check[head] == 1) {
                            check[head] = 0;
                            head++;
                            if (head == size) {
                                head = 0;
                            }
                        } else {
                            if (evicflag == 0) {
                                strcpy(evicted[evictedcount], cache[head]);
                                evictedcount++;
                            }
                            evicflag = 0;
                            strcpy(cache[head], buf);
                            check[head] = 0;
                            head++;
                            if (head == size) {
                                head = 0;
                            }
                            d = 1;
                        }
                    }
                    // printf("count :%d value:%s\n", count, cache[count]);
                }

                else {
                    strcpy(cache[count], buf);
                    check[count] = 0;
                    // printf("count :%d value:%s\n", count, cache[count]);
                    count++;
                }
            }

            // printf("Got here\n");
            // reset match flag

            matchfl = 0;
            /* printf("Queue:");
    for (int i = 0; i < count; i++) {
    	printf("%s ", cache[i]);
    }
    printf("\n");*/
        }
        comp = misses - cap;
        fprintf(stdout, "%d %d\n", comp, cap);
    }

    ///
    ///
    ///
    ///

    if (LRU == 1) {
        char holder[2096];
        char hholder[2096];
        while (fgets(buf, 2096, stdin) != NULL) {
            //read(0, buf, 1024);
            //  fgets(buf, 1024, stdin);
            buf[strcspn(buf, "\n")] = 0;
            for (int j = 0; j < count; j++) {

                if (strcmp(buf, cache[j]) == 0) {
                    // printf("buf:%s cache:%s\n", buf, cache[j]);
                    if (j == count - 1) {
                        matchfl = 1;
                        break;
                    }

                    strcpy(holder, cache[count - 2]);
                    strcpy(cache[count - 2], cache[count - 1]);
                    for (int i = count - 2; i > j; i--) {
                        strcpy(hholder, cache[i - 1]);
                        strcpy(cache[i - 1], holder);
                        strcpy(holder, hholder);
                    }
                    strcpy(cache[count - 1], buf);
                    matchfl = 1;
                    break;
                }
            }

            if (matchfl == 1) {
                printf("HIT\n");
            }
            // else if not matching print miss and insert into cache
            else {
                printf("MISS\n");
                for (int i = 0; i < evictedcount; i++) {
                    if (strcmp(evicted[i], buf) == 0) {
                        evicflag = 1;
                        cap++;
                        break;
                    }
                }
                misses++;
                if (evicflag == 0) {
                    strcpy(evicted[evictedcount], cache[0]);
                    evictedcount++;
                }
                evicflag = 0;

                if (count == size && size > 1) {
                    // fprintf(stderr, "Evict\n");
                    strcpy(holder, cache[count - 2]);
                    strcpy(cache[count - 2], cache[count - 1]);
                    for (int i = count - 1; i > 0; i--) {
                        strcpy(hholder, cache[i - 1]);
                        strcpy(cache[i - 1], holder);
                        strcpy(holder, hholder);
                    }
                    strcpy(cache[count - 1], buf);
                }

                else {
                    if (size == 1) {
                        strcpy(cache[0], buf);
                        count = 1;
                    } else {
                        strcpy(cache[count], buf);
                        count++;
                    }
                }

                //fgets(buf, 1024, stdin);

                // check for matching, set match flag to 1 if found match
            }
            matchfl = 0;
            /*printf("Queue:");
    for (int i = 0; i < count; i++) {
    	printf("%s ", cache[i]);
    }
    printf("\n");*/
        }
        comp = misses - cap;
        fprintf(stdout, "%d %d\n", comp, cap);
    }

    //
    //

    for (int i = 0; i < size; i++) {
        free(cache[i]);
    }

    free(cache);

    for (int i = 0; i < 1000; i++) {
        free(evicted[i]);
    }

    free(evicted);

    return 0;
}
