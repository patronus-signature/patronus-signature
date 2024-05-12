#ifndef SAMPLE_POLYTOPE_H
#define SAMPLE_POLYTOPE_H

#include <stdint.h>
#include "params_patronus.h"

typedef struct {
  int32_t coeffs[N];
} poly;

/* Vectors of polynomials of length L */
typedef struct {
  poly vec[L];
} polyvecl;

void polyvecl_uniform_polytope(polyvecl *v, const uint8_t seed[CRHBYTES], uint16_t nonce);

#endif
