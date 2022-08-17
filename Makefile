all:
	${MAKE} -C libc3 all
	${MAKE} -C c3c all

test: all
	${MAKE} -C test test

clean:
	${MAKE} -C libc3 clean
	${MAKE} -C test clean

.PHONY: all clean test
