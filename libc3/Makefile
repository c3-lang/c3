all: libc3.la

test:
	${MAKE} -C test

CLEANFILES = *.lo *.la

clean:
	rm -rf ${CLEANFILES}

.PHONY: all clean test

include config.mk
