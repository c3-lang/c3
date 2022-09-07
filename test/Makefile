## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

CLEANFILES = *.a c3_test c3_test.asan c3_test.cov c3_test.debug *.css *.gcno *.html *.o .libs *.lo test/*.err test/*.out
CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda

build: c3_test

all: build cov debug
	if ${HAVE_ASAN}; then ${MAKE} asan; fi

asan: c3_test.asan

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov:   c3_test.cov

debug: c3_test.debug

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details test.html

test: c3_test
	time ./c3_test
	IC3=${PWD}/../ic3/ic3 time ./ic3_test

test_asan: c3_test.asan
	time ./c3_test.asan
	IC3=${PWD}/../ic3/ic3.asan time ./ic3_test

test_cov: c3_test.cov
	time ./c3_test.cov
	IC3=${PWD}/../ic3/ic3.cov time ./ic3_test

test_debug: c3_test.debug
	time ./c3_test.debug
	IC3=${PWD}/../ic3/ic3.debug time ./ic3_test

test_valgrind: c3_test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./c3_test

.PHONY: all asan cov debug clean clean_cov test test_asan test_cov test_debug test_valgrind

include config.mk
