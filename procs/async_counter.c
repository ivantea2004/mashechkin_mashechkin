#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#if USE_ATOMIC
typedef atomic_int Type;
#else
typedef int Type;
#endif

enum {
    COUNT = 10000,
    THREADS = 10000
};

pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

void*
counter_thread(void *arg)
{
    Type *count = (Type*) arg;
#if USE_MUTEX
    for (int i =0; i < COUNT; i++)
    {
        pthread_mutex_lock(&global_mutex);
        (*count)++;
        pthread_mutex_unlock(&global_mutex);
    }
#else
    for (int i =0; i < COUNT; i++)
    {
        (*count)++;
    }
#endif
    return NULL;
}

int
main(void)
{
    pthread_t tids[THREADS];
    Type count = 0;
    for (int i = 0; i < THREADS; i++) {
        pthread_create(tids + i, NULL, &counter_thread, &count);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(tids[i], NULL);
    }
    printf("%d\n", count);

    pthread_mutex_destroy(&global_mutex);

    return 0;
}