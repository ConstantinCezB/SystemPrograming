#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc <= 1 || argc >= 7)
    {
        perror("not enough arguments");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= argc; i++)
    {
        if (fork() == 0)
        {
            sleep(atoi(argv[i]));
            printf("process %d terminated. pid=%d\n", i, getpid());
            exit(EXIT_SUCCESS);
        }
    }

    while (wait(NULL) > 0)
        ;
    return 0;
}