## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

CLEANFILES = *.a ic3 ic3.asan ic3.cov ic3.debug *.css *.gcno *.html *.o .libs *.lo
CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda

build: ic3

all:   build cov debug
	if ${HAVE_ASAN}; then ${MAKE} asan; fi

asan: ic3.asan

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov:   ic3.cov

debug: ic3.debug

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details ic3.html

.PHONY: all asan cov debug clean clean_cov

include config.mk
