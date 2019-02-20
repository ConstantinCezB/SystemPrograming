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
    if (argc < 1 || argc > 9)
    {
        printf("Too little or not enough arguments! Argument number: %d", argc);
        exit(EXIT_FAILURE);
    }
    int pid;
    for (int i = 1; i < argc; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork error");
        }
        if (pid == 0)
        {
            sleep(atoi(argv[i]));
            printf("process %d terminated. pid=%d\n", i, getpid());
            exit(EXIT_SUCCESS);
        }
    }

    while (wait(NULL) > 0);
    exit(EXIT_SUCCESS);
}
