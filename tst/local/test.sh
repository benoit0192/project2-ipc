#!/bin/bash

# This script copies sources files implementing the Topic IPC and
# compile them with local dependencies so they can be run on a Linux
# computer with valgrind for debugging purpose

# remove old sources
rm ./syscall.c
rm ./wrapper.c

# get real sources
cp ../../src/minix/servers/pm/topic.c ./syscall.c
cp ../../src/minix/lib/libc/sys/topic.c ./wrapper.c

# replace some includes
sed -i -e 's/#include "pm\.h"//g' ./syscall.c
sed -i -e 's/#include <minix\/syslib\.h>//g' ./syscall.c

sed -i -e 's/#include <lib\.h> \/\/ _syscall and message//g' ./wrapper.c
sed -i -e 's/#include <unistd\.h> \/\/ prototypes//g' ./wrapper.c

# add a new header
sed -i '1i#include "local.h"' syscall.c
sed -i '1i#include "local.h"' wrapper.c

# change erro code signs in syscall.c
err=( ERANGE EGENERIC EALREADY ENOMEM ENOENT EINVAL EACCES ENOBUFS ENOMSG )
for e in "${err[@]}"
do
    sed -i "s/$e/-$e/g" ./syscall.c
done



# copy test sources
rm test.c
rm test.h
cp ../test.c .
cp ../test.h .

tests=( simple_test test_create_lookup test_publishers test_subscribers )

for t in "${tests[@]}"
do
	cp ../$t.c .
    # add local header
    sed -i '1i#include "local.h"' $t.c
    # compile test
    gcc -g -Wall -o $t $t.c test.c wrapper.c syscall.c local.c
done
