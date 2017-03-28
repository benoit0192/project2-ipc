#include "local.h"




int topic_lookup(topicid_t *ids, size_t count) {
    message m;
    m.m1_p1 = (char *)ids;
    m.m1_i1 = (int)   count;
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
    m.m1_i2 = (int) getpid();
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISHER_SUBSCRIBE, &m);
}

int topic_publisher_unsubscribe(topicid_t id) {
    message m;
    m.m1_i1 = (int) id;
    m.m1_i2 = (int) getpid();
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISHER_UNSUBSCRIBE, &m);
}

int topic_client_subscribe(topicid_t id)  {
    message m;
    m.m1_i1 = (int) id;
    m.m1_i2 = (int) getpid();
    return _syscall(PM_PROC_NR, PM_TOPIC_CLIENT_SUBSCRIBE, &m);
}

int topic_client_unsubscribe(topicid_t id) {
    message m;
    m.m1_i1 = (int) id;
    m.m1_i2 = (int) getpid();
    return _syscall(PM_PROC_NR, PM_TOPIC_CLIENT_UNSUBSCRIBE, &m);
}

int topic_publish(topicid_t id, void *buf, size_t count) {
    message m;
    m.m1_i1 = (int) id;
    m.m1_p1 = (char *)buf;
    m.m1_i2 = (int) count;
    m.m1_i3 = (int) getpid();
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISH, &m);
}

int topic_retrieve(topicid_t id, void **buf) {
    message m;
    m.m1_i1 = (int) id;
    m.m1_p1 = (char *) buf;
    m.m1_i2 = (int) getpid();
    m.m1_i3 = 0; // FLAG
    *buf = NULL;
    return _syscall(PM_PROC_NR, PM_TOPIC_RETRIEVE, &m);
}
