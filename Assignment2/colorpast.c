#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>


void print(char *message);
bool colorMatch (char *color);
void writeHeader (int *fdWrite, char *picX, char *picY);
bool verifyFileName (char *fileName);
void createFileContent (int *fdWrite, int picX, int picY, int color) ;

//TODO: writing code with fix char, writing correct code.

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        print("too little or too many arguments!\n");
        exit(EXIT_FAILURE);
    }

    if(!verifyFileName(argv[1]))
    {
        print("You are not creating a file in the right format!\n");
        exit(EXIT_FAILURE);
    }
        
    if (!(colorMatch(argv[2]) && colorMatch(argv[3]) && colorMatch(argv[4]) 
        && colorMatch(argv[5]) && colorMatch(argv[6])))
    {
        print("One or more colors are not from the available options.\n the available options are:\nred\ngreen\nblue\nyellow\norange\ncyan\nmagenta\nocean\nviolet\n");
        exit(EXIT_FAILURE);
    }

    int fdWrite = open(argv[1], O_CREAT | O_WRONLY, 0644);

    if (fdWrite == -1)
    {
        print("Cannot create or open file\n");
        exit(EXIT_FAILURE);
    }

    writeHeader (&fdWrite, "1000", "1000"); 
    createFileContent (&fdWrite, 1000, 1000, 111);

    close(fdWrite);
    exit(EXIT_SUCCESS);
}

void print(char *message)
{
    write(STDOUT_FILENO, message, strlen(message));
}

bool colorMatch (char *color) 
{
    return (strcmp(color, "red") != 0) || (strcmp(color, "green") != 0)
        || (strcmp(color, "blue") != 0) || (strcmp(color, "yellow") != 0)
        || (strcmp(color, "orange") != 0) || (strcmp(color, "cyan") != 0)
        || (strcmp(color, "magenta") != 0) || (strcmp(color, "ocean") != 0)
        || (strcmp(color, "violet") != 0);
}

void writeHeader (int *fdWrite, char *picX, char *picY) {
    char specialChar[2] = {'\n', ' '};
    const char *scale = "255";
    const char *format = "P6";
    const int buffSize = 1;
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

bool verifyFileName (char *fileName) {
    bool fileExtension = false;
    char extension[30];
    int index = 0;

    int i;
    for(i = 0; i < strlen(fileName); i++)
    {
        if (fileExtension)
        {
            extension[index++] = fileName[i];
        }
        if(fileName[i] == '.') fileExtension = true;
    }
    extension[i] = '\0';

    return strcmp(extension, "ppm") == 0; 
}

int colorCharToInt (char *color)
{
    if (strcmp(color, "red")) 
    {
        return 16711680;
    }
    else if (strcmp(color, "green"))
    {
        return 65280;
    }


    return 16711680;
}

void createFileContent (int *fdWrite, int picX, int picY, int color) 
{   
    const char *scale = "255";
    const int buffSize = 3;
    int totalPixel = picX * picY;
    char buf[buffSize];
    buf[0] = 255;
    buf[1] = 0;
    buf[2] = 0;
    char buf1[buffSize];
    buf1[0] = 0;
    buf1[1] = 255;
    buf1[2] = 0;
    char buf2[buffSize];
    buf2[0] = 0;
    buf2[1] = 0;
    buf2[2] = 255;
    char buf3[buffSize];
    buf3[0] = 255;
    buf3[1] = 255;
    buf3[2] = 255;
    char buf4[buffSize];
    buf4[0] = 0;
    buf4[1] = 0;
    buf4[2] = 0;

    int indexMiddle = 0, indexMiddleCount = 0, indexMiddleActivation = 0;

    int indexX = 0, indexY = 0;

    for (int i = 0; i < totalPixel; i++)
    {
        //  printf("%d\n", indexMiddleActivation);
        if(indexX <= 500 - indexMiddle)
        {
            if (indexY <= 500)
            {
                write(*fdWrite, buf, buffSize);
            }
            else 
            {
                write(*fdWrite, buf1, buffSize);
            }
        }
        else if (indexX > 500 + indexMiddle)
        {
            if (indexY <= 500)
            {
                write(*fdWrite, buf2, buffSize);
            }
            else 
            {
                write(*fdWrite, buf3, buffSize);  
            }
        }
        else
        {
            write(*fdWrite, buf4, buffSize);
        }
        indexX++;
        if (indexX == picX)
        {
            indexX = 0;
            indexY++;
            if(indexMiddleActivation == 1)
            {
                indexMiddle++;
            }
            else if (indexMiddleActivation == 2)
            {
                indexMiddle--;
            }
            else
            {
                indexMiddle = 0;
            }
            
        }


        if(indexY >= 250 && indexY <= 500)
        {
            indexMiddleActivation = 1;
        }
        else if (indexY > 500 && indexY <= 750)
        {
            indexMiddleActivation = 2;
        }
        else 
        {
            indexMiddleActivation = 0;
        }
    }
}