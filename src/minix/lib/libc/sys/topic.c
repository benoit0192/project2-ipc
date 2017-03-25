#include <lib.h> // _syscall and message
#include <unistd.h> // prototypes


int topic_lookup(topicid_t **ids) {
    message m;
    // TODO: pass pointer in argument
    *ids = NULL;
    return _syscall(PM_PROC_NR, PM_TOPIC_LOOKUP, &m);
}

int topic_create(topicid_t id) {
    message m;
    m.m1_i1 = (int) id;
    return _syscall(PM_PROC_NR, PM_TOPIC_CREATE, &m);
}

int topic_publisher_subscribe(topicid_t id) {
    message m;
    m.m1_i1 = (int) id;
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISHER_SUBSCRIBE, &m);
}

int topic_publisher_unsubscribe(topicid_t id) {
    message m;
    m.m1_i1 = (int) id;
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISHER_UNSUBSCRIBE, &m);
}

int topic_client_subscribe(topicid_t id)  {
    message m;
    m.m1_i1 = (int) id;
    return _syscall(PM_PROC_NR, PM_TOPIC_CLIENT_SUBSCRIBE, &m);
}

int topic_client_unsubscribe(topicid_t id) {
    message m;
    m.m1_i1 = (int) id;
    return _syscall(PM_PROC_NR, PM_TOPIC_CLIENT_UNSUBSCRIBE, &m);
}

int topic_publish(topicid_t id, void *buf, size_t count) {
    message m;
    // TODO: pass msg in argument
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISH, &m);
}

int topic_retrieve(topicid_t id, void **buf) {
    message m;
    *buf = NULL;
    return _syscall(PM_PROC_NR, PM_TOPIC_RETRIEVE, &m);
}
