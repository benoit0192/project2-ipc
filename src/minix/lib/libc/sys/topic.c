#include <lib.h> // _syscall and message
#include <unistd.h> // prototypes
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#include <stdio.h>


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
    m.m1_p1 = (char *) buf;
    m.m1_i2 = (int) count;
    m.m1_i3 = (int) getpid();
    return _syscall(PM_PROC_NR, PM_TOPIC_PUBLISH, &m);
}

void *topic_retrieve(topicid_t id, void *buf, size_t *size) {
    message m;
    m.m1_i1 = (int) id;
    m.m1_p1 = (char *) buf; /* if buf is NULL, this ask for msg size */
    m.m1_i2 = (int) getpid();
    m.m1_i3 = (int) *size;
    void *msg;
    int r = _syscall(PM_PROC_NR, PM_TOPIC_RETRIEVE, &m);

    /* if an error occured, there is no result to return */
    if(r == -1) return NULL;

    /* if a buffer was provided and we reach this point, then it has been filled with the message */
    if( buf ) {
        *size = (size_t) r; /* return the size of the message */
        return buf;
    }

    /* otherwise we just retrieved the message size. we need to allocate a buffer to store the message */
    msg = malloc(r);
    if( !msg ) {
        errno = ENOMEM;
        return NULL;
    }

    m.m1_i1 = (int) id; /* m has been modified by previous syscall. reset its fields */
    m.m1_p1 = (char *) msg; /* ask for data copy */
    m.m1_i2 = (int) getpid();
    m.m1_i3 = r;
    r = _syscall(PM_PROC_NR, PM_TOPIC_RETRIEVE, &m);

    /* if an error occured, there is no result to return */
    if(r == -1) {
        free(msg);
        msg = NULL;
    } else {
        /* no error occured, r is the size of the message */
        *size = (size_t) r;
    }

    return msg;
}
