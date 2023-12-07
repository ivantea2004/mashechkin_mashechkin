#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long type;
    int value;
} Msg;

enum {
    TYPE_MAX = 10,
    VALS_PER_TYPE = 3
};

int
main(void) {

    int error = 0;
    int queue = -1;

    queue = msgget(IPC_PRIVATE, 0600);

    if (queue == -1) {
        perror("msgget");
        error = 1;
        goto cleanup;
    }
    printf("written:\n");
    int prev_val = 0;
    for (int i = 1; i <= TYPE_MAX; i++) {
        for (int j = 0; j < VALS_PER_TYPE; j++) {
            Msg msg = {i, ++prev_val};
            if (msgsnd(queue, &msg, sizeof(msg.value), 0)) {
                perror("msgsnd");
                error = 1;
                goto cleanup;
            }
            printf("{type:%d, value: %d}\n", (int)msg.type, msg.value);
        }
    }
    printf("read:\n");
    while (1) {

        int what;

        if (scanf("%d", &what) != 1) {
            break;
        }

        Msg msg;

        if (msgrcv(queue, &msg, sizeof(msg.value), what, MSG_EXCEPT) == -1) {
            perror("msgrcv");
            error = 1;
            goto cleanup;
        }


        printf("{type:%d, value:%d}\n", (int) msg.type, msg.value);

    }

cleanup:

    if (queue != -1 && msgctl(queue, IPC_RMID, NULL)) {
        perror("msgctl");
        error = 1;
    }

    if (error) {
        exit(1);
    }

    return 0;
}