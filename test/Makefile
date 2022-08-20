## c3
## Copyright 2022 Thomas de Grivel <thoxdg@gmail.com>

all: c3_test

debug: c3_test.debug

CLEANFILES = c3_test c3_test.debug *.o *.lo
clean:
	rm -f ${CLEANFILES}

test: c3_test
	./c3_test

test_debug: c3_test.debug
	./c3_test.debug

.PHONY: all test

include config.mk
