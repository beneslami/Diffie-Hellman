#ifndef DIFFIEHELLMAN_H
#define DIFFIEHELLMAN_H

typedef struct key_ key__t;   /* opaque data structure */
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

unsigned int
generate_prime(int, int);

void
generate_private_key(key__t*, unsigned long);

void
generate_public_key(key__t*, unsigned long, unsigned long, unsigned long);

unsigned long
generate_shared_key(unsigned long, unsigned long, unsigned long);

int
is_prime(unsigned int);

int
generate_random(int, int);

int
get_key_private_key(key__t*);

int
get_key_public_key(key__t*);

#endif
