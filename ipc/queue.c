#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

typedef struct {
    long type;
    char buff[100];
} Msg;

int
main(int argc, char **argv) {

    int error = 0;
    int q = -1;
    key_t key = IPC_PRIVATE;
#if !USE_PRIVATE
    if (argc < 2) {
        fprintf(stderr, "No file name for key generation.\n");
        error = 1;
        goto cleanup;
    } else {
        error = 1;
        key = ftok(argv[1], 1);
        if (key == -1) {
            perror(argv[1]);
            error = 1;
            goto cleanup;
        }
    }
#endif

    q = msgget(IPC_PRIVATE, 0600);

    if (q == -1) {
        perror("msgget");
        error = 1;
        goto cleanup;
    }
    Msg msg1 = {1, "Я процесс"};
    if (msgsnd(q, &msg1, strlen(msg1.buff) + 1, 0)) {
        perror("msgsnd");
        error = 1;
        goto cleanup;
    }

    Msg msg2;
    if (msgrcv(q, &msg2, sizeof(msg2.buff), 0, 0) < 0) {
        perror("msgrcv");
        error = 1;
        goto cleanup;
    }
    printf("%s\n", msg2.buff);
cleanup:
    if (q != -1 && msgctl(q, IPC_RMID, NULL)){
        perror(NULL);
        error = 1;
    }
    if (error) {
        exit(1);
    }

    return 0;
}