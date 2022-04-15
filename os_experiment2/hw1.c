#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
	int n, pid;
	scanf("%d", &n);
	pid = fork();
	int f0 = 0, f1 = 1, f2 = 0, i = 0;
	if (pid < 0)
	{
		printf("ERROR!\n");
	}
	else if (pid == 0)
	{
		if (n < 0 || n == 0)
			printf("INPUT ERROR!\n");
		else if (n == 1)
			printf("0\n");
		else if (n == 2)
                        printf("0 1\n");

		else
		{
			printf("0 1");
			for (i = 0; i< n - 2; i++)
			{
				f2 = f0 + f1;
				f0 = f1;
				f1 = f2;
				printf(" %d", f2);
			}
			printf("\n");
		}
	}
	else
		wait(NULL);
	return 0;
}
