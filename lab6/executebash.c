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

    int pid;

    pid = fork();
    if (pid < 0)
    {
        perror("fork error");
    }
    if (pid == 0)
    {
        execl ("./test.bash", "./test.bash", "./test.bash",(char*) NULL);
        exit(EXIT_SUCCESS);
    }

    while (wait(NULL) > 0);
    printf("STUDY! STUDY! STUDY!\n");

    return 0;
}
