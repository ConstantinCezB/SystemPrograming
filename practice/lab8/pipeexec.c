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
    if (argc != 2)
    {
        perror("Missing command argument");
        exit(EXIT_FAILURE);
    }

    // Creating the pipe for comunication between parent and child.
    int pfd[2];
    if (pipe(pfd) == -1)
    {
        perror("Error with pipe");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0)
    {
        // Writting to the pipe by redirecting the output of execlp
        // to the write section of the pipe.
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        execlp(argv[1], argv[1], (char *)NULL);
    }
    else
    {
        // Reading from pipe
        char buff[255];
        close(pfd[1]);
        read(pfd[0], buff, 255);
        close(pfd[0]);

        // Writing the answer to the file.
        int fd = open("./result.txt", O_CREAT | O_TRUNC | O_WRONLY, 644);
        write(fd, buff, strlen(buff));
        close(fd);

        // Printing the bytes stored in the file.
        printf("The result of %s is written into result.txt with total of %ld bytes.\n", argv[1], strlen(buff));
        exit(EXIT_SUCCESS);
    }

    return 0;
}