#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int
main(void)
{
    char name[100] = {0};
    printf("What to run?\n");
    if (!fgets(name, sizeof(name), stdin)) {
        fprintf(stderr, "Failed to read.\n");
        exit(1);
    }
    name[strlen(name) - 1] = '\0';

    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        exit(1);
    }
    fflush(stdout);
    fflush(stderr);
    if (pid) { // in parent
        int res;
        wait(&res); // wait for child
        if (WIFEXITED(res)) {
            printf("Finished wth exit code: %d\n", WEXITSTATUS(res));
        } else {
            printf("Failed.\n");
        }
    } else { // in child
        execlp(name, name, NULL);
        perror("Failed to exec");
        exit(1);
    }
    
    return 0;
}