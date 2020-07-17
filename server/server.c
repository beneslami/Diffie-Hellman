#include "socket.h"
#include "../diffie-hellman/diffie-hellman.h"

int main(int argc, char **argv){
  int socketfd, comm_socket_fd, data_socket, ret;
  unsigned int client_secret, result;
  fd_set readfds;
  char buffer[BUFFER_SIZE];
  server_struct_t *server_attribute = calloc(1, sizeof(server_struct_t*));
  key__t *server_key;

  if (argc != 2) {
        perror("ERROR, port is not initialized");
        exit(1);
  }
  in_port_t port = atoi(argv[1]);
  socketfd = init_socket_server(server_attribute);
  if(socketfd == -1){
        perror("Server-socket() error!");
        exit(0);
  }
  printf("Server-socket created\n");

  if(bind_create_socket(socketfd, port) < 0){
        perror("Server-socket-bind() failed");
        exit(0);
  }
  printf("bind done\n");

  if(listen(socketfd, 10) == -1){
    perror("Server-socket-listen() failed");
    exit(1);
  }
  add_to_monitor(server_attribute, socketfd);

  for(;;){
    server_key = calloc(1, sizeof(key_t*));
    refresh(server_attribute, &readfds);
    printf("Waiting for incoming connection\n");
    select(get_max(server_attribute) + 1, &readfds, NULL, NULL, NULL);

    if(FD_ISSET(socketfd, &readfds)){                                           /* connection initiaztion part */
      printf("New connection recieved\n");
      data_socket = accept(socketfd, NULL, NULL);
      if(data_socket == -1){
        perror("accpet");
        exit(EXIT_FAILURE);
      }
      printf("connection accepted\n");
      add_to_monitor(server_attribute, data_socket);
    }
    else if(FD_ISSET(0, &readfds)){                                             /* input from console */
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
    else{                                                   /* data strives on some other client's FDs. Find the client which has sent us the data request */
      for(int i=2; i< MAX_CLIENT_SUPPORTED; i++){
        if(FD_ISSET(get_monitored_fd_set(server_attribute, i), &readfds)){
          comm_socket_fd = get_monitored_fd_set(server_attribute, i);
          memset(buffer, 0, BUFFER_SIZE);
          ret = recv(comm_socket_fd, buffer, sizeof(int), 0);
          if(ret == -1){
            perror("read");
            exit(0);
          }
          memcpy(&client_secret, buffer, sizeof(int));
          srand(time(NULL));
          unsigned long min_prime = generate_prime(500, 1000);
          unsigned long max_prime = generate_prime(5000, 10000);
          generate_private_key(server_key, min_prime);
          generate_public_key(server_key, get_key_private_key(server_key), min_prime, max_prime);
          result = get_key_public_key(server_key);

          ret = send(data_socket, &result, sizeof(int), 0);
          if(ret == -1){
            perror("read");
            exit(0);
          }
          int shared_key = generate_shared_key(get_key_private_key(server_key), client_secret, max_prime);
          printf("shared key = %u\n", shared_key);
          close(comm_socket_fd);
          remove_from_monitor(server_attribute, comm_socket_fd);
          break;
        }
        break;
      }
    }
    free(server_key);
  }
  close(socketfd);
  remove_from_monitor(server_attribute, socketfd);
  unlink(SOCKET_NAME);
  free(server_attribute);
  return 0;
}
