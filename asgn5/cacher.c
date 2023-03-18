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

 while ((c = getopt(argc, argv, "FLCN:")) != -1) {
        switch (c) {
        case 'N':
            sizefl = 1;
            size = atoi(optarg);
            break;
        case 'F':
            filo = 1;
            break;
        case 'L':
            LRU = 1;
            break;
        case 'C':
            clock = 1;
            break;
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
    
     char** cache = malloc(size * sizeof(char*));
     for (int i = 0; i < size; i++) {
     	cache[i] = malloc(1024 * sizeof(char));
     	}
    char buf[1024];
    int count = 0;
    int head = 0;
    int matchfl = 0;
    char evicted[1000][1024];
    int evictedcount = 0;
    int misses = 0;
    int comp = 0;
    int cap = 0;
    if (filo == 1) {
    while (fgets(buf, 1024, stdin) != NULL) {
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
    		cap++;
    		break;
    		}
    		}
    misses++;
    
    // if cache is full lifo pop and push using circular array
    
    if (count == size) {
        strcpy(evicted[evictedcount], cache[head]);
        evictedcount++;
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
    while (fgets(buf, 1024, stdin) != NULL) {
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
        }
        else {
        strcpy(evicted[evictedcount], cache[head]);
        evictedcount++;
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
     char holder[1024];
     char hholder[1024];
     while (fgets(buf, 1024, stdin) != NULL) {
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
    	  strcpy(cache[i-1], holder);
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
    		cap++;
    		break;
    		}
    		}
    misses++;
    
    strcpy(evicted[evictedcount], cache[0]);
    evictedcount++;
    if (count == size && size > 1) {
    strcpy(holder, cache[count - 2]);
    strcpy(cache[count - 2], cache[count - 1]);
    for (int i = count - 1; i > 0; i--) {
          strcpy(hholder, cache[i - 1]);
          strcpy(cache[i-1], holder);
          strcpy(holder, hholder);
    	  }
    	  strcpy(cache[count - 1], buf);
    }
    
    else {
    strcpy(cache[count], buf);
    count++;
    
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


return 0;
}
