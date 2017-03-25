#include <stdio.h>

int do_topic_lookup(void) {
    printf("** SYSCALL: topic_lookup() **\n");
    return 0;
}

int do_topic_create(void) {
    printf("** SYSCALL: topic_create() **\n");
    return 0;
}

int do_topic_publisher_subscribe(void) {
    printf("** SYSCALL: topic_publisher_subscribe() **\n");
    return 0;
}

int do_topic_publisher_unsubscribe(void) {
    printf("** SYSCALL: topic_publisher_unsubscribe() **\n");
    return 0;
}

int do_topic_client_subscribe(void) {
    printf("** SYSCALL: topic_client_subscribe() **\n");
    return 0;
}

int do_topic_client_unsubscribe(void) {
    printf("** SYSCALL: topic_client_unsubscribe() **\n");
    return 0;
}

int do_topic_publish(void) {
    printf("** SYSCALL: topic_publish() **\n");
    return 0;
}

int do_topic_retrieve(void) {
    printf("** SYSCALL: topic_retrieve() **\n");
    return 0;
}
