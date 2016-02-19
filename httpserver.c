/* 
This code primarily comes from 
http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
and
http://www.binarii.com/files/papers/c_sockets.txt
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "hash_table.h"
#define BUFF_SIZE 10000

int quit = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void server_error(char *error_msg, char** reply_ptr, int *bad_request_ptr) {
  perror(error_msg);
  *reply_ptr = "HTTP/1.1 500 Internal Server Error";
  (*bad_request_ptr)++;
}


void *start_server(void *argv_void)
{

  char** argv = (char**)argv_void;
  // structs to represent the server and client
  struct sockaddr_in server_addr,client_addr;    

  int sock; // socket descriptor

  // 1. socket: creates a socket descriptor that you later use to make other system calls
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    exit(1);
  }
  int temp;
  if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
    perror("Setsockopt");
    exit(1);
  }

  // configure the server
  int PORT_NUMBER = atoi(argv[1]);
  server_addr.sin_port = htons(PORT_NUMBER); // specify port number
  server_addr.sin_family = AF_INET;         
  server_addr.sin_addr.s_addr = INADDR_ANY; 
  bzero(&(server_addr.sin_zero),8); 
  
  // 2. bind: use the socket and associate it with the port number
  if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
    perror("Unable to bind");
    exit(1);
  }

  // 3. listen: indicates that we want to listen to the port to which we bound; second arg is number of allowed connections
  if (listen(sock, 1) == -1) {
    perror("Listen");
    exit(1);
  }



  // once you get here, the server is set up and about to start listening
  printf("\nServer configured to listen on port %d\n", PORT_NUMBER);
  fflush(stdout);

  int fd = -1;
  int successful_requests = 0;
  int unsuccessful_requests = 0;
  int bytes_received = 0;
  char fnames [BUFF_SIZE]; 
  hash_table *page_table = create_hash_table(10);

  while(!quit) {

    // 4. accept: wait here until we get a connection on that port
    int sin_size = sizeof(struct sockaddr_in);
        fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
    if (fd != -1) {
      printf("Server got a connection from (%s, %d)\n",
          inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
          
      // buffer to read data into
      char request[BUFF_SIZE];
      char *reply;

      // 5. recv: read incoming message into buffer
      int bytes_received = recv(fd,request,1024,0);
      // null-terminate the string
      request[bytes_received] = '\0';
      char *fname_ptr = strchr(request, '/') + 1;
      if (!fname_ptr) {
        char error_buff [BUFF_SIZE];
        sprintf(error_buff, "Unrecognized format: %s", request);
        server_error(error_buff, &reply, &unsuccessful_requests);
      }
      char *fname = strsep(&fname_ptr, " ");
      if (strcmp(fname, "favicon.ico") != 0) {

        // access file specified by request
        char *ok_header = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"; 
        if (strcmp(fname, "stats") == 0) {
          char header [BUFF_SIZE];
          char html [BUFF_SIZE];
          strcpy(header, ok_header); 
          sprintf(html, "<html>\n\
              Number of page requests handled successfully: %d\n<p>\n\
              Number of page requests that could not be handled \
              (because the page didn't exist or an error occurred): %d\n<p>\n\
              Total number of bytes sent back to the client \
              for all successful page requests: %d\n<p>\n\
              The distinct names of all files retrieved \
              for all successful page requests: \n<ul>\n%s</li>\n</ul>",
              successful_requests, unsuccessful_requests, bytes_received,
              fnames);
          reply = strcat(header, html);
        } else {

          char header [BUFF_SIZE];
          char html [BUFF_SIZE];
          char pathbuffer [BUFF_SIZE]; // buffer for full path to file

          strcpy(pathbuffer, argv[2]); // add root to filepath
          if (!pathbuffer[0]) { 
            server_error("strcpy failed", &reply, &unsuccessful_requests);
          }
          char* filepath = strcat(pathbuffer, fname); // append filename to path
          if (!filepath) {
            server_error("strcat failed", &reply, &unsuccessful_requests);
          }
          FILE *file = fopen(filepath, "r");
          if (!file) {
            fprintf(stderr, "Could not find %s in %s\n", fname, argv[2]);
            reply = "HTTP/1.1 404 Not Found";
          } else {
            strcpy(header, ok_header);
            size_t bytes_read = fread(html, 1, sizeof(html), file);
            reply = strcat(header, html);
            if (!reply) { 
              server_error("strcat failed", &reply, &unsuccessful_requests);
            }
          }
        }
    
        // 6. send: send the message over the socket
        // note that the second argument is a char*,
        // and the third is the number of chars
        send(fd, reply, strlen(reply), 0);
        printf("Server sent message: %s\n", reply);

        successful_requests++;
        bytes_received += strlen(reply); 
        int err = add_to_table(page_table, fname);
        if (err) {
          server_error("error adding to table", &reply, &unsuccessful_requests);
        }
        strcat(fnames, "</li>\n<li>"); 
        strcat(fnames, fname); 
      } 
    }
  } // end while
  
  // 7. close: close the connection
  close(fd);
  printf("Server closed connection\n");

  // 8. close: close the socket
  close(sock);
  printf("Server shutting down\n");

  return 0;
} 

void *prompt_for_quit(void *doesnt_matter) {
  char input_buffer [BUFF_SIZE];
  while (!quit) {
    scanf("%s", input_buffer);
    if (strcmp(input_buffer, "q") == 0) {
      quit = 1;
    }
  }
  return NULL;
}

int error() { 
  return -1;
}

int main(int argc, char *argv[])
{
  // check the number of arguments
  if (argc < 2) {
      printf("\nUsage: %s [port_number]\n", argv[0]);
      exit(-1);
  }

  int port_number = atoi(argv[1]);
  if (port_number <= 1024) {
    printf("\nPlease specify a port number greater than 1024\n");
    exit(-1);
  }

  puts("Enter 'q' at any time to quit.");
  pthread_t t1, t2;
  /*start_server(port_number, argv[2]);*/
  if ( pthread_create( &t1, NULL, start_server, argv) != 0  ) {
    return error(); 
  }
  if ( pthread_create( &t1, NULL, prompt_for_quit, NULL  ) != 0  ) {
    return error(); 
  } 
  if ( pthread_join( t1, NULL  ) != 0  ) { 
    return error(); 
  } 
  if ( pthread_join( t2, NULL  ) != 0  ) { 
    return error();
  }
}

