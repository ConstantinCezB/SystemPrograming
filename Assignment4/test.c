#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>

void action(int dummy)
{
    printf("Switching\n");
}
int main(int argc, char *argv[])
{
    int pid[10];
    signal(SIGUSR1, action);


    for (int i = 0; i < 10; i++)
    {
        
        printf("works: %d\n", i);
        

        if ((pid[i] = fork()) == 0)
        {
            pause();
            printf("Child %d is running\n", getpid());
            kill(getppid(), SIGUSR1);
            exit(0);
        }
    }

    
    for (int i = 0; i < 10; i++)
    {
        printf("Parent is running\n");
        kill(pid[i], SIGUSR1);
        pause();
    }
    return 0;
}