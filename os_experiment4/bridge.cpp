#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
using namespace std;
#define NORTH 1
#define SOUTH 0

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
int num_on_bridge = 0;
time_t t;

void get_on(int direction)
{
    pthread_mutex_lock(&mutex);
    while (num_on_bridge >= 2)
        pthread_cond_wait(&notfull, &mutex);
    num_on_bridge++;
    if (direction == SOUTH)
        printf("有一个人从江安南上桥，目前桥上有%d个人。\n", num_on_bridge);
    else
        printf("有一个人从江安北上桥，目前桥上有%d个人。\n", num_on_bridge);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&notfull);
}
void get_off(int direction)
{
    pthread_mutex_lock(&mutex);
    num_on_bridge--;
    if (direction == SOUTH)
        printf("有一个人从江安南下桥，目前桥上有%d个人。\n", num_on_bridge);
    else
        printf("有一个人从江安北下桥，目前桥上有%d个人。\n", num_on_bridge);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&notfull);
}
void *south_process(void *args)
{
    for (int i = 0; i < 10; i++)
    {
        get_on(SOUTH);
        int k = (int)rand() % 5;
        sleep(k / 2.0);
        get_off(NORTH);
        sleep(k / 2.0);
    }
}
void *north_process(void *args)
{
    for (int i = 0; i < 10; i++)
    {
        get_on(NORTH);
        int k = (int)rand() % 5;
        sleep(k / 2.0);
        get_off(SOUTH);
        sleep(k / 2.0);
    }
}
int main()
{
    srand((unsigned)time (&t));
    pthread_t south_thread, north_thread;
    pthread_create(&south_thread, nullptr, &south_process, nullptr);
    pthread_create(&north_thread, nullptr, &north_process, nullptr);
    pthread_join(south_thread, nullptr);
    pthread_join(north_thread, nullptr);
    printf("程序结束。\n");
    return 0;
}
