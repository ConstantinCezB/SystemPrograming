#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>

void signalHandler(int action)
{
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Missing the file name.\n");
        exit(EXIT_SUCCESS);
    }

    char *buff[255];
    buff[0] = "EXAM! EXAM! EXAM!\n";
    buff[1] = "HELP! HELP! HELP!\n";
    buff[2] = "STUDY! STUDY! STUDY!\n";

    signal(SIGUSR1, signalHandler);
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, 0644);
    printf("parent opened a new file: %s\n", argv[1]);
    int pidChildren[2];

    for (int i = 0; i < 2; i++)
    {
        if ((pidChildren[i] = fork()) == 0)
        {

            printf("parent created child process: %d\n", getpid());
            pause();
            write(fd, buff[i], strlen(buff[i]));
            printf("%d has written to file: %s", getpid(), buff[i]);
            kill(getppid(), SIGUSR1);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        kill(pidChildren[i], SIGUSR1);
        pause();
    }

    write(fd, buff[2], strlen(buff[2]));
    printf("parrent has written to file: %s", buff[2]);
    close(fd);
    printf("parent has closed the file\n");
    exit(EXIT_SUCCESS);
}