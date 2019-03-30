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
    if (argc < 2)
    {
        printf("NOTHING TO DO\n");
        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i <= argc; i += 2)
    {
        if (fork() == 0)
        {
            (i < argc || (i >= argc && i % 2 == 1)) ? execlp(argv[i], argv[i], argv[i + 1], (char *)NULL) : execlp(argv[i], argv[i], (char *)NULL);
        }
        else
        {
            wait(NULL);
        }
    }

    return 0;
}