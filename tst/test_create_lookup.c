#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include "test.h"
/**
 * Creates topics, and look for them
 */

#define IDS_SIZE 5

int main(void) {
    int r;
    topicid_t ids[IDS_SIZE];

    test_begin("lookup empty topics");
    r = topic_lookup(ids, IDS_SIZE);
    assert(r == 0);
    test_end();

    test_begin("topic creation");
    r = topic_create(0);
    assert(r == 0);
    test_end();

    test_begin("creation of already existing topic");
    r = topic_create(0);
    assert(r == -1 && errno == EALREADY);
    test_end();

    test_begin("lookup empty topics");
    r = topic_lookup(ids, IDS_SIZE);
    assert(r == 1 && ids[0] == 0);
    test_end();

    test_begin("creation of multiple topics");
    for(int i = 1; i < IDS_SIZE; ++i) {
        r = topic_create(i);
        assert(r == 0);
    }
    test_end();

    test_begin("lookup multiple topic IDs");
    r = topic_lookup(ids, IDS_SIZE);
    assert(r == 5);
    for(int i = 0; i < IDS_SIZE; ++i)
        assert(ids[i] == i);
    test_end();

    test_begin("creation of one more topic");
    r = topic_create(IDS_SIZE);
    assert(r == 0);
    test_end();

    test_begin("lookup with too small buffer");
    r = topic_lookup(ids, IDS_SIZE);
    assert(r == -1 && errno == ERANGE);
    test_end();

    test_summary();

    return 0;
}
