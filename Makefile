## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

build:
	${MAKE} -C libtommath build
	${MAKE} -C ucd2c build
	${MAKE} -C libc3 build
	${MAKE} -C ic3 build
	${MAKE} -C test build

all:
	${MAKE} -C libtommath all
	${MAKE} -C ucd2c all
	${MAKE} -C libc3 all
	${MAKE} -C ic3 all
	${MAKE} -C test all

asan:
	${MAKE} -C libtommath asan
	${MAKE} -C libc3 asan
	${MAKE} -C ic3 asan
	${MAKE} -C test asan

cov:
	${MAKE} -C libtommath cov
	${MAKE} -C libc3 cov
	${MAKE} -C ic3 cov
	${MAKE} -C test cov

clean:
	${MAKE} -C libtommath clean
	${MAKE} -C ucd2c clean
	${MAKE} -C libc3 clean
	${MAKE} -C ic3 clean
	${MAKE} -C test clean

clean_cov:
	${MAKE} -C libtommath clean_cov
	${MAKE} -C libc3 clean_cov
	${MAKE} -C ic3 clean_cov
	${MAKE} -C test clean_cov

debug:
	${MAKE} -C libtommath debug
	${MAKE} -C libc3 debug
	${MAKE} -C ic3 debug
	${MAKE} -C test debug

gcovr: test_cov
	${MAKE} -C libc3 gcovr
	${MAKE} -C ic3 gcovr
	${MAKE} -C test gcovr
	if [ -d "$$HOME/Downloads/c3_gcovr" ]; then bin/gcovr-to-downloads; fi

install: all
	${MAKE} -C libc3 install
	${MAKE} -C ic3 install

test: build
	${MAKE} -C test test

test_asan: asan
	${MAKE} -C test test_asan

test_cov: cov clean_cov
	${MAKE} -C test test_cov

test_debug: debug
	${MAKE} -C test test_debug

.PHONY: all asan cov clean clean_cov debug gcovr ic3 install libc3 libtommath test test_asan test_cov test_debug
