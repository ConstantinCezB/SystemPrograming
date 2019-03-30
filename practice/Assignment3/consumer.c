#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include<sys/types.h>
#include<signal.h>

int main (int argc, char **argv)
{
    // reding the file
    FILE *fp;
    if( (fp = fopen("./storage", "r")) == NULL){
        perror("error with fopen");
        exit(EXIT_FAILURE);
    }
    int number;
    fscanf(fp, "%d", &number);
    number -= 3;
    fclose(fp);

    // writing to the file
    if( (fp = fopen("./storage", "w")) == NULL){
        perror("error with fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d", number);
    fclose(fp);

    // printing the total
    printf("from consumer: current total is %d\n", number);

    // running producer
    execlp("./producer", "./producer", (char *)NULL);

    return 0;
}