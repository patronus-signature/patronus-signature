#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include "symmetric.h"
#include "params_patronus.h"
#include "sample_polytope.h"

/*************************************************
* Name:        polyvecl_uniform_polytope
*
* Description: Sample uniformly a random vector of polynomials of length L in
*              the intersection of L1 and Linf balls by performing rejection
*              sampling on the output stream from SHAKE256(seed|nonce) or
*              AES256CTR(seed,nonce).
*
* Arguments:   - polyvecl *v: pointer to output vector
*              - const uint8_t seed[]: byte array with seed of length CRHBYTES
*              - uint16_t nonce: 16-bit nonce
**************************************************/

int polyvecl_uniform_polytope(polyvecl *v,
	const uint8_t seed[CRHBYTES],
	uint16_t nonce)
{
	int64_t coeffs[N*L+2];
	unsigned int i, rej;
	int j, ind = 0, fill = 1;
	uint8_t buf[POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES];
	int64_t rand, val, sign;
	poly *a;
	stream256_state state;
	stream256_init(&state, seed, nonce);
	stream256_squeezeblocks(buf, POLYVECL_UNIFORM_POLYTOPE_NBLOCKS, &state);

	do {
		rej = 0;
		coeffs[0]= 0;
		coeffs[1] = BR + L*N + 1;
    // at the end of this for loop, coeffs[0], ..., coeffs[L*N+1] will be such that:
    // coeffs[0] = 0
    // coeffs[L*N+1] = BR + L*N + 1
    // other L*N values are sorted and represent an uniform subset of [[1, BR+L*N]]
		for(i = 2; i < L*N+2; ++i) {
			do {
#if PATRONUS_PARAMS == 5
        // refill the randomness buffer if necessary
				if (ind+4 > POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES) {
					stream256_squeezeblocks(buf, POLYVECL_UNIFORM_POLYTOPE_NBLOCKS, &state);
					ind = 0;
					fill++;
				}
				// log(2, BR+L*N-1) is 24.02, so we need a 25-bit mask here
				rand = ((int64_t)buf[ind]) << 24 | ((int64_t)buf[ind+1]) << 16 | ((int64_t)buf[ind+2] << 8) | (int64_t)buf[ind+3];
				rand = rand & ((int64_t)(0x1ffffff));
				ind += 4;
#elif PATRONUS_PARAMS == 3
  			// refill the randomness buffer if necessary
				if (ind+3 > POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES) {
					stream256_squeezeblocks(buf, POLYVECL_UNIFORM_POLYTOPE_NBLOCKS, &state);
					ind = 0;
					fill++;
				}
				// log(2, BR+L*N-1) is 23.01, so we need a 24-bit mask here
				rand = ((int64_t)buf[ind]) << 16 | ((int64_t)buf[ind+1] << 8) | (int64_t)buf[ind+2];
				ind += 3;
#elif PATRONUS_PARAMS == 2
  			// refill the randomness buffer if necessary
				if (ind+3 > POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES) {
					stream256_squeezeblocks(buf, POLYVECL_UNIFORM_POLYTOPE_NBLOCKS, &state);
					ind = 0;
					fill++;
				}
				// log(2, BR+L*N-1) is 22.243, so we need a 23-bit mask here
				rand = ((int64_t)buf[ind]) << 16 | ((int64_t)buf[ind+1] << 8) | (int64_t)buf[ind+2];
				rand = rand & ((int64_t)(0x7fffff));
				ind += 3;
#endif
			} while(rand > BR+L*N-1);
			val = rand + 1;
			j = (int)i - 1;
      // the probability that val is equal to one of the already sampled values is low
      // thus we suppose that val isn't equal to one of them and right-shift coeffs greater than it
      // if we see later that we were wrong, we shift it back
			while (coeffs[j] > val) {
				coeffs[j + 1] = coeffs[j];
				j--;
			}
			coeffs[j + 1] = val;
			if (coeffs[j] == val) {
				for (int u = j+1; u < (int)i; u++) {
					coeffs[u] = coeffs[u+1];
				}
				i--;
			}
		}
    // compute differences minus one and check the Linf rejection condition
		for (i = L*N+1; i > 1; --i) {
			coeffs[i] = coeffs[i] - coeffs[i-1] - 1;
			if (coeffs[i] > R) {rej = 1; break;}
		}
		if (rej) {continue;}
    // add signs and check the plus/minus zero rejection condition
		for (i = 1, j = 0; i < L*N+2; ++i, ++j) {
			sign = ((int64_t)buf[ind++] & (1 << j)) >> j;
			if (coeffs[i] + sign == 0) {rej = 1; ind++; break;}
			coeffs[i] = ((sign << 1) - 1) * coeffs[i];
			if (j == 7) {
				ind++;
				j = 0;
				// refill the randomness buffer if necessary
				if (ind > POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES) {
					stream256_squeezeblocks(buf, POLYVECL_UNIFORM_POLYTOPE_NBLOCKS, &state);
					ind = 0;
					fill++;
				}
			}
		}
		if (rej) {continue;}
		for (i = 0; i < L; ++i) {
			a = &v->vec[i];
			for (j = 0; j < N; ++j) {
				a->coeffs[j] = coeffs[i*N+j+1];
			}
		}
	} while (rej);
	return fill*POLYVECL_UNIFORM_POLYTOPE_NBLOCKS*STREAM256_BLOCKBYTES;
}
