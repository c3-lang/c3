## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

all: libc3.a libc3.la

libc3.a: libc3.la
	cp .libs/libc3.a ./

test:
	${MAKE} -C test

CLEANFILES = libc3.la .libs *.lo *.la

clean:
	rm -rf ${CLEANFILES}

.PHONY: all clean test

include config.mk
