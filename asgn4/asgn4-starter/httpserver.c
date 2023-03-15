// Asgn 2: A simple HTTP server.
// By: Eugene Chou
//     Andrew Quinn
//     Brian Zhao

#include "asgn2_helper_funcs.h"
#include "connection.h"

#include "response.h"
#include "request.h"
#include "queue.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <assert.h>

#include <pthread.h>


pthread_mutex_t mutex;
pthread_mutex_t auditex;
void handle_connection(uintptr_t  r);

void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);

queue_t *q;
queue_t *put_q;


void * queue_worker(){

      // fprintf(stderr, "Thread created!\n");
       while(1)  {
       uintptr_t r;
       queue_pop(q, (void **)&r);
      // fprintf(stderr, "queue popped!\n");
       handle_connection(r);
       }
}


int main(int argc, char **argv) {
	// fprintf(stderr, "Hello1\n");
	
    if (argc < 2) {
        warnx("wrong arguments: %s port_num", argv[0]);
      //  fprintf(stderr, "usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int threadcount = 0;
    size_t port;
    char *endptr;
    
    // only port
    if (argc == 2) {
     endptr = NULL;
     port = (size_t) strtoull(argv[1], &endptr, 10);
     // fprintf(stderr, "port1: %zu\n", port);
    if (endptr && *endptr != '\0') {
        warnx("invalid port number: %s", argv[1]);
        return EXIT_FAILURE;
    }  
    threadcount = 4;
    }
    
    // port and threadcount
    if (argc > 2) {
           endptr = NULL;
           port = (size_t) strtoull(argv[3], &endptr, 10);
         //   fprintf(stderr, "port1: %zu\n", port);
    if (endptr && *endptr != '\0') {
        warnx("invalid port number: %s", argv[3]);
        return EXIT_FAILURE;
    }
    char *t = argv[2];
    // fprintf(stderr, "arg1: %s\n", t);
    // char tc[10];
    // strncpy(tc,t+2,strlen(t) - 2);
    // fprintf(stderr, "t: %s\n", tc);
    threadcount = atoi(t); 
    }
    
   // fprintf(stderr, "Hello2\n");
    
    // fprintf(stderr, "Hello1\n");
    q = queue_new(10);
    put_q = queue_new(5);
    int rc = 0;
    rc = pthread_mutex_init(&mutex, NULL);
    assert (!rc);

    signal(SIGPIPE, SIG_IGN);
    Listener_Socket sock;
    listener_init(&sock, port);
    
    // fprintf(stderr, "Hello2\n");
     // fprintf(stderr, "threadcount : %d\n", threadcount);
    
  // uintptr_t rc1;
  
 pthread_t thread[threadcount];
 
 for (int i = 0; i < threadcount; i++) {
	pthread_create(&thread[i], NULL,queue_worker, NULL);
 }

 while (1) {
        uintptr_t connfd = listener_accept(&sock);
        queue_push(q, (void *) connfd);
      
    }
    assert (!rc);
    rc = pthread_mutex_destroy(&mutex);
    return EXIT_SUCCESS;
}


void handle_connection(uintptr_t  r) {
	// fprintf(stderr, "Handle Connection\n");
    conn_t *conn = conn_new(r);
    

    const Response_t *res = conn_parse(conn);
    
    // worker threads

    if (res != NULL) {
        // fprintf(stderr, "Handle Connection1\n");
        conn_send_response(conn, res);
    } else {
    	// fprintf(stderr, "Handle Connection ELse\n");
        // debug("%s", conn_str(conn));
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_GET) {
          //  fprintf(stderr, "Handle Connection2\n");
            handle_get(conn);
        } else if (req == &REQUEST_PUT) {
           // fprintf(stderr, "Handle Connection3\n");      
            handle_put(conn);
        } else {
            handle_unsupported(conn);
        }
    }

     conn_delete(&conn);
     close(r);
}

