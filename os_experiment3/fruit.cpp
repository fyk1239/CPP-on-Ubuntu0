#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
sem_t orange, apple, plate;
int ifOver = 0;

void *son_process(void *arg)
{
    printf("Son process begins to run.\n");
    while (1)
    {
        sem_wait(&orange);
        if (ifOver)
            break;
        printf("Son process eats an orange.\n");
        sem_post(&plate);
    }
}

void *daughter_process(void *arg)
{
    printf("Daughter process begins to run.\n");
    while (1)
    {
        sem_wait(&apple);
        if (ifOver)
            break;
        printf("Daughter process eats an apple.\n");
        sem_post(&plate);
    }
}

void *father_process(void *arg)
{
    printf("Father process begins to run.\n");
    int j = 0, k;
    time_t t;
    srand((unsigned)time(&t));
    for (j = 0; j < 5; j++)
    {
        sem_wait(&plate);
        k = (int)(rand() % 2);
        if (k == 0)
        {
            printf("Father process puts an orange.\n");
            sem_post(&orange);
        }
        else
        {
            printf("Father process puts an apple.\n");
            sem_post(&apple);
        }
    }
    sem_wait(&plate);
    ifOver = 1;
    sem_post(&orange);
    sem_post(&apple);
}

int main()
{
    sem_init(&plate, 0, 1);
    sem_init(&orange, 0, 0);
    sem_init(&apple, 0, 0);
    pthread_t father_thread, son_thread, daughter_thread;

    pthread_create(&father_thread, NULL, &father_process, NULL);
    pthread_create(&son_thread, NULL, &son_process, NULL);
    pthread_create(&daughter_thread, NULL, &daughter_process, NULL);

    pthread_join(father_thread, NULL);
    pthread_join(son_thread, NULL);
    pthread_join(daughter_thread, NULL);
    printf("The programme ends.\n");

    return 0;
}