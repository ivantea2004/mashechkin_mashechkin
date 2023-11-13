#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int
main(void)
{
    errno = 0;
    pid_t cpid = fork();
    if (cpid == -1) {
        perror("Faild to fork");
        exit(1);
    }
    if (cpid) {
        printf("From parent: parent=%d, self=%d, child=%d\n", getppid(), getpid(), cpid);        
    } else {
        printf("From child: parent=%d, self=%d\n", getppid(), getpid());
    }
    return 0;
}