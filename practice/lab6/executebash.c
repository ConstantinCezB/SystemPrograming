#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    if (fork() == 0)
    {
        execlp("./mybash", "./mybash", (char *)NULL);
    }
    else
    {
        wait(NULL);
        printf("STUDY! STUDY! STUDY!\n");
    }

    
    return 0;
}