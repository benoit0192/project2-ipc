#include "test.h"


void test_begin(char *name) {
    printf(COLOR_RESET "Test: %s... ", name);
    fflush(stdout);
}

void test_end() {
    printf(COLOR_RESET "[" COLOR_OK "OK" COLOR_RESET "]\n");
}

void test_summary() {
    printf(COLOR_OK "All tests succeeded!\n" COLOR_RESET);
}
