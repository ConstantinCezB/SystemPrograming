#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

void parent(int *fd, char *argv[]);
void child(int *fd, char *argv[]);

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    fork() ? parent(fd, argv) : child(fd, argv);
}

void parent(int *fd, char *argv[])
{
    close(fd[0]);
    dup2(fd[1], 1);
    close(fd[1]);
    execlp(argv[1], argv[1], NULL);
}
void child(int *fd, char *argv[])
{
    close(fd[1]);
    dup2(fd[0], 0);
    close(fd[0]);
    int out_fd = open("result.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (!fork())
    {
        dup2(out_fd, 1);
        execlp("cat", "cat", NULL);
    }
    else
    {
        wait(NULL);
        int out_bytes = lseek(out_fd, 0, SEEK_END);
        printf("The result of %s is written into result.txt with total %d bytes.\n", argv[1], out_bytes);
        close(out_fd);
    }
}