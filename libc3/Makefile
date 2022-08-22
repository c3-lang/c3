## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

CLEANFILES = *.a *.gcno libc3.la *.la .libs *.lo *.o

CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda
CLEANFILES += ${CLEANFILES_COV}

all: libc3.la libc3.a

asan: libc3.asan.la libc3.asan.a

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov: libc3.cov.la libc3.cov.a

debug: libc3.debug.la libc3.debug.a

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

.PHONY: all asan clean cov debug test

include config.mk
