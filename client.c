#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
  int data_socket;
  struct sockaddr_un *addr;

  data_socket = init_socket_client(addr);

  /*
  send and receive
  */
  
  close(data_socket);
  exit(EXIT_SUCCESS);
}
