#include <unistd.h>

/**
 * A simple test for syscall existence.
 * No compiling or linking errors should happend if the syscalls
 * are correctly defined.
 */

int main(void) {
    topicid_t *topics;
    int nb_topics = topic_lookup(&topics);
    topic_create(12);
    topic_publisher_subscribe(12);
    topic_client_subscribe(12);
    topic_publish(12, (void*)"abc", 4);
    char *msg;
    int size = topic_retrieve(12, (void**)&msg);
    topic_client_unsubscribe(12);
    topic_publisher_unsubscribe(12);

    return 0;
}
