## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

all: libc3.la libc3.a

debug: libc3.debug.la libc3.debug.a

libc3.a: libc3.la
	cp .libs/libc3.a ./

libc3.asan.a: libc3.asan.la
	cp .libs/libc3.asan.a ./

libc3.cov.a: libc3.cov.la
	cp .libs/libc3.cov.a ./

libc3.debug.a: libc3.debug.la
	cp .libs/libc3.debug.a ./

test:

CLEANFILES = libc3.la .libs *.lo *.la

clean:
	rm -rf ${CLEANFILES}

.PHONY: all clean debug test

include config.mk
