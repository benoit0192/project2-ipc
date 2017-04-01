#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "test.h"

char *instance = NULL;

char *msg_invalid_choice = "Invalid choice";

char *msg = NULL;

void clear_input_buf() {
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

void clear_console() {
    printf("\033[H\033[J");
}

int menu() {
    int choice;
    printf("What do you want to do?\n");
    printf("\n");
    printf("    1. lookup existing topics\n");
    printf("    2. create a topic\n");
    printf("    3. register as a publisher\n");
    printf("    4. unregister as a publisher\n");
    printf("    5. register as a subscriber\n");
    printf("    6. unregister as a subscriber\n");
    printf("    7. publish a message\n");
    printf("    8. retrieve a message\n");
    printf("\n");
    printf("    0. exit\n");
    printf("\n");
    if(msg != NULL) {printf("%s\n", msg); msg = NULL;}
    printf("Your choice? "); fflush(stdout);
    scanf("%d", &choice);
    clear_input_buf();
    return choice;
}

int display_errno(int ret) {
    if(ret >= 0) {
        printf("Function returned without error.\n");
    } else { // ret == -1
        printf("Function returned with the error: ");
        switch (errno) {
            case ERANGE:
                printf("ERANGE"); break;
            case EGENERIC:
                printf("EGENERIC"); break;
            case EALREADY:
                printf("EALREADY"); break;
            case ENOMEM:
                printf("ENOMEM"); break;
            case ENOENT:
                printf("ENOENT"); break;
            case EINVAL:
                printf("EINVAL"); break;
            case EACCES:
                printf("EACCES"); break;
            case ENOBUFS:
                printf("ENOBUFS"); break;
            case ENOMSG:
                printf("ENOMSG"); break;
            default:
                printf("%d (this error shouldn't occure)", errno);
        }
        printf("\n");
    }
    return ret;
}

void do_lookup(void) {
    int size = -1;
    while(size <= 0) {
        printf("What size for the passed buffer? (must be positive)\n");
        scanf("%d", &size);
        clear_input_buf();
    }
    topicid_t topics[size];
    int ret = display_errno(topic_lookup(topics, size));
    if(ret >= 0) printf("topics: ");
    for(int i = 0; i < ret; i++) {
        printf("%d ", topics[i]);
    }
    printf("\n");
}

void do_create(void) {
    topicid_t id;
    printf("What is the topic ID to create?\n");
    scanf("%d", &id);
    clear_input_buf();
    int ret = display_errno(topic_create(id));
    if(ret == 0) printf("created topic %d\n", id);
}

void do_pub_reg(void) {
    topicid_t id;
    printf("What is the ID of the topic you want to register to as a publisher?\n");
    scanf("%d", &id);
    clear_input_buf();
    int ret = display_errno(topic_publisher_subscribe(id));
    if(ret == 0) printf("registered as publisher to topic %d\n", id);
}

void do_pub_unreg(void) {
    topicid_t id;
    printf("What is the ID of the topic you want to unregister from as a publisher?\n");
    scanf("%d", &id);
    clear_input_buf();
    int ret = display_errno(topic_publisher_unsubscribe(id));
    if(ret == 0) printf("unregistered publisher from topic %d\n", id);
}

void do_sub_reg(void) {
    topicid_t id;
    printf("What is the ID of the topic you want to register to as a subscriber?\n");
    scanf("%d", &id);
    clear_input_buf();
    int ret = display_errno(topic_client_subscribe(id));
    if(ret == 0) printf("registered as subscriber to topic %d\n", id);
}

void do_sub_unreg(void) {
    topicid_t id;
    printf("What is the ID of the topic you want to unregister from as a subscriber?\n");
    scanf("%d", &id);
    clear_input_buf();
    int ret = display_errno(topic_client_unsubscribe(id));
    if(ret == 0) printf("unregistered as subscriber from topic %d\n", id);
}

void do_publish(void) {
    topicid_t id;
    printf("What is the ID of the topic you want to publish to?\n");
    scanf("%d", &id);
    clear_input_buf();
    printf("Type your message:\n");
    char buf[101];
    int n = -1;
    while(n <= 0)
        n = read(0, buf, 100);
    if(buf[n-1] == '\n') n--;
    buf[n] = 0; // null terminate string
    int ret = display_errno(topic_publish(id, (void*)buf, n+1));
    if(ret == 0) printf("Published in topic %d the message: \"%s\"\n", id, buf);
}

void do_retrieve(void) {
    topicid_t id;
    printf("What is the ID of the topic you want to retrieve a message from?\n");
    scanf("%d", &id);
    clear_input_buf();
    char buf[101];
    size_t n = 101;
    void *ret_ptr = topic_retrieve(id, (void*)buf, &n);
    int ret = display_errno(ret_ptr == NULL ? -1 : 0);
    if(ret == 0) printf("Retrieved message from topic %d: \"%s\"\n", id, buf);
}

void (*choice_fct[9])() = {
    NULL, // exit
    do_lookup,
    do_create,
    do_pub_reg,
    do_pub_unreg,
    do_sub_reg,
    do_sub_unreg,
    do_publish,
    do_retrieve
};

int main(int argc, char* argv[]) {
    if(argc >= 2) // save instance name if provided
        instance = argv[1];

    while(1) {
        clear_console();
        if(instance != NULL) printf("instance=%s\n", instance);
        int choice = menu();
        if(choice == 0) break; // exit
        if(choice < 1 || choice > 8) {
            msg = msg_invalid_choice;
            continue;
        }
        (*choice_fct[choice])();
        printf("\nHit any key"); fflush(stdout); getchar(); // pause
    }

    return 0;
}
