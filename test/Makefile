## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

CLEANFILES = *.a *.css *.gcno *.html ic3/*.diff ic3/*.err ic3/*.out ic3/*.ret *.o libc3_test libc3_test.asan libc3_test.cov libc3_test.debug .libs *.lo

CLEANFILES_COV = *.css *.gcda *.html .libs/*.gcda
CLEANFILES += ${CLEANFILES_COV}

DISTCLEANFILES = ${CLEANFILES} config.mk

build: libc3_test

all: build cov debug
	if ${HAVE_ASAN}; then ${MAKE} asan; fi

asan: libc3_test.asan

clean:
	rm -rf ${CLEANFILES}

clean_cov:
	rm -rf ${CLEANFILES_COV}

cov:   libc3_test.cov

debug: libc3_test.debug

distclean:
	rm -rf ${DISTCLEANFILES}

gcovr:
	gcovr --gcov-executable ${GCOV} --html-details test.html

ic3_test_cov:
	IC3=${C3_TOP}/ic3/ic3.cov time ./ic3_test

libc3_test_cov: libc3_test.cov
	time ./libc3_test.cov

test: libc3_test
	time ./libc3_test
	IC3=${C3_TOP}/ic3/ic3 time ./ic3_test

test_asan: libc3_test.asan
	time ./libc3_test.asan
	IC3=${C3_TOP}/ic3/ic3.asan time ./ic3_test

test_cov: libc3_test_cov ic3_test_cov

test_debug: libc3_test.debug
	time ./libc3_test.debug
	IC3=${C3_TOP}/ic3/ic3.debug time ./ic3_test

test_valgrind: libc3_test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./libc3_test

.PHONY: all asan cov debug clean clean_cov distclean ic3_test_cov libc3_test_cov test test_asan test_cov test_debug test_valgrind

include config.mk
