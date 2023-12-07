#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/wait.h>

enum {
    PROCS = 10
};

int
sem_down(int sems, int n) {
    struct sembuf buf = {0};
    buf.sem_num = n;
    buf.sem_op = -1;
    if (semop(sems, &buf, 1)) {
        perror("semop");
        return -1;
    }
    return 0;
}

int
sem_up(int sems, int n) {
    struct sembuf buf = {0};
    buf.sem_num = n;
    buf.sem_op = 1;
    if (semop(sems, &buf, 1)) {
        perror("semop");
        return -1;
    }
    return 0;
}

int
main(void) {

    int error = 0;
    int should_destroy = 1;
    int* curr_value = NULL;
    int shmem = -1;
    int sems = -1;

    sems = semget(IPC_PRIVATE, PROCS, 0600);
    if (sems == -1) {
        perror("semget");
        error = 1;
        goto cleanup;
    }
    shmem = shmget(IPC_PRIVATE, sizeof(*curr_value), 0600);
    if (shmem == -1) {
        perror("shmget");
        error = 1;
        goto cleanup;
    }
    errno = 0;
    curr_value = shmat(shmem, NULL, 0);
    if (errno) {
        curr_value = NULL;
        perror("shmat");
        error = 1;
        goto cleanup;
    }

    *curr_value = 0;

    for (int i = 0; i < PROCS; i++) {
        if (!fork()) {
            should_destroy = 0;
            if (sem_down(sems, i)) {
                error = 1;
                goto cleanup;
            }
            *curr_value += 1;
            printf("%d waited for %d\ncurr_value: %d\n", getpid(), i, *curr_value);
            if (i + 1 < PROCS) {
                if (sem_up(sems, i + 1)) {
                    error = 1;
                    goto cleanup;
                }
            }
            goto cleanup;
        }
    }

    if (sem_up(sems, 0)) {
        error = 1;
        goto cleanup;
    }

    while (wait(NULL) > 0);

    printf("value: %d\n", *curr_value);

cleanup:

    if (curr_value && shmdt(curr_value)) {
        perror("shmdt");
        error = 1;
    }

    if (should_destroy) {
        if (shmem != -1 && shmctl(shmem, IPC_RMID, NULL)) {
            perror("shmctl");
            error = 1;
        }
        if (sems != -1 && semctl(sems, 0, IPC_RMID)) {
            perror("semctl");
            error = 1;
        }
    }
    if (error) {
        exit(1);
    }
    return 0;
}