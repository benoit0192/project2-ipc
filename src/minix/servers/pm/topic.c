#include "pm.h"
#include <stdlib.h>
#include <stdio.h>
#include <minix/syslib.h>
#include <signal.h>

#define MAX_MSG_PER_TOPIC 5


/**
 * A user, identified by its process number.
 * And a pointer to make a list of users
 */
struct user_elt {
    pid_t pid;
    struct user_elt *next;
};

/**
 * A stored message with the list of user needing to retrieve it.
 */
struct topic_message {
    void *buf;
    size_t size;
    struct user_elt *to_retrieve;
    struct topic_message *next;
};

/**
 * A topic, with its id, its publishers, subscribers and its messages
 * And a pointer to make a list of topics
 */
struct topic {
    topicid_t id;
    struct user_elt *publishers;
    struct user_elt *subscribers;
    struct topic_message *messages;
    struct topic *next;
};

/**
 * The list of all created topics
 */
struct topic *topics = NULL;

/* print begining and end of syscall execution for debuging purpose */
//#define _TOPIC_DEBUG 1

#ifdef _TOPIC_DEBUG
#define _SYSCALL_BEGIN(name) {printf("** SYSCALL: " name " **\n");}
#else
#define _SYSCALL_BEGIN(name)
#endif
/* end of debuging */

/**
 * returns the address of the pointer referencing the topic with ID 'id'
 * or referencing NULL if not found.
 * The returned pointer is the last 'next' valid pointer (it can be the list head)
 */
struct topic **get_topic(topicid_t id) {
    struct topic **it;
    for(it = &topics; (*it) && (*it)->id != id; it = &(*it)->next);
    return it;
}

/**
 * Iterate over *list and returns the address of the pointer referencing
 * the user with pid 'pid', or referencing NULL if not found.
 * The returned pointer is the last 'next' valid pointer (it can be the list head)
 */
struct user_elt **get_user(struct user_elt **list, pid_t pid) {
    for(; (*list) && (*list)->pid != pid; list = &(*list)->next);
    return list;
}

/**
 * return the list of available topics to the user
 */
int do_topic_lookup(void) {
    _SYSCALL_BEGIN("topic_lookup()");
    int n;
    struct topic *it;

    /* retrieve arguments */
    vir_bytes topic_ids = (vir_bytes) m_in.m1_p1;
    size_t    count     = (size_t)    m_in.m1_i1;

    /* count topics to verify that topic_ids is large enough */
    for(it = topics, n = 0; it; it = it->next)
        if(++n > count) return ERANGE;

    /* special case when there is no topics */
    if(n == 0) return 0;

    /* generate topic IDs list */
    topicid_t ids[n];
    for(it = topics, n = 0; it; it = it->next)
        ids[n++] = it->id;

    /* copy result to user space */
    if(sys_datacopy(PM_PROC_NR, (vir_bytes)ids, who_e, (vir_bytes) topic_ids, n * sizeof(topicid_t)) != OK)
        return EGENERIC;

    /* return array size */
    return n;
}

/**
 * Add a new topic
 */
int do_topic_create(void) {
    _SYSCALL_BEGIN("topic_create()");
    struct topic **t;

    /* retrieve arguments */
    topicid_t id = (topicid_t) m_in.m1_i1;

    /* search for an already existing topic with the same id */
    t = get_topic(id);
    if( *t ) return EALREADY;

    /* create the new topic structure */
    *t = malloc(sizeof(**t));
    if( !(*t) ) return ENOMEM;

    /* set struct fields */
    (*t)->id = id;
    (*t)->publishers = NULL;
    (*t)->subscribers = NULL;
    (*t)->messages = NULL;
    (*t)->next = NULL;

    return OK;
}

 /**
  * Add a publisher to a topic
  */
