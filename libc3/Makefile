## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

CLEANFILES = *.a *.gcno *.la libc3.la .libs *.lo *.o

CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda
CLEANFILES += ${CLEANFILES_COV}

DISTCLEANFILES = ${CLEANFILES} config.mk

build: libc3.la libc3.a

all: build cov debug
	if ${HAVE_ASAN}; then ${MAKE} asan; fi

asan: libc3.asan.la libc3.asan.a

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov: libc3.cov.la libc3.cov.a

debug: libc3.debug.la libc3.debug.a

distclean:
	rm -rf ${DISTCLEANFILES}

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details libc3.html

libc3.a: libc3.la
	cp .libs/libc3.a ./

libc3.asan.a: libc3.asan.la
	cp .libs/libc3.asan.a ./

libc3.cov.a: libc3.cov.la
	cp .libs/libc3.cov.a ./

libc3.debug.a: libc3.debug.la
	cp .libs/libc3.debug.a ./

test:

.PHONY: all asan build clean cov debug distclean test

include config.mk
