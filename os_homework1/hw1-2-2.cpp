#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#define RUNTIME 7000
// 单个CPU的占用率50% 运行程序+绑定核心
int main()
{
    struct timeval tv;
    long long start_time, end_time;

    while (1)
    {
        gettimeofday(&tv, NULL);
        start_time = tv.tv_sec * 1000000 + tv.tv_usec;
        end_time = start_time;

        while ((end_time - start_time) < RUNTIME)
        {
            gettimeofday(&tv, NULL);
            end_time = tv.tv_sec * 1000000 + tv.tv_usec;
        }
        usleep(RUNTIME);
    }
    return 0;
}