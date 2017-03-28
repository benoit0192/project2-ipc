#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include "test.h"

/**
 * Creates topics, and look for them
 */

int main(void) {
    int r;

    test_begin("publisher registers to non existing topic");
    r = topic_publisher_subscribe(12);
    assert(r == -1 && errno == ENOENT);
    test_end();

    test_begin("publisher publish in non existing topic");
    r = topic_publish(12, (void*)"abc", 4);
    assert(r == -1 && errno == ENOENT);
    test_end();

    test_begin("topic creation");
    r = topic_create(12);
    assert(r == 0);
    test_end();

    test_begin("publisher publish in existing topic, without being registered");
    r = topic_publish(12, (void*)"abc", 4);
    assert(r == -1 && errno == EACCES);
    test_end();

    test_begin("publisher registers to existing topic");
    r = topic_publisher_subscribe(12);
    assert(r == 0);
    test_end();

    test_begin("publisher registers to existing topic, already registered");
    r = topic_publisher_subscribe(12);
    assert(r == -1 && errno == EALREADY);
    test_end();

    test_begin("publisher publish message of size 0");
    r = topic_publish(12, NULL, 0);
    assert(r == -1 && errno == EINVAL);
    test_end();

    test_begin("publisher publish in existing topic, registration OK");
    r = topic_publish(12, (void*)"abc", 4);
    assert(r == 0);
    test_end();

    test_begin("add one subscriber to this topic");
    r = topic_client_subscribe(12);
    assert(r == 0);
    test_end();

    test_begin("publisher publish in existing topic, previous message has no subscribers and is removed");
    r = topic_publish(12, (void*)"abc", 4);
    assert(r == 0);
    test_end();

    test_begin("publisher publish 4 more messages in existing topic");
    char msg[4][4] = {"def", "ghi", "jkl", "mno"};
    for(int i = 0 ; i < 4; ++i) {
        printf("%d... ", i+1); fflush(stdout);
        r = topic_publish(12, (void*)msg[i], 4);
        assert(r == 0);
    }
    test_end();

    test_begin("publisher publish in existing topic, but no place left");
    r = topic_publish(12, (void*)"pqr", 4);
    assert(r == -1 && errno == ENOBUFS);
    test_end();

    test_begin("publisher unregisters from existing topic");
    r = topic_publisher_unsubscribe(12);
    assert(r == 0);
    test_end();

    test_begin("publisher publish in existing topic, after unregistration");
    r = topic_publish(12, (void*)"stu", 4);
    assert(r == -1 && errno == EACCES);
    test_end();

    test_begin("publisher unregisters from existing topic, but not already registred");
    r = topic_publisher_unsubscribe(12);
    assert(r == -1 && errno == EINVAL);
    test_end();

    test_summary();

    return 0;
}
