
all: c3_test

CLEANFILES = c3_test *.o
clean:
	rm -f ${CLEANFILES}

test: c3_test
	./c3_test

.PHONY: all test

include config.mk
