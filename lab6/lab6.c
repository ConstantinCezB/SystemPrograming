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
    for (int i = 1; i < argc; i = i + 2)
    {
    
        pid = fork();
        if (pid < 0)
        {
            perror("fork error");
        }
        if (pid == 0)
        {
            printf("PID: %d\n", getpid());
            execlp( argv[i], argv[i], argv[i + 1], (char*) 0);
            exit(EXIT_SUCCESS);
        }else{
            wait(NULL);
        }
       
    }

    exit(EXIT_SUCCESS);
}
