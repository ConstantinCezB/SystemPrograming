#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>

int main (int argc, char *argv[]) {

    int fd = open("list1.txt", O_CREAT | O_WRONLY, 0644);
    char openErrorMessage[] = "Cannot create or open file";

    if(fd == -1){
        write(STDOUT_FILENO, openErrorMessage, sizeof(openErrorMessage) - 1);
        exit(1);
    } 

    char store_1[] = "101   GM\tBuick\t2010\n",
         store_2[] = "102   Ford\tLincon\t2005\n";

    write(fd, store_1, sizeof(store_1) - 1);
    write(fd, store_2, sizeof(store_2) - 1);

    close(fd);
    return 0;
}