void handle_get(conn_t *conn) {
    // fprintf(stderr, "Handle Get\n");
    char *uri = conn_get_uri(conn);
    char *header = conn_get_header(conn, "Request-Id");
    // fprintf(stdout, "%s\n", header);
    const Response_t *res = NULL;
    // debug("GET request not implemented. But, we want to get %s", uri);

    // What are the steps in here?

    // 1. Open the file.
    // If  open it returns < 0, then use the result appropriately
    //   a. Cannot access -- use RESPONSE_FORBIDDEN
    //   b. Cannot find the file -- use RESPONSE_NOT_FOUND
    //   c. other error? -- use RESPONSE_INTERNAL_SERVER_ERROR
    // (hint: check errno for these cases)!

    // 2. Get the size of the file.
    // (hint: checkout the function fstat)!

    // Get the size of the file.

    // 3. Check if the file is a directory, because directories *will*
    // open, but are not valid.
    // (hint: checkout the macro "S_IFDIR", which you can use after you call fstat!)


    // 4. Send the file
    // (hint: checkout the conn_send_file function!)
    pthread_mutex_lock(&mutex);
    int fd = open(uri, O_RDONLY);
    // CHECKING IF FILE EXISTS AND CAN BE ACCESED
    
    if (fd < 0) {
       // debug("%s: %d", uri, errno);
        if (errno == EACCES) {
            res = &RESPONSE_FORBIDDEN;
            pthread_mutex_unlock(&mutex);
            goto out;
        } else if (errno == ENOENT) {
            res = &RESPONSE_NOT_FOUND;
            pthread_mutex_unlock(&mutex);
            goto out;
        } else { 
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            pthread_mutex_unlock(&mutex);
            goto out;
        }
    }
    // GET SIZE OF FILE 
    
    flock(fd, LOCK_SH);
    pthread_mutex_unlock(&mutex);
    long bytes = 0;
    struct stat file_stats;
    if (stat(uri, &file_stats) == 0) {
        bytes = file_stats.st_size;
        // fprintf(stderr, "\nsize = %ld\n", bytes);
    } else {
        fprintf(stderr, "Invalid Stat Command\n");
        res = &RESPONSE_INTERNAL_SERVER_ERROR;
        goto out;
    }
    
    // CHECK IF LOCATION IS DIRECTORY
    
    if (S_ISREG(file_stats.st_mode) == 0) {
        res = &RESPONSE_FORBIDDEN;
        goto out;
    }
    
    // send the file
     res = &RESPONSE_OK;
    // pthread_mutex_lock(&auditex);
    conn_send_file(conn, fd, bytes);
    fprintf(stderr, "GET,/%s,%hu,%s\n", uri, response_get_code(res), header);
    // pthread_mutex_unlock(&auditex);
    close(fd);
    return;
    
out:
   //pthread_mutex_lock(&auditex);
   conn_send_response(conn, res);
   fprintf(stderr, "GET,/%s,%hu,%s\n", uri, response_get_code(res), header);
  // pthread_mutex_unlock(&auditex);
   close(fd);

}

void handle_unsupported(conn_t *conn) {
    

    // send responses
    conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
}

void handle_put(conn_t *conn) {
    // fprintf(stderr, "Handle Put\n");
    char *uri = conn_get_uri(conn);
    char *header = conn_get_header(conn, "Request-Id");
    // fprintf(stderr, "%s\n", header);
    const Response_t *res = NULL;
    // debug("handling put request for %s", uri);

    // Check if file already exists before opening it.
    pthread_mutex_lock(&mutex);
    bool existed = access(uri, F_OK) == 0;
    // debug("%s existed? %d", uri, existed);

    // Open the file..
    int fd = open(uri, O_CREAT | O_WRONLY, 0600);
    
    if (fd < 0) {
       // debug("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            res = &RESPONSE_FORBIDDEN;
            pthread_mutex_unlock(&mutex);
            goto out;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            pthread_mutex_unlock(&mutex);
            goto out;
        }
    }
    flock(fd, LOCK_EX);
    ftruncate(fd, 0);
    pthread_mutex_unlock(&mutex);
    res = conn_recv_file(conn, fd);
    
    if (res == NULL && existed) {
        res = &RESPONSE_OK;
    } else if (res == NULL && !existed) {
        res = &RESPONSE_CREATED;
    }


out:
    // pthread_mutex_lock(&auditex);
    conn_send_response(conn, res);
    fprintf(stderr, "PUT,/%s,%hu,%s\n", uri, response_get_code(res), header);
    // pthread_mutex_unlock(&auditex);
    close(fd);
}
