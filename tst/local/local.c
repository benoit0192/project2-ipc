#include "local.h"

int _syscall( int dummy, int syscall_id, message *m) {
    m_in = *m;
    int ret = -2;
    switch (syscall_id) {
        case PM_TOPIC_LOOKUP:
            ret = do_topic_lookup();
            break;
        case PM_TOPIC_CREATE:
            ret = do_topic_create();
            break;
        case PM_TOPIC_PUBLISHER_SUBSCRIBE:
            ret = do_topic_publisher_subscribe();
            break;
        case PM_TOPIC_PUBLISHER_UNSUBSCRIBE:
            ret = do_topic_publisher_unsubscribe();
            break;
        case PM_TOPIC_CLIENT_SUBSCRIBE:
            ret = do_topic_client_subscribe();
            break;
        case PM_TOPIC_CLIENT_UNSUBSCRIBE:
            ret = do_topic_client_unsubscribe();
            break;
        case PM_TOPIC_PUBLISH:
            ret = do_topic_publish();
            break;
        case PM_TOPIC_RETRIEVE:
            ret = do_topic_retrieve();
            break;
        default:
            printf("*** UNKNOWN SYSCALL ***\n");
    }
    if(ret < 0) { errno = -ret; return -1; }
    return ret;
}


int sys_datacopy(int dummy1, vir_bytes src, int dummy2, vir_bytes dst, size_t size) {
    for(size_t i = 0; i < size; ++i)
        ((char*)dst)[i] = ((char*)src)[i];
    return 0;
}
