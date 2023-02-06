#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asgn2_helper_funcs.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <regex.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE PATH_MAX

/**
 *  This parses any line that does the following:
 * (1) starts with a word that uses any number of a-zA-Z characters.
 * (2) contains a space
 * (3) has a word that starts with a '/' and contains any number of
       characters in [a-ZA-Z0-9._].
 * (4) has a newline
 */


#define PARSE_REGEX "^([A-zA-Z]*) /([a-zA-Z0-9._]*)\n"
#define PARSE "^([A-zA-Z]*) /([a-zA-Z0-9._/]*) (HTTP/[1-9].[1-9])\r\n([a-zA-Z0-9.-]*: [a-zA-Z0-9:._/*-~><]*\r\n)*\r\n((.|\n)*)"
#define GET_REGEX "^([A-zA-Z]*) HTTP/[1-9].[1-9]\r\n(([a-zA-Z0-9._]*) ([a-zA-Z0-9._]*)\r\n)*\r\n"

// #define PARSE_REGEX "^([A-zA-Z]*)"

typedef struct {
	char* key;
	int value;
	} keyval;

typedef struct {

  // Make an *extra* spot for a null terminator!
  char  buf[BUFFER_SIZE + 1];
  uint16_t bufsize;
  char *command;
  char *location;
  char* ver;
  char *head;
  char *mes;
  keyval cl;
} Command;


enum StatusCode {
	ok = 200,
	created = 201,
	bad_request = 400,
	forbidden = 403,
	not_found = 404,
	internal_service_error = 500,
	not_implemented = 501,
	version_not_supported = 505,

};

char* retCode(int code) {
	switch(code) {
	
        	case ok :
         	return "200 OK";
         	break;
         	
         	case created :
         	return "201 Created";
         	break;
         	
         	case bad_request :
         	return "400 Bad Request";
         	break;
         	
         	case forbidden :
         	return "403 Forbidden";
         	break;
         	
         	case not_found :
         	return "404 Not Found";
         	break;
         	
         	case internal_service_error :
         	return "500 Internal Server Error";
         	break;
         	
         	case not_implemented :
         	return "501 Not Implemented";
         	break;
         	
         	case version_not_supported :
         	return "505 Version Not Supported";
         	break;
         	
         	}
         return "Something went Wrong";
}

int ret_string(char* ver, char* code, int listened) {
	int bytes = write(listened, ver, strlen(ver));
                    if (bytes <= 0) {
               //         fprintf(stderr, "Invalid Command7\n");
                        return 1;                  
                    }
        	bytes = write(listened, code, strlen(code));
                    if (bytes <= 0) {
                 //       fprintf(stderr, "Invalid Command8\n"); 
                        return 1;         
                    }
                bytes = write(listened, "\r\nContent-Length: ", strlen("\r\nContent-Length: "));
                    if (bytes <= 0) {
                 //       fprintf(stderr, "Invalid Command9\n");
                        return 1;       
                    }
                    return 0;
}

int get(char* location, int listened, int fd) {
	char buf[10000];
	int bytes_read = 0;
        fd = open(location, O_RDONLY);
   /*    long bytes = 0;
        struct stat file_stats;
        if( stat(location, &file_stats) == 0) {
    		bytes = file_stats.st_size;
    		fprintf(stderr, "\nsize = %ld\n", bytes);
    	}
    	else {
    		return -1;
    		}
    	
    	pass_bytes(fd, listened, bytes);
    	*/
  
        if (fd == -1) {
        //    fprintf(stderr, "Invalid Command1\n");
            close(fd);
            return not_found;
        }
        do {
            bytes_read = read(fd, buf, 10000);
            // printf("%d", bytes_read);
            if (bytes_read < 0) {
          //      fprintf(stderr, "Invalid Command2\n");
                return 1;
            } else if (bytes_read > 0) {
                int bytes_written = 0;
                do {
                    int bytes = write(listened, buf + bytes_written, bytes_read - bytes_written);
                    if (bytes <= 0) {
                //        fprintf(stderr, "Invalid Command3\n");
                    }
                    bytes_written += bytes;
                } while (bytes_written < bytes_read);
            }
        } while (bytes_read > 0);
        close(fd);
        
        return 0;

}

