#include <stdint.h>
#include <stdio.h>
#include "../params_patronus.h"
#include "../randombytes.h"
#include "../sample_polytope.h"
#include "cpucycles.h"
#include "speed_print.h"
#include "rand_print.h"
#include "../symmetric.h"

#define NTESTS 100000

uint64_t t[NTESTS];
uint64_t r[NTESTS];

void print_params(void);

int main(void) {

  unsigned int i;
  uint8_t seed[CRHBYTES];
  uint16_t nonce = 0;
  polyvecl a;
  randombytes(seed, sizeof(seed));
  for(i = 0; i < NTESTS; ++i) {
    t[i] = cpucycles();
    r[i] = polyvecl_uniform_polytope(&a, seed, nonce++);
  }
  print_params();
  print_results("polyvecl_uniform_polytope speed:", t, NTESTS);
  print_randomness("polyvecl_uniform_polytope randomness:", r, NTESTS);
  return 0;
}

void print_params(void) {
	printf("-----------------------------\n");
	printf("Patronus %d (params_patronus.h)", PATRONUS_PARAMS);
#ifdef PATRONUS_USE_AES
	printf("\nsampling with AESCTR256:\n");
#else
	printf("\nsampling with SHAKE256:\n");
#endif
	printf("- N: %d \n", N);
	printf("- L: %d \n", L);
	printf("- R: %d (r+beta)\n", R);
	printf("- BR: %d (sqrt(n)*R)\n", BR);
  printf("- RANDBUF BYTELENGTH: %d \n", POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES);
	printf("-----------------------------\n\n");
	return;
}

