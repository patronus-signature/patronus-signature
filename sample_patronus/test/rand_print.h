#ifndef PRINT_RAND_H
#define PRINT_RAND_H

#include <stddef.h>
#include <stdint.h>

void print_randomness(const char *s, uint64_t *r, size_t tlen);

#endif
