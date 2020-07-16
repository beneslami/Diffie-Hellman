#ifndef DIFFIEHELLMAN_H
#define DIFFIEHELLMAN_H

typedef struct info_ info_t; /* opaque data structure */
typedef struct key_ key_t;   /* opaque data structure */

unsigned int
generate_prime(int, int);

unsigned long
generate_private_key(unsigned long);

unsigned long
generate_public_key(unsigned long);

unsigned long
generate_shared_key(unsigned long);

int
is_prime(unsigned int);


#endif
