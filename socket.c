#include "socket.h"
#include <stdio.h>

struct server_struct_{
  int monitored_fd_set[MAX_CLIENT_SUPPORTED];
int client_result[MAX_CLIENT_SUPPORTED] = {0};
int comm_socket_fd = -1;
};

/* getter and setter APIs for opaque data structure */
int
get_monitored_fd_set(server_struct_t *server_attribute, int value){
  return server_attribute->monitored_fd_set[value];
}

static void set_monitored_fd_set(server_struct_t *server_attribute, int index, int skt_fd){
  server_attribute->monitored_fd_set[index] = skt_fd;
}

static void
initialize_monitor_fd_set(server_struct_t *server_attribute){ /* remove all FDs, if any from the array*/
  set_monitored_fd_set(server_attribute, -1);
}

static void
add_monitor_fd_set(server_struct_t *server_attribute, int index, int skt_fd){ /* add new FD to the array*/
  for(int i=0; i<MAX_CLIENT_SUPPORTED; i++){
    if(get_monitored_fd_set(server_attribute, i) != -1) continue;
    set_monitored_fd_set(server_attribute, i, skt_fd);
    break;
  }
}

static void
refresh_fd_set(server_struct_t *server_attribute, fd_set *fd_set_ptr){/* Clone all the FDs in the array into fd_set data structure*/
  FD_ZERO(fd_set_ptr);
  for(int i=0; i<MAX_CLIENT_SUPPORTED; i++){
    if(get_monitored_fd_set(server_attribute, i) != -1){
        FD_SET(get_monitored_fd_set(server_attribute, i), fd_set_ptr); /* copy monitored_fd_set[i] to fd_set_ptr */
    }
  }
}

static void
remove_monitor_fd_set(server_struct_t *server_attribute, int skt_fd){/* remove the FD from the array*/
  for(int i=0; i<MAX_CLIENT_SUPPORTED; i++){
    if(get_monitored_fd_set(server_attribute, i) != skt_fd) continue;
    set_monitored_fd_set(monitored_fd_set, i,-1);
    break;
  }
}

static
int get_max_fd(server_struct_t *server_attribute){/* get the numerical max value among all FDs which server is monitoring*/
  int max = -1;
  for(int i=0; i<MAX_CLIENT_SUPPORTED; i++){
      if(get_monitored_fd_set(server_attribute, i)> max){
        max = get_monitored_fd_set(server_attribute, i);
      }
  }
  return max;
}

int
get_max(server_struct_t *server_attribute){
  int max;
  max = get_max_fd(server_attribute);
  return max;
}

void
refresh(server_struct_t *server_attribute, fd_set *fd_set_ptr){
  refresh_fd_set(server_attribute, fd_set_ptr);
} /* wrapper for the private function */

void
add_to_monitor(server_struct_t *server_attribute, int fd_set){ /* wrapper for the private function */
  add_monitor_fd_set(server_attribute, 0, fd_set);
}

void
remove_from_monitor(server_struct_t *server_attribute, int fd_set){ /* wrapper for the private function */
  remove_monitor_fd_set(server_attribute, fd_set);
}

int
init_socket_server(struct sockaddr_un *name, server_struct_t *server_attribute){
  initialize_monitor_fd_set(server_attribute);
  add_monitor_fd_set(server_attribute, -1, 0);
  unlink(SOCKET_NAME);
  connection_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(connection_socket == -1){
    perror("socket");
    -1;
  }
  printf("master socket created\n");
  memset(name, 0, sizeof(struct sockaddr_un));
  name->sun_family = AF_INET;
  strncpy(name->sun_path, SOCKET_NAME, sizeof(name->sun_path) - 1);
  ret = bind(connection_socket, (const struct sockaddr *)name, sizeof(struct sockaddr_un));
  if(ret == -1){
    perror("bind");
    return -1;
  }
  printf("bind succedded\n");
  int ret = listen(connection_socket, 20); //20 is the number of clients simultaneously try to connect.
  if(ret == -1){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  add_monitor_fd_set(server_attribute, 0, connection_socket); /* add master socket FD to the array, connection_socket = monitored_fd_set[1]*/
  return connection_socket;
}

//******************************************************************************
int
init_socket_client(struct sockaddr_un *addr){
  struct sockaddr_un addr;
  int data_socket;
  data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(data_socket == -1){
    perror("socket");
    return -1;
  }
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_INET;
  strncpy(addr.sun_path, SOCEKT_NAME, sizeof(addr.sun_path));
  ret = connect(data_socket, (const struct sockaddr*) &addr, sizeof(struct sockaddr_un));
  if(ret == -1){
    perror("connect");
    return -1;
  }
  return data_socket;
}
