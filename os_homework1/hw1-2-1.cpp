#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
// CPU的总体占用率50%
int main()
{
    struct timeval tv;
    long long start_time, end_time;
    long long busy_time = 8000;
    long long idle_time = 8000;
    
    while (1)
    {
        gettimeofday(&tv, NULL);
        start_time = tv.tv_sec * 1000000 + tv.tv_usec;
        end_time = start_time;

        while ((end_time - start_time) < busy_time)
        {
            gettimeofday(&tv, NULL);
            end_time = tv.tv_sec * 1000000 + tv.tv_usec;
        }
        usleep(idle_time);
    }
    return 0;
}