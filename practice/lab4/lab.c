#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>

int isprime(int n);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("Missing argument");
        exit(EXIT_FAILURE);
    }

    int number = atoi(argv[1]);

    if (number > 15 && number < 1)
    {
        perror("Number should be between that 1 and 15");
        exit(EXIT_FAILURE);
    }

    int fd = open("temp", O_CREAT | O_TRUNC | O_RDWR);
    write(fd, argv[1], strlen(argv[1]) + 1);
    lseek(fd, 0, SEEK_SET);

    if (fork() == 0)
    {
        char buff[255];
        read(fd, buff, 255);
        printf("String of the numebr: %s\n", buff);
        int number = atoi(buff);
        printf("number that is in child: %d\n", number);
        printf("Is the number a prime: %s\n", isprime(number)? "yes" : "no");
    }
    else
    {
        wait(NULL);
        printf("The child has finished printing\n");
    }

    close(fd);
    exit(EXIT_SUCCESS);
}

int isprime(int n)
{
    int i;
    if (n <= 1)
        return 0;
    for (i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
        return 1;
    }
}