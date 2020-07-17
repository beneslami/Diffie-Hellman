#include "diffie-hellman.h"


struct key_{
  unsigned int private_key;
  unsigned int public_key;
};

int
get_key_private_key(key__t *key){
  return key->private_key;
}

static void
set_key_private_key(key__t *key, int private_key){
  key->private_key = private_key;
}

int
get_key_public_key(key__t *key){
  return key->public_key;
}

static void
set_key_public_key(key__t *key, int public_key){
  key->public_key = public_key;
}

unsigned int
generate_prime(int min, int max){
  int flag = 1;
  unsigned int prime;
  while(flag){
    prime = generate_random(min, max);
    if(prime % 2 != 0){
      unsigned int i = 3;
      for(; i< prime; i +=2){
        if(prime % i != 0){
          flag = 0;
        }
      }
    }
  }
  return prime;


}

void
generate_private_key(key__t *key, unsigned long seed){
  set_key_private_key(key, (unsigned long)rand()%seed + 1);
}

void
generate_public_key(key__t *key, unsigned long private_key, unsigned long min_prime, unsigned long max_prime){
  //printf("%lu\n", (unsigned long)pow(min_prime, private_key) % max_prime);
  set_key_public_key(key, (unsigned long)pow(min_prime/100, private_key/100) % max_prime);
}

unsigned long
generate_shared_key(unsigned long private_key, unsigned long public_key, unsigned long max_prime){
  return (unsigned long)pow(public_key/100, private_key/100) % max_prime;
}

int
generate_random(int min, int max){
    int random_number;
    srand(time(NULL));
    while(1){
        random_number = rand()% max + 1;
        if(random_number > min)
          break;
    }
    return random_number;
}
