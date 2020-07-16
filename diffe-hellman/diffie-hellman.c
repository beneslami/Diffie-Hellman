#include "diffie-hellman.h"

struct info_ {
  int prime;
  int generator;
};

struct key_{
  int private_key;
  int public_key;
};

static int
get_info_prime(info_t *info){
  return info->prime;
}

static void
set_info_prime(info_t *info, int prime){
  info->prime = prime;
}

static int
get_info_generator(info_t *info){
  return info->generator;
}

static void
set_info_generator(info_t *info, int generator) {
  info->generator = generator;
}

static int
get_key_private_key(key_t *key){
  return key->private_key;
}

static void
set_key_private_key(key_t *key, int private_key){
  key->private_key = private_key;
}

static int 
get_key_public_key(key_t *key){
  return key->public_key;
}

static void
set_key_public_key(key_t *key, int public_key){
  key->public_key = public_key;
}
