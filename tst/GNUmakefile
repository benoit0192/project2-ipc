TESTS=simple_test test_create_lookup
CC=clang
CFLAGS=-Wall

build: $(TESTS)

%: %.c
	$(CC) -o $@ test.c $< $(CFLAGS)

clean:
	rm $(TESTS)

.PHONY: all clean
