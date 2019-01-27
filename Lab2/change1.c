//sinple script that detects the id of the company and replaced the ascii by it's integer value.

#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h> 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void print(char* message);
bool isDigit(char c);

int main(int argc, char *argv[]){

    if(argc != 1){
        print("Provide the name of the file.");
        return 1;
    }

    int fd = open(argv[1], O_RDWR, 0644);

    if(fd == -1){
        print("Cannot create or open file");
        return 1;
    }

    char* start = NULL;
    char* end = NULL;
    bool startNum = true; 

    char buf[1];
    bool replace = true;
    bool numberFound = false;
    int numberToReplace[1];
    numberToReplace[0]=0;
    int numberOfDigits = 1;
    
 
    while (read(fd, buf, 1) > 0){
        if(replace)
        {
            if(isDigit(buf[0]))
            {
                numberFound= true;
                numberToReplace[0] = (numberToReplace[0] * 10) + (buf[0] - '0');
                numberOfDigits++;
            }
            else if(!isDigit(buf[0]) && numberFound)
            {
                printf("%d\n", numberToReplace[0]);
                printf("%d\n", numberOfDigits);
               
                lseek(fd, -numberOfDigits, SEEK_CUR);
                write(fd, &numberToReplace, numberOfDigits - 1);
                replace = false;
            }
        }

        if(buf[0] == '\n')
        {
            replace = true;
            numberFound = false;
            numberOfDigits = 1;
            numberToReplace[0] = 0;
        }
    }
    close(fd);
    return 0;
}

void print(char* message){
    write(STDOUT_FILENO, message, strlen(message));
}

bool isDigit(char c){
    return c >= '0' && c <= '9';
}
