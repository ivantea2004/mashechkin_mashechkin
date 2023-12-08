#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

enum {
    COUNT = 1000
};

typedef struct
{
    pthread_mutex_t mutex;
} Resource;

pthread_mutex_t global = PTHREAD_MUTEX_INITIALIZER;

Resource resources[COUNT];
volatile int end = 0;

void
resource_lock(Resource *res)
{
    pthread_mutex_lock(&res->mutex);
}

void
resource_unlock(Resource *res)
{
    pthread_mutex_unlock(&res->mutex);
}

/*

Считаем, что философы(пользователи) сидят за круглым столом.
Каждому философу нужно взять правую и левую вилку (ресурс).

*/
void
user(int id)
{
    printf("user id: %d\n", id);
    
    int left = id; // Считаем номера левой и правой вилок
    int right = (id + 1) % COUNT;

#if USE_ORDER // Если есть флаг, то будем соблюдать порядок захвата вилок (ресурсов), так чтобы всегда сначала захватывался левый ресурс, а потом правый
    if (left > right) {
        int tmp = left;
        left = right;
        right = tmp;
    }
#endif

    while (!end) { // Философы едят, пока не придёт время заканчивать

#if USE_GLOBAL_MUTEX // Если установлен флаг, то каждый захват вилок (ресурсов)
                     // будет происходить синхронизированно с другими захватами, что гарантируется использованием глобального мьютекса
        pthread_mutex_lock(&global);
#endif
        resource_lock(resources + left);
        resource_lock(resources + right);
#if USE_GLOBAL_MUTEX
        pthread_mutex_unlock(&global);
#endif
        printf("%d locked %d %d\n", id, left, right); // Вывоим информацию о захвате вилок

        resource_unlock(resources + left);
        resource_unlock(resources + right);
    }

}

void* user_thread(void *arg)
{
    long id = (long)arg;
    user(id);
    return NULL;
}

int
main(void)
{
    pthread_t users[COUNT];
    for (int i = 0; i < COUNT; i++) {
        pthread_mutex_init(&resources[i].mutex, NULL);
    }

    for (int i = 0; i < COUNT; i++) {
        pthread_create(users + i, NULL, &user_thread, (void*)(long) i);
    }

    sleep(5); // Ждём и говорим философам заканчивать
    end = 1;
    
    for (int i = 0; i < COUNT; i++) {
        pthread_join(users[i], NULL);
    }
    
    for (int i = 0; i < COUNT; i++) {
        pthread_mutex_destroy(&resources[i].mutex);
    }

    return 0;
}