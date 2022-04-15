#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void *thread_function(void *arg);
int num[1005], i, n;
int main()
{
	n, i = 0;
	scanf("%d", &n);
	pthread_t a_thread;
	void *thread_result;
	int res = pthread_create(&a_thread, NULL, thread_function, (void*)n);
	if (res != 0)
	{
		perror("Thread craetion failed!\n");
		exit(EXIT_FAILURE);
	}
	res = pthread_join(a_thread, &thread_result);
	if (res != 0)
	{
		perror("Thread join failed!\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < n - 1; i++)
	{
		printf("%d ", num[i]);
	}
	printf("%d\n", num[n - 1]);
	exit(EXIT_SUCCESS);
	return 0;
}
void *thread_function(void *arg)
{
	num[0] = 0;
	num[1] = 1;
	for (i = 2; i < n; i++)
	{
		num[i] = num[i - 1] + num[i - 2];
	}
}
