#ifndef PARAMS_PATRONUS_H
#define PARAMS_PATRONUS_H

#define SEEDBYTES 32
#define CRHBYTES 64
#define N 256

#if PATRONUS_PARAMS == 2
#define R 183662
#define BR 5877184
#define RP 1
#define L 4

#elif PATRONUS_PARAMS == 3
#define R 289912
#define BR 10372207
#define RP 1
#define L 5

#elif PATRONUS_PARAMS == 5
#define R 569261
#define BR 24087968
#define RP 1
#define L 7

#endif
#endif
