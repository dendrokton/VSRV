#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

void* functionC(void* arg)
{
    pthread_mutex_lock(&mutex1);
    counter++;
    printf("Значение счетчика: %d\n", counter);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;
    int rc1, rc2;

    /* Создание первого потока */
    if ((rc1 = pthread_create(&thread1, NULL, &functionC, NULL))) {
        printf("Ошибка создания потока 1: %d\n", rc1);
        exit(1);
    }

    /* Создание второго потока */
    if ((rc2 = pthread_create(&thread2, NULL, &functionC, NULL))) {
        printf("Ошибка создания потока 2: %d\n", rc2);
        exit(1);
    }
    /* Ожидание завершения потоков */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Оба потока завершили работу.\n");
    return 0;
}