int do_topic_publisher_subscribe(void) {
    _SYSCALL_BEGIN("topic_publisher_subscribe()");
    struct topic **t;
    struct user_elt **u;

    /* retrieve arguments */
    topicid_t id  = (topicid_t) m_in.m1_i1;
    pid_t     pid = (pid_t)     m_in.m1_i2;

    /* topic must exist */
    t = get_topic(id);
    if( !(*t) ) return ENOENT;

    /* look for user in publisher list */
    u = get_user( &(*t)->publishers, pid );
    /* user must not be arleady registered */
    if( *u ) return EALREADY;
    /* add user to the list */
    *u = malloc(sizeof(**u));
    if( !(*u) ) return ENOMEM;

    /* set struct fields */
    (*u)->pid = pid;
    (*u)->next = NULL;

    return OK;
}

/**
 * Remove a publisher to a topic
 */
int do_topic_publisher_unsubscribe(void) {
    _SYSCALL_BEGIN("topic_publisher_unsubscribe()");
    struct topic **t;
    struct user_elt **u, *tmp;

    /* retrieve arguments */
    topicid_t id  = (topicid_t) m_in.m1_i1;
    pid_t     pid = (pid_t)     m_in.m1_i2;

    /* topic must exist */
    t = get_topic(id);
    if( !(*t) ) return ENOENT;

    /* look for user in publisher list */
    u = get_user( &(*t)->publishers, pid );
    /* user must be arleady registered */
    if( !(*u) ) return EINVAL;
    /* remove user from the list */
    tmp = (*u)->next;
    free(*u);
    *u = tmp;

    return OK;
}

int do_topic_client_subscribe(void) {
    _SYSCALL_BEGIN("topic_client_subscribe()");
    struct topic **t;
    struct user_elt **u;

    /* retrieve arguments */
    topicid_t id  = (topicid_t) m_in.m1_i1;
    pid_t     pid = (pid_t)     m_in.m1_i2;

    /* topic must exist */
    t = get_topic(id);
    if( !(*t) ) return ENOENT;

    /* look for user in subscribers list */
    u = get_user( &(*t)->subscribers, pid );
    /* user must not be arleady registered */
    if( *u ) return EALREADY;
    /* add user to the list */
    *u = malloc(sizeof(**u));
    if( !(*u) ) return ENOMEM;

    /* set struct fields */
    (*u)->pid = pid;
    (*u)->next = NULL;

    return OK;
}

int do_topic_client_unsubscribe(void) {
    _SYSCALL_BEGIN("topic_client_unsubscribe()");
    struct topic **t;
    struct user_elt **u, *tmp;

    /* retrieve arguments */
    topicid_t id  = (topicid_t) m_in.m1_i1;
    pid_t     pid = (pid_t)     m_in.m1_i2;

    /* topic must exist */
    t = get_topic(id);
    if( !(*t) ) return ENOENT;

    /* look for user in subscribers list */
    u = get_user( &(*t)->subscribers, pid );
    /* user must be arleady registered */
    if( !(*u) ) return EINVAL;
    /* remove user from the list */
    tmp = (*u)->next;
    free(*u);
    *u = tmp;

    return OK;
}

/**
 * Publish a message in a specific topic
 */
