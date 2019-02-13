#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>

int isprime(int n)
{
    int i;
    if (n <= 1)
        return 0;
    for (i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int main()
{
    int n;
    char str_n[3];
    int fd;
    char file_name[] = "data";

    if (fork())
    {
        // parent
        scanf("%d", &n);
        if (1 > n || n > 15)
        {
            printf("Number is not in range [1, 15].\n");
        }
        else
        {
            if ((fd = open(file_name, O_WRONLY | O_CREAT)) != -1)
            {
                sprintf(str_n, "%d", n);
                write(fd, str_n, strlen(str_n));
                close(fd);
            }
        }
    }
    else
    {
        // child
        sleep(1); // I couldn't find another way to do it
        if ((fd = open(file_name, O_RDONLY)) != -1)
        {
            read(fd, str_n, sizeof(str_n));
            n = atoi(str_n);
            printf("%d is%s a prime.\n", n, isprime(n) ? "" : " not");
            close(fd);
        }
    }

    wait(NULL);

    return 0;
}
