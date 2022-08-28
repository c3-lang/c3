## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

CLEANFILES = *.a c3_test c3_test.asan c3_test.cov c3_test.debug *.css *.gcno *.html *.o .libs *.lo
CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda

all:   c3_test
asan:  c3_test.asan
cov:   c3_test.cov
debug: c3_test.debug

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details test.html

test: c3_test
	time ./c3_test

test_asan: c3_test.asan
	time ./c3_test.asan

test_cov: c3_test.cov
	time ./c3_test.cov

test_debug: c3_test.debug
	time ./c3_test.debug

.PHONY: all asan cov debug clean clean_cov test test_asan test_cov test_debug

include config.mk
