#ifndef _LOCAL_H_
#define _LOCAL_H_

#include <unistd.h> // size_t
#include <errno.h>
#include <stdio.h>

// error code sign
#define _SIGN -

// redefine some minix types
typedef int topicid_t;
typedef void* vir_bytes;

// redefine message
typedef struct {
    int m1_i1;
    int m1_i2;
    int m1_i3;
    char *m1_p1;
} message;

#define EGENERIC 127

int topic_lookup(topicid_t *, size_t);
int topic_create(topicid_t);
int topic_publisher_subscribe(topicid_t);
int topic_publisher_unsubscribe(topicid_t);
int topic_client_subscribe(topicid_t);
int topic_client_unsubscribe(topicid_t);
int topic_publish(topicid_t, void*, size_t);
int topic_retrieve(topicid_t, void**);

int do_topic_lookup(void);
int do_topic_create(void);
int do_topic_publisher_subscribe(void);
int do_topic_publisher_unsubscribe(void);
int do_topic_client_subscribe(void);
int do_topic_client_unsubscribe(void);
int do_topic_publish(void);
int do_topic_retrieve(void);

// redefine _syscall
#define PM_TOPIC_LOOKUP 0
#define PM_TOPIC_CREATE 1
#define PM_TOPIC_PUBLISHER_SUBSCRIBE 2
#define PM_TOPIC_PUBLISHER_UNSUBSCRIBE 3
#define PM_TOPIC_CLIENT_SUBSCRIBE 4
#define PM_TOPIC_CLIENT_UNSUBSCRIBE 5
#define PM_TOPIC_PUBLISH 6
#define PM_TOPIC_RETRIEVE 7
#define PM_PROC_NR 0
message m_in;
int who_e;
int _syscall( int dummy, int syscall_id, message *m);

// redefine sys_datacopy
#define OK 0
int sys_datacopy(int dummy1, vir_bytes src, int dummy2, vir_bytes dst, size_t size);

#endif // _LOCAL_H_
