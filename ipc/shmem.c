#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/wait.h>

enum {
    BUF_SIZE = 100
};

int
main(void) {

    int error = 0;
    int shmem = -1;
    int should_destroy = 1;
    char *shmem_ptr = NULL;

    shmem = shmget(IPC_PRIVATE, BUF_SIZE, 0600);
    if (shmem == -1) {
        perror("shmget");
        error = 1;
        goto cleanup;
    }
    errno = 0;
    shmem_ptr = shmat(shmem, NULL, 0);

    if (errno) {
        shmem_ptr = NULL;
        perror("shmat");
        error = 1;
        goto cleanup;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        error = 1;
        goto cleanup;
    }
    if (!pid) {
        should_destroy = 0;
        if (snprintf(shmem_ptr, BUF_SIZE, "Я общая память") >= BUF_SIZE) {
            fprintf(stderr, "Too long text.\n");
            error = 1;
            goto cleanup;
        }
    } else {
        int res;
        if (wait(&res) < 0) {
            perror("wait\n");
            error = 1;
            goto cleanup;
        } else if (!(WIFEXITED(res) && WEXITSTATUS(res) == 0)) {
            fprintf(stderr, "Failed to write text.\n");
            error = 1;
            goto cleanup;
        }
        printf("%s\n", shmem_ptr);
    }

cleanup:
    if (shmem_ptr && shmdt(shmem_ptr)) {
        perror("shmdt");
        error = 1;
        goto cleanup;
    }
    if (should_destroy && shmem -1 && shmctl(shmem, IPC_RMID, NULL)) {
        perror("shmctl");
        error = 1;
    }

    if (error) {
        exit(1);
    }

    return 0;
}