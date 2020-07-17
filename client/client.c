#include "socket.h"
#include "../diffie-hellman/diffie-hellman.h"

int main(int argc, char **argv){
  int data_socket, ret;
  char *server_ip = argv[1];
  in_port_t server_port = atoi(argv[2]);
  key__t *client_key = calloc(1, sizeof(key_t*));
  unsigned int secret;

  data_socket = init_socket_client();
  ret = connect_to_server(data_socket, server_ip, server_port);
  if(ret < 0){
    printf("connection failed\n");
    exit(0);
  }
  printf("connection established\n");

  /* key generation process */
  srand(time(NULL));
  unsigned long min_prime = generate_prime(500, 1000);
  unsigned long max_prime = generate_prime(5000, 10000);
  generate_private_key(client_key, min_prime);
  generate_public_key(client_key, get_key_private_key(client_key), min_prime, max_prime);
  secret = get_key_public_key(client_key);

  if(send(data_socket, &secret, sizeof(int), 0) == -1){
    printf("sending failed\n");
    exit(0);
  }
  char buffer[50];
  int num = recv(data_socket, buffer, sizeof(buffer), 0);
  if(num <= 0){
    printf("receive failed\n");
    exit(0);
  }
  buffer[num] = '\0';
  int server_secret;
  memcpy(&server_secret, buffer, sizeof(int));

  int shared_key = generate_shared_key(get_key_private_key(client_key), server_secret, max_prime);
  printf("shared key = %u\n", shared_key);

  close(data_socket);
  free(client_key);
  exit(EXIT_SUCCESS);
}
