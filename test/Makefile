## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

all:   c3_test
asan:  c3_test.asan
cov:   c3_test.cov
debug: c3_test.debug

CLEANFILES = c3_test c3_test.asan c3_test.cov c3_test.debug *.a *.o *.lo *.gcno
clean:
	rm -f ${CLEANFILES}

test: c3_test
	./c3_test

test_asan: c3_test.asan
	./c3_test.asan

test_cov: c3_test.cov
	./c3_test.cov

test_debug: c3_test.debug
	./c3_test.debug

.PHONY: all test

include config.mk
