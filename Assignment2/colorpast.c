#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>


void print(char *message);
void colorMatch (char *color);
void writeHeader (int *fd, int picX, int picY);

//TODO: checking if the file is PPM, creating the file, writing the header, writing code with fix char, writing correct code.

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        print("too little or too many arguments!");
    }
        
    if (!(colorMatch(argv[2]) && colorMatch(argv[3]) && colorMatch(argv[4]) 
        && colorMatch(argv[5]) && colorMatch(argv[6])))
    {
        print("One or more colors are not from the available options.\n the available options are:\nred\ngreen\nblue\nyellow\norange\ncyan\nmagenta\nocean\nviolet");
    }
}

void print(char *message)
{
    write(STDOUT_FILENO, message, strlen(message));
}

void colorMatch (char *color) 
{
    return (strcmp(color, "red") != 0) || (strcmp(color, "green") != 0)
        || (strcmp(color, "blue") != 0) || (strcmp(color, "yellow") != 0)
        || (strcmp(color, "orange") != 0) || (strcmp(color, "cyan") != 0)
        || (strcmp(color, "magenta") != 0) || (strcmp(color, "ocean") != 0)
        || (strcmp(color, "violet")
}

void writeHeader (int *fd, int picX, int picY) {
    char specialChar[2] = {'\n', ' '};
    const int scale = 255;
    const int buffSize = ceil((float)ceil(log2(scaleInt))/8.0) * 3;
    const char *format = "P6";
    char buf[buffSize];

    write(*fdWrite, format, strlen(format));
    write(*fdWrite, &specialChar[0], sizeof(char));
    write(*fdWrite, picX, strlen(picX));
    write(*fdWrite, &specialChar[1], sizeof(char));
    write(*fdWrite, picY, strlen(picY));
    write(*fdWrite, &specialChar[0], sizeof(char));
    write(*fdWrite, scale, strlen(scale));
    write(*fdWrite, &specialChar[0], sizeof(char));
}