#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
    pid_t child_pid = fork();
    if (child_pid == -1) {
        fprintf(stderr, "Failed to fork\n");
        exit(1);
    }
    if (child_pid) {
        printf("parent\n");
    } else {
        printf("child\n");
    }
    return 0;
}
