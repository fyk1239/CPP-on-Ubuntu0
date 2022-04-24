#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
int main()
{
    struct timeval tv;
    long long start_time, end_time;
    long long busy_time[1000];
    long long idle_time[1000];
    int i;
    for (i = 0; i < 1000; i++)
    {
        busy_time[i] = 10000 * 0.5 * (sin(i * 0.00628) + 1);
        idle_time[i] = 10000 - busy_time[i];
    }
    i = 0;
    while (1)
    {
        gettimeofday(&tv, NULL);
        start_time = tv.tv_sec * 1000000 + tv.tv_usec;
        end_time = start_time;

        while ((end_time - start_time) < busy_time[i])
        {
            gettimeofday(&tv, NULL);
            end_time = tv.tv_sec * 1000000 + tv.tv_usec;
        }
        usleep(idle_time[i]);
        i = (i + 1) % 1000;
    }
    return 0;
}