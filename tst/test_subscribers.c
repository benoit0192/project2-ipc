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
    char *msg = NULL, *tmp = NULL;
    size_t msg_size = 0;

    test_begin("subscriber registers to non existing topic");
    r = topic_client_subscribe(12);
    assert(r == -1 && errno == ENOENT);
    test_end();

    test_begin("subscriber retrieve from non existing topic");
    msg = topic_retrieve(12, msg, &msg_size);
    assert(msg == NULL && errno == ENOENT);
    test_end();

    test_begin("topic creation");
    r = topic_create(12);
    assert(r == 0);
    test_end();

    test_begin("subscriber retrieves from existing topic, without being registered");
    msg = topic_retrieve(12, msg, &msg_size);
    assert(msg == NULL && errno == EACCES);
    test_end();

    test_begin("subscriber registers to existing topic");
    r = topic_client_subscribe(12);
    assert(r == 0);
    test_end();

    test_begin("subscriber registers to existing topic, already registered");
    r = topic_client_subscribe(12);
    assert(r == -1 && errno == EALREADY);
    test_end();

    test_begin("retrieve message, but no message available");
    msg = topic_retrieve(12, msg, &msg_size);
    assert(msg == NULL && errno == ENOMSG);
    test_end();

    test_begin("add publisher");
    r = topic_publisher_subscribe(12);
    assert(r == 0);
    test_end();

    test_begin("publish");
    r = topic_publish(12, "abc", 4);
    assert(r == 0);
    test_end();

    test_begin("subscriber retrieve from existing topic, result is allocated");
    msg = topic_retrieve(12, msg, &msg_size);
    assert(msg != NULL && msg_size == 4 && msg[0] == 'a' && msg[1] == 'b' && msg[2] == 'c' && msg[3] == 0);
    test_end();

    test_begin("subscriber retrieve from existing topic, no message left");
    tmp = topic_retrieve(12, msg, &msg_size);
    assert(tmp == NULL && errno == ENOMSG);
    test_end();

    test_begin("publish");
    r = topic_publish(12, "def", 4);
    assert(r == 0);
    test_end();

    test_begin("subscriber retrieve from existing topic, buffer is reused");
    tmp = msg;
    msg = topic_retrieve(12, msg, &msg_size);
    assert(msg == tmp && msg_size == 4 && msg[0] == 'd' && msg[1] == 'e' && msg[2] == 'f' && msg[3] == 0);
    test_end();

    test_begin("publish 5 messages to fill buffer. previous messages have been removed");
    char msgs[5][4] = {"ghi", "jkl", "mno", "pqr", "stu"};
    for(int i = 0 ; i < 5; ++i) {
        printf("%d... ", i+1); fflush(stdout);
        r = topic_publish(12, (void*)msgs[i], 4);
        assert(r == 0);
    }
    test_end();

    test_begin("publisher publish in existing topic, but no place left");
    r = topic_publish(12, (void*)"vwx", 4);
    assert(r == -1 && errno == ENOBUFS);
    test_end();

    test_begin("subscriber unregisters from existing topic");
    r = topic_client_unsubscribe(12);
    assert(r == 0);
    test_end();

    test_begin("subscriber retrieves from existing topic, after unregistration");
    tmp = topic_retrieve(12, msg, &msg_size);
    assert(tmp == NULL && errno == EACCES);
    test_end();

    test_begin("subscriber unregisters from existing topic, but not already registred");
    r = topic_client_unsubscribe(12);
    assert(r == -1 && errno == EINVAL);
    test_end();

    test_summary();

    free(msg);

    return 0;
}
