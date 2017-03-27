#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>

// define some colors
#define COLOR_OK      "\033[32m"
#define COLOR_FAIL    "\033[31m"
#define COLOR_WARNING "\033[33m"
#define COLOR_RESET   "\033[39m"

void test_begin(char *name);
void test_end();
void test_summary();


#endif // __TEST_H__
