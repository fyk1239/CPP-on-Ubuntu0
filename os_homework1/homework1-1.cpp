#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sched.h>
#define RUN_PERIOD_S 5
static void *thread_start(void *arg)
{
    int thread_num = *(int *)arg;
    printf("pthread %d started.\n", thread_num);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long last_ts = 1000000 * tv.tv_sec + tv.tv_usec;
    unsigned long init_ts = last_ts;
    unsigned long max_slice = 0;
    unsigned long loop_count = 0;
    while (1)
    {
        gettimeofday(&tv, NULL);
        unsigned long curr_ts = 1000000 * tv.tv_sec + tv.tv_usec;
        ++loop_count;
        unsigned long spent_period = curr_ts - last_ts;
        if (spent_period > 1000)
            printf("#%d Period: %luus\n", thread_num, spent_period);
        if (spent_period > max_slice)
            max_slice = spent_period;
        if (curr_ts - init_ts > RUN_PERIOD_S * 1000 * 1000)
            break;
        last_ts = curr_ts;
    }
    printf("loop %lu times; max: %luus\n", loop_count, max_slice);
    printf("thread %d exiting...\n", thread_num);
    return nullptr;
}
int main()
{
    printf("Begin to get the length of time slice...\n");
    pthread_t thread_1;
    pthread_attr_t attr_1;
    pthread_t thread_2;
    pthread_attr_t attr_2;

    pthread_attr_init(&attr_1);
    pthread_attr_init(&attr_2);

    cpu_set_t cpuset_1;
    CPU_ZERO(&cpuset_1);
    CPU_SET(1, &cpuset_1);
    pthread_attr_setaffinity_np(&attr_1, sizeof(cpu_set_t), &cpuset_1);
    pthread_attr_setaffinity_np(&attr_2, sizeof(cpu_set_t), &cpuset_1);

    int thread_num_1 = 1;
    int thread_num_2 = 2;
    pthread_create(&thread_1, &attr_1, &thread_start, &thread_num_1);
    pthread_create(&thread_2, &attr_2, &thread_start, &thread_num_2);

    sleep(RUN_PERIOD_S);

    void *result;
    pthread_join(thread_1, &result);
    pthread_join(thread_2, &result);

    pthread_attr_destroy(&attr_1);
    pthread_attr_destroy(&attr_2);
}