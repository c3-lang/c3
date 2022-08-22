## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

all:
	${MAKE} -C libc3 all
	${MAKE} -C c3c all
	${MAKE} -C test all

asan:
	${MAKE} -C libc3 asan
	${MAKE} -C test asan

cov:
	${MAKE} -C libc3 cov
	${MAKE} -C test cov

clean:
	${MAKE} -C libc3 clean
	${MAKE} -C c3c clean
	${MAKE} -C test clean

clean_cov:
	${MAKE} -C libc3 clean_cov
	${MAKE} -C test clean_cov

debug:
	${MAKE} -C libc3 debug
	${MAKE} -C test debug

gcovr:
	${MAKE} -C libc3 gcovr
	${MAKE} -C test gcovr
	if [ -d "$$HOME/Downloads/c3_gcovr" ]; then bin/gcovr-to-downloads; fi

install: all
	${MAKE} -C libc3 install
	${MAKE} -C c3c install

test: all
	${MAKE} -C test test

test_asan: asan
	${MAKE} -C test test_asan

test_cov: cov
	${MAKE} -C test test_cov

test_debug: debug
	${MAKE} -C test test_debug

.PHONY: all asan cov clean clean_cov debug gcovr install test test_asan test_cov test_debug