int do_topic_publish(void) {
    _SYSCALL_BEGIN("topic_publish()");
    struct topic **t;
    struct user_elt **u, **u2, *u_tmp;
    struct topic_message **m, *m_tmp;
    int n;

    /* get arguments */
    topicid_t   id      = (topicid_t) m_in.m1_i1;
    vir_bytes   vir_buf = (vir_bytes) m_in.m1_p1;
    size_t      size    = (size_t)    m_in.m1_i2;
    pid_t       pid     = (pid_t)     m_in.m1_i3;

    /* check buffer size validity */
    if( size == 0 ) return EINVAL;

    /* topic must exist */
    t = get_topic(id);
    if( !(*t) ) return ENOENT;

    /* publisher must be registered */
    u = get_user( &(*t)->publishers, pid );
    if( !(*u) ) return EACCES;

    /* iterate over messages and remove those having an empty to_retrieve list */
    for(n = 0, m = &(*t)->messages; *m; m = &(*m)->next) {
        /* check if all waiting processes are still alive */
        for(u = &(*m)->to_retrieve; *u; u = &(*u)->next) {
            if( kill( (*u)->pid, 0) == -1 ) {
                if(errno == ESRCH) {
                    /* the process has died. we shouldn't be waiting for it */
                    /* remove it from the subscribers list */
                    u2 = get_user( &(*t)->subscribers, (*u)->pid );
                    u_tmp = (*u2)->next;
                    free(*u2);
                    *u2 = u_tmp;
                    /* remove it from the to_retrieve list */
                    u_tmp = (*u)->next;
                    free(*u);
                    *u = u_tmp;
                    if( !(*u) ) break; /* if it was the last element, don't iterate to the next */
                } else if(errno == EPERM) {
                    printf("*** do_topic_publish(): can't check if subscriber is alive ***\n");
                } else {
                    printf("*** do_topic_publish(): is 0 an invalid signal?!?! ***\n");
                }
            }
        }
        /* if the to_retrieve list is empty, remove this message from the topic */
        if( !(*m)->to_retrieve ) {
            free((*m)->buf);
            m_tmp = (*m)->next;
            free(*m);
            *m = m_tmp;
            if( !(*m) ) break; /* if it was the last element, don't iterate to the next */
        } else {
            n++;
        }
    }

    /* check if maximum number of messages in this topic is reached */
    if(n >= MAX_MSG_PER_TOPIC) return ENOBUFS;

    /* Add the new message in the topic */
    *m = malloc(sizeof(**m));
    if( !(*m) ) return ENOMEM;

    /* buffer is not contained in the message. get it from user space. */
    (*m)->buf = malloc(size);
    if( !(*m)->buf ) {
        free(*m);
        *m = NULL;
        return ENOMEM;
    }
    if( sys_datacopy(who_e, (vir_bytes) vir_buf, PM_PROC_NR, (vir_bytes)(*m)->buf, size) != OK ) {
        free((*m)->buf);
        free(*m);
        *m = NULL;
        return EGENERIC;
    }

    /* set struct fields */
    (*m)->size = size;
    (*m)->to_retrieve = NULL;
    (*m)->next = NULL;

    /* copy list of users who will retrieve this message */
    for(u = &(*t)->subscribers, u2 = &(*m)->to_retrieve; *u; u = &(*u)->next, u2 = &(*u2)->next) {
        *u2 = malloc(sizeof(**u2));
        /* can't copy user. free all previously copied users */
        if( !(*u2) ) {
            for(u2 = &(*t)->subscribers; *u2; u2 = &(*u2)->next) {
                u_tmp = (*u2)->next;
                free(*u2);
                *u2 = u_tmp;
            }
            free((*m)->buf);
            free(*m);
            *m = NULL;
            return ENOMEM;
        }
        /* malloc succeeded. set user value */
        (*u2)->pid = (*u)->pid;
        (*u2)->next = NULL;
    }

    return OK;
}

int do_topic_retrieve(void) {
    _SYSCALL_BEGIN("topic_retrieve()");

    struct topic **t;
    struct user_elt **u,*tmp;
    struct topic_message **m;

    /* get arguments */
    topicid_t   id      =  (topicid_t) m_in.m1_i1;
    vir_bytes   buf     =  (vir_bytes) m_in.m1_p1;
    pid_t       pid     =  (pid_t)     m_in.m1_i2;

    /* topic must exist */
    t = get_topic(id);
    if( !(*t) ) return ENOENT;

    /* subscriber must be registered */
    u = get_user( &(*t)->subscribers, pid );
    if( !(*u) ) return EACCES;

    for(m = &(*t)->messages; *m; m = &(*m)->next) {
        u = get_user( &(*m)->to_retrieve, pid );
        if( *u ) break;
    }

    /* no message for the user */
    if( !(*m) ) return ENOMSG;

    /* if there is a buffer, copy the message to user space */
    if( buf ) {
        if(sys_datacopy(PM_PROC_NR, (vir_bytes)(*m)->buf, who_e, (vir_bytes) buf, (*m)->size) != OK)
            return EGENERIC;
        tmp = (*u)->next;
        free(*u);
        *u = tmp;
    }
    return (*m)->size;
}
