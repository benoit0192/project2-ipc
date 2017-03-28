#include <lib.h> // _syscall and message
#include <unistd.h> // prototypes
#include <errno.h>
#include <assert.h>
#include <stdlib.h>


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
    m.m1_p1 = NULL; // ask for the message size
    m.m1_i2 = (int) getpid();
    int r = _syscall(PM_PROC_NR, PM_TOPIC_RETRIEVE, &m);

    /* check if there is a message to retrieve, or errors */
    if(r == -1) return -1;

    /* allocate buffer to store the message */
    assert(r > 0);
    *buf = malloc(r);
    if( !(*buf) ) {
        errno = ENOMEM;
        return -1;
    }

    m.m1_p1 = (char *)(*buf); // ask for data copy
    r = _syscall(PM_PROC_NR, PM_TOPIC_RETRIEVE, &m);
    if(r == -1) {
        free(*buf);
        *buf = NULL;
    }
    return r;
}
