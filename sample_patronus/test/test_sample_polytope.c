#include <stdint.h>
#include <stdio.h>
#include "../params_patronus.h"
#include "../randombytes.h"
#include "../sample_polytope.h"
#include "cpucycles.h"
#include "speed_print.h"
#include "../symmetric.h"

#define NTESTS 10000

uint64_t t[NTESTS];

void print_params(void);

int main(void) {

  unsigned int i;
  uint8_t seed[CRHBYTES];
  uint16_t nonce = 0;
  polyvecl a;
  randombytes(seed, sizeof(seed));
  for(i = 0; i < NTESTS; ++i) {
    t[i] = cpucycles();
    polyvecl_uniform_polytope(&a, seed, nonce++);
  }
  print_params();
  print_results("polyvecl_uniform_polytope:", t, NTESTS);

  return 0;
}

void print_params(void) {
	printf("-------------------------\n");
	printf("Set of parameters used: %d (set defined in ../params_patronus.h)", PATRONUS_PARAMS);
#ifdef PATRONUS_USE_AES
	printf(", with AESCTR256\n");
#else
	printf(", with SHAKE256\n");
#endif
	printf("Parameters used: %d, more precisely:\n", N);
	printf("- SEEDBYTES is: %d \n", SEEDBYTES);
	printf("- CRHBYTES is: %d \n", CRHBYTES);
	#if PATRONUS_PARAMS == 5
	#define BUF_LEN ((50 * L*N + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
	#elif PATRONUS_PARAMS == 3
	#define BUF_LEN ((45 * L*N + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
	#elif PATRONUS_PARAMS == 2
	#define BUF_LEN ((35 * L*N + STREAM256_BLOCKBYTES - 1)/STREAM256_BLOCKBYTES)
	#endif
	printf("- BUF BYTE LENGTH is: %d \n", BUF_LEN*STREAM256_BLOCKBYTES);
	printf("- N is: %d \n", N);
	printf("- R is: %d \n", R);
	printf("- BR is: %d \n", BR);
	printf("- RP is: %d \n", RP);
	printf("- L is: %d \n", L);
	printf("-------------------------\n\n");
	return;
}

