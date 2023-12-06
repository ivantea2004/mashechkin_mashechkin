#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void*
parallel(void *arg)
{
    int *x = (int*)arg;
    *x = 5;

    printf("Started doing some job parallelly.\n");
    sleep(2);
    printf("Ended doing some job parallelly.\n");

    return NULL;
}

int
main(void)
{
    int value = 0;

    pthread_t tid;
    pthread_create(&tid, NULL, parallel, &value);
    
    printf("Can do some job here.\n");

    pthread_join(tid, NULL); // waiting for other thread

    printf("%d\n", value);

    return 0;
}