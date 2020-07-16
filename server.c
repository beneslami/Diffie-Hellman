#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

  int ret;
  int data = 0;
  fd_set readfds;
  int data_socket;
  int connection_socket;
  char buffer[BUFFER_SIZE];
  struct sockaddr_un *name = NULL;
  server_struct_t *server_attribute = NULL;

  connection_socket = init_socket_server(name, server_attribute);
  if(connection_socket == -1){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  for(;;){
    refresh(server_attribute, &readfds);
    printf("waiting of accept() system call\n");
    select(get_max(server_attribute) + 1, &readfds, NULL, NULL, NULL);

    if(FD_ISSET(connection_socket, &readfds)){                        /* connection initiaztion part */
      printf("New connection recieved\n");
      data_socket = accept(connection_socket, NULL, NULL);
      if(data_socket == -1){
        perror("accpet");
        exit(EXIT_FAILURE);
      }
      printf("connection accepted\n");
      add_to_monitor(server_attribute, data_socket);
    }
    else if(FD_ISSET(0, &readfds)){                                   /* input from console */
      char op[BUFFER_SIZE];
      ret = read(0, op, BUFFER_SIZE -1);
      op[strcspn(op, "\r\n")] = 0; // flush new line
      if(ret < 0){
          printf("Insert valid operation\n");
          break;
      }
      op[ret] = 0;
      printf("input from console:\n%s\n", op);
    }
    else {                                                             /* data strives on some other client's FDs. Find the client which has sent us the data request */
      for(int i=2; i< MAX_CLIENT_SUPPORTED; i++){
        if(FD_ISSET(get_monitored_fd_set(server_attribute, i), &readfds)){
          int comm_socket_fd = get_monitored_fd_set(server_attribute, i);
          memset(buffer, 0 , BUFFER_SIZE);
          ret = read(comm_socket_fd, buffer, sizeof(int)); //returns the number of bytes recieved
          if(ret == -1){
            perror("read");
            exit(EXIT_FAILURE);
          }
          // decode from socket
        }
      }
    }

  }
  close(connection_socket);
  remove_from_monitor(server_attribute, connection_socket);
  printf("connection closed...\n");
  unlink(SOCKET_NAME);
  exit(EXIT_SUCCESS);
}