int put(Command* c, int listened, char* version) {
	fprintf(stderr, "here3");
	int fd;
	// int bytes_read = 0;
	// char buf1;
	int bytes;
	bool created = false;
	int x = 0;
	fd = open(c->location, O_RDWR | O_TRUNC);
        //	printf("%d", fd);
    char* haystack = c->head;
    char* needle = "Content-Length: ";
  //  char* needle2 = "\r\n";
    char *ret;
  //  char* ret2;
    ret = strstr(haystack, needle) + 16;
    ret[strlen(ret) - 2] = '\0';
    // fprintf(stderr, "\nret = l%sl\n", ret);
    if (ret == NULL) {
    		fprintf(stderr, "ret: \n%s", ret);
    		// fprintf(stderr, "\n%s", ret2);
    		fprintf(stderr, "Invalid Command ret ret\n");
    		ret_string(version, retCode(bad_request), listened);
                bytes = write_all(listened, "12\r\n\r\nBad Request\n", strlen("12\r\n\r\nBad Request\n"));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command10\n");          
                    }
                    close(fd);
    	return 1;
    }
   /* int index1 = ret - haystack;
    int index2 = ret2 - haystack;
    char sub[100];
    // int counter = 0;
    for (int i = 0; i < index2 - index1; i++) {
        sub[i] = ret[i];
     //   counter = i;
    }
   // counter++;
   // sub[counter] = '\0';*/
    x = strtol(ret, NULL, 10);
    

    c->cl.key = "Content-Length";
    c->cl.value = x;
        if (fd == -1) {
            created = true;
            close(fd);
            fd = open(c->location, O_RDWR | O_CREAT | O_TRUNC, 0777);
           /* if (read(fd, &buf1, 1) == -1) {
                     bytes = write_all(listened, "10\r\n\r\nForbidden\n", strlen("10\r\n\r\nForbidden\n"));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command10\n");          
                    }
                    close(fd);
                    return 1;
                
            }*/
            ret_string(version, retCode(201), listened);
            bytes = write_all(listened, " 8\r\n\r\nCreated\n", strlen(" 8\r\n\r\nCreated\n"));
            if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command10\n");          
                    }
            
        }
        if (created == false) {
                  ret_string(version, retCode(ok), listened);
            bytes = write_all(listened, " 3\r\n\r\nOK\n", strlen(" 3\r\n\r\nOK\n"));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command10\n");          
                    }
        }
        
        
       // fprintf(stderr, "\n%s\n", c->mes);   
        fprintf(stderr, "x = %d", x); 
        uint16_t z = c->bufsize - strlen(c->command) - strlen(c->head) - strlen(c->ver) - strlen(c->location);
        if (z < x) {
        	fprintf(stderr, "made it here frfr\n"); 
        	int rem = x - z;
        	write_all(fd, c->mes,z);
        	// char buffer[rem];
        	if (z > 3000) {
        	pass_bytes(listened, fd, rem);
        	}
        	// bytes_read = read(listened, buffer, rem);
        	// write_all(fd,buffer, bytes_read);
        	}
        	else {
        		write_all(fd, c->mes,x);
        		}
        
        close(fd);
        return 0;

}

int request(Command *c, int listened){
	regex_t re;
  	regmatch_t matches[6];
  	// char buf1;
  	int rc;
	int bytes_read = 0;
	// int bytes = 0;
	bytes_read = read_until(listened, c->buf, 4096,"\r\n\r\n");
	
	// int checker = 0;
	// fprintf(stderr, "\n%s\n", c->buf);
        c->bufsize = bytes_read;
        
   
        
  		    if (c->bufsize > 0) {
    		    c->buf[c->bufsize] = 0;
 
    		   // First step -- compile the regex. If this fails, there's
    		   // probably an issue with your PARSE_REGEX string
    		   rc = regcomp(&re, PARSE, REG_EXTENDED);
    		   assert (!rc);
    		   
    		   
    		   
    		   rc = regexec(&re, (char *)c->buf, 6, matches, 0);

    		   if (rc == 0) {
      			  c->command = c->buf;
      			  c->location = c->buf + matches[2].rm_so;
      			  c->ver = c->buf + matches[3].rm_so;
      			  c->head = c->buf + matches[4].rm_so;
      			  c->mes = c->buf + matches[5].rm_so;


      		// Uncomment me to fixup issues in the above!
      		  c->command[matches[1].rm_eo] = '\0';
      		  c->location[matches[2].rm_eo - matches[2].rm_so] = '\0';
      		  c->ver[matches[3].rm_eo - matches[3].rm_so] = '\0';
      		  c->head[matches[4].rm_eo - matches[4].rm_so] = '\0';
      		  c->mes[matches[5].rm_eo - matches[5].rm_so] = '\0';
      		  // fprintf(stderr, "\n%s\n", c->mes);
      		  
                  }
                  
                  else {
                 
		//fprintf(stderr, "%s\n", c->buf + matches[2].rm_so);
		//fprintf(stderr, "%s\n", c->buf + matches[3].rm_so);
		// fprintf(stderr, "%s\n", c->buf);
                  	
                        return 1;
                   }
                   }     
                    return  0;


}


