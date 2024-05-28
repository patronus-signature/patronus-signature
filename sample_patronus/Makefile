CC ?= /usr/bin/cc
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wvla -Wpointer-arith -O3 -fomit-frame-pointer -g
NISTFLAGS += -Wno-unused-result -O3 -fomit-frame-pointer

SAMPLE_SOURCES =  sample_polytope.c fips202.c symmetric-shake.c
SAMPLE_HEADERS = sample_polytope.h params_patronus.h  symmetric.h randombytes.h fips202.h
SAMPLE_SOURCES_AES =  sample_polytope.c fips202.c symmetric-aes.c aes256ctr.c
SAMPLE_HEADERS_AES = sample_polytope.h params_patronus.h  symmetric.h randombytes.h fips202.h aes256ctr.h

.PHONY: all  clean samplepoly

all: samplepoly

samplepoly: test/test_sample_polytope_params_2  test/test_sample_polytope_params_3  test/test_sample_polytope_params_5 \
	test/test_sample_polytope_params_2_aes   test/test_sample_polytope_params_3_aes test/test_sample_polytope_params_5_aes

test/test_sample_polytope_params_2: test/test_sample_polytope.c test/speed_print.c test/speed_print.h \
  test/rand_print.c test/rand_print.h test/cpucycles.c test/cpucycles.h randombytes.c $(SAMPLE_SOURCES) \
  $(SAMPLE_HEADERS) 
	$(CC) $(CFLAGS) -DPATRONUS_PARAMS=2 \
		-o $@ $< test/speed_print.c test/rand_print.c test/cpucycles.c randombytes.c $(SAMPLE_SOURCES) 

test/test_sample_polytope_params_3: test/test_sample_polytope.c test/speed_print.c test/speed_print.h \
  test/rand_print.c test/rand_print.h test/cpucycles.c test/cpucycles.h randombytes.c $(SAMPLE_SOURCES) \
  $(SAMPLE_HEADERS) 
	$(CC) $(CFLAGS) -DPATRONUS_PARAMS=3 \
		-o $@ $< test/speed_print.c test/rand_print.c test/cpucycles.c randombytes.c $(SAMPLE_SOURCES) 

test/test_sample_polytope_params_5: test/test_sample_polytope.c test/speed_print.c test/speed_print.h \
  test/rand_print.c test/rand_print.h test/cpucycles.c test/cpucycles.h randombytes.c $(SAMPLE_SOURCES) \
  $(SAMPLE_HEADERS) 
	$(CC) $(CFLAGS) -DPATRONUS_PARAMS=5 \
		-o $@ $< test/speed_print.c test/rand_print.c test/cpucycles.c randombytes.c $(SAMPLE_SOURCES) 

test/test_sample_polytope_params_2_aes: test/test_sample_polytope.c test/speed_print.c test/speed_print.h \
  test/rand_print.c test/rand_print.h test/cpucycles.c test/cpucycles.h randombytes.c $(SAMPLE_SOURCES_AES) \
  $(SAMPLE_HEADERS_AES) 
	$(CC) $(CFLAGS) -DPATRONUS_PARAMS=2 -DPATRONUS_USE_AES\
		-o $@ $< test/speed_print.c test/rand_print.c test/cpucycles.c randombytes.c $(SAMPLE_SOURCES_AES) 

test/test_sample_polytope_params_3_aes: test/test_sample_polytope.c test/speed_print.c test/speed_print.h \
  test/rand_print.c test/rand_print.h test/cpucycles.c test/cpucycles.h randombytes.c $(SAMPLE_SOURCES_AES) \
  $(SAMPLE_HEADERS_AES) 
	$(CC) $(CFLAGS) -DPATRONUS_PARAMS=3 -DPATRONUS_USE_AES\
		-o $@ $< test/speed_print.c test/rand_print.c test/cpucycles.c randombytes.c $(SAMPLE_SOURCES_AES) 


test/test_sample_polytope_params_5_aes: test/test_sample_polytope.c test/speed_print.c test/speed_print.h \
  test/rand_print.c test/rand_print.h test/cpucycles.c test/cpucycles.h randombytes.c $(SAMPLE_SOURCES_AES) \
  $(SAMPLE_HEADERS_AES) 
	$(CC) $(CFLAGS) -DPATRONUS_PARAMS=5 -DPATRONUS_USE_AES\
		-o $@ $< test/speed_print.c test/rand_print.c test/cpucycles.c randombytes.c $(SAMPLE_SOURCES_AES) 

clean:
	rm -f *~ test/*~ *.gcno *.gcda *.lcov
	rm -f test/test_sample_polytope_params_2
	rm -f test/test_sample_polytope_params_3
	rm -f test/test_sample_polytope_params_5
	rm -f test/test_sample_polytope_params_2_aes
	rm -f test/test_sample_polytope_params_3_aes
	rm -f test/test_sample_polytope_params_5_aes
