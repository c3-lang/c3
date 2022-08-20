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

debug:
	${MAKE} -C libc3 debug
	${MAKE} -C test debug

test: all
	${MAKE} -C test test

test_asan: asan
	${MAKE} -C test test_asan

test_cov: cov
	${MAKE} -C test test_cov

test_debug: debug
	${MAKE} -C test test_debug

install: all
	${MAKE} -C libc3 install
	${MAKE} -C c3c install

clean:
	${MAKE} -C libc3 clean
	${MAKE} -C c3c clean
	${MAKE} -C test clean

.PHONY: all clean install test