int response(int regf, Command *c, int listened) {
	int fd;
	char* version = "HTTP/1.1 ";
	long file_size = 0;
	struct stat file_stats;
	 int bytes = 0;
	 /* bytes = write(listened, c->command, strlen(c->command));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command1\n");
                        
                    }
                    
        bytes = write(listened, c->location, strlen(c->location));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command2\n");
                        
                    }
        bytes = write(listened, c->ver, strlen(c->ver));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command3\n");
                        
                    }
        bytes = write(listened, c->head, strlen(c->head));
                    if (bytes < 0) {
                        fprintf(stderr, "Invalid Command4\n");
                        
                    } 
                    */
                    
	if (regf == 1) {
		ret_string(version, retCode(bad_request), listened);
                bytes = write(listened, "12\r\n\r\nBad Request\n", strlen("12\r\n\r\nBad Request\n"));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command11\n");          
                    }
      	            return 0;
         }
         
         if (strcmp(c->ver, "HTTP/1.1") != 0) {
         	ret_string(version, retCode(505), listened);
                bytes = write(listened, "22\r\n\r\nVersion Not Supported\n", strlen("22\r\n\r\nVersion Not Supported\n"));
                    if (bytes <= 0) {
                        fprintf(stderr, "Invalid Command10\n");          
                    }
      	            return 0;
         }
         
         
         
         
         
       if (strcmp(c->command, "GET") == 0) {
       		
       		fd = open(c->location, O_RDWR);
       		if (fd == -1) {
       		//	fd = open("temp.txt", O_RDONLY);
       		struct stat sb;
			if( stat(c->location, &sb) != -1) 
			{
    				if( S_ISREG(sb.st_mode) == 0 )
    				{
        			 	fprintf(stderr, "Invalid Command4\n");
                			ret_string(version, retCode(403), listened);
                			bytes = write(listened, "10\r\n\r\nForbidden\n", strlen("10\r\n\r\nForbidden\n"));
                    			if (bytes <= 0) {
                        			fprintf(stderr, "Invalid Command10\n");          
                    				}
                    				close(fd);
                				return 1;
    						}
    			}
       			
       			fprintf(stderr, "\n%s\n", c->location);
            		fprintf(stderr, "Invalid Command not found\n");
            		ret_string(version, retCode(not_found), listened);
       			bytes = write(listened, "10\r\n\r\nNot Found\n", strlen("10\r\n\r\nNot Found\n"));
            		close(fd);
            		return 1;
        		}
        	// fprintf(stderr, "\n%s\n", c->location);
        	// fprintf(stderr, "here5");
       		if (stat(c->location, &file_stats) < 0) {
       			close(fd);
            		return -1;
          		}
		// fprintf(stderr, "here6");
    		file_size =  file_stats.st_size;
    		ret_string(version, retCode(ok), listened);
    		// fprintf(stderr, "here7");
    		char fsize[1000];
    		sprintf(fsize, "%ld", file_size);
    		bytes = write(listened, fsize, strlen(fsize));
    		// fprintf(stderr, "here8");
    		bytes = write(listened, "\r\n\r\n", strlen("\r\n\r\n"));
    		// fprintf(stderr, "here9");
         	get(c->location, listened, fd);	
         	// fprintf(stderr, "%s", c->command);
         	close(fd);
         	return 0;
         }
         
       if (strcmp(c->command, "PUT") == 0) {
       		put(c, listened, version);
       		return 0;
       		}
       		
       		
       	ret_string(version, retCode(501), listened);
                bytes = write(listened, "16\r\n\r\nNot Implemented\n", strlen("16\r\n\r\nNot Implemented\n"));
                    	if (bytes <= 0) {
                        	fprintf(stderr, "Invalid Command10\n");          
                    		}
       		
	return 0;
}

int main(int argc, char **argv) {
	// char buf[4097];	
	char* portc;
	Command c;
// fprintf(stderr, "here");
	
	
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
    //    fprintf(stderr, "here1");
        int listened = 0;
        while(1) {
    //    fprintf(stderr, "here1.5");
        listened = listener_accept(&sok);
     //   fprintf(stderr, "here2");
        int suc = request(&c, listened);
        // fprintf(stderr, "here3");
        response(suc, &c, listened);
        close(listened);
        }
        close(sockfd);
        
  
  
    return 0;
}
