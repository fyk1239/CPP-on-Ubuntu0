#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex1 PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t mutex2 PTHREAD_ADAPTIVE_MUTEX_INITIALIZER_NP;

void *print_msg1(void *arg)
{
    int i = 0;

    printf("print_msg1 Try to lock mutex1.\n");
    pthread_mutex_lock(&mutex1);

    for (i = 0; i < 15; i++)
    {
        printf("print_msg1 output : %d\n", i);
        usleep(100);
    }

    printf("print_msg1 Try to lock mutex2.\n");
    pthread_mutex_lock(&mutex2);

    pthread_mutex_unlock(&mutex1);
}

void *print_msg2(void *arg)
{
    int i = 0;

    printf("print_msg2 Try to lock mutex2.\n");
    pthread_mutex_lock(&mutex2);

    for (i = 0; i < 15; i++)
    {
        printf("print_msg2 output : %d\n", i);
        usleep(100);
    }

    printf("print_msg2 Try to lock mutex1.\n");
    pthread_mutex_lock(&mutex1);

    pthread_mutex_unlock(&mutex2);
}

int main(int argc, char **argv)
{
    pthread_t id1;
    pthread_t id2;
    pthread_create(&id1, NULL, print_msg1, NULL);
    pthread_create(&id2, NULL, print_msg2, NULL);
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    return 1;
}