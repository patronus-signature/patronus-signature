#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "rand_print.h"

static int cmp_uint64(const void *a, const void *b) {
  if(*(uint64_t *)a < *(uint64_t *)b) return -1;
  if(*(uint64_t *)a > *(uint64_t *)b) return 1;
  return 0;
}

static uint64_t median(uint64_t *l, size_t llen) {
  qsort(l,llen,sizeof(uint64_t),cmp_uint64);

  if(llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}

static uint64_t average(uint64_t *t, size_t tlen) {
  size_t i;
  uint64_t acc=0;

  for(i=0;i<tlen;i++)
    acc += t[i];

  return acc/tlen;
}

static double variance(uint64_t *t, size_t tlen) {
  size_t i;
  double acc=.0, mean;

  mean = (double)average(t, tlen);

  for(i=0;i<tlen;i++)
    acc += ((double)t[i]-mean)*((double)t[i]-mean);

  return acc/((double)tlen-1.);
}

void print_randomness(const char *s, uint64_t *r, size_t tlen) {
  printf("%s\n", s);
  printf("median: %llu bytes/sample\n", (unsigned long long)median(r, tlen));
  printf("average: %llu bytes/sample\n", (unsigned long long)average(r, tlen));
  printf("variance: %17f\n", variance(r, tlen));
  printf("\n");
}
