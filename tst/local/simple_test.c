#include "local.h"
#include <unistd.h>
#include <stdio.h>

/**
 * A simple test for syscall existence.
 * No compiling or linking errors should happend if the syscalls
 * are correctly defined.
 */

int main(void) {
    topicid_t topics[5];
    topic_lookup(topics, 5);
    int r = topic_create(42);
    printf("returned code: %d\n", r);
    topic_publisher_subscribe(12);
    topic_client_subscribe(12);
    topic_publish(12, (void*)"abc", 4);
    char *msg;
    topic_retrieve(12, (void**)&msg);
    topic_client_unsubscribe(12);
    topic_publisher_unsubscribe(12);

    return 0;
}
