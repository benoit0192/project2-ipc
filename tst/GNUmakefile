TESTS=simple_test test_create_lookup test_publishers test_subscribers
CC=clang
CFLAGS=-Wall

build: $(TESTS)

%: %.c
	$(CC) -o $@ test.c $< $(CFLAGS)

clean:
	rm $(TESTS)

.PHONY: all clean
