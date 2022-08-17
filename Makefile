all:
	${MAKE} -C libc3 all
	${MAKE} -C c3c all

test: all
	${MAKE} -C test test

install: all
	${MAKE} -C libc3 install
	${MAKE} -C c3c install

clean:
	${MAKE} -C libc3 clean
	${MAKE} -C c3c clean
	${MAKE} -C test clean

.PHONY: all clean install test
