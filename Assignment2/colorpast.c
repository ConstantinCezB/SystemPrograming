#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>

static int fdWrite;

bool checkIfInt (char *string);
void print(char *message);
bool colorMatch (char *color);
void writeHeader (int *fdWrite, char *picX, char *picY);
bool verifyFileName (char *fileName);
void getColor (char **buf, char *color);
void createFileContent (int *fdWrite, int picX, int picY, char *buf, char *buf1, char *buf2, char *buf3, char *buf4);
void drawImage (int *fdWrite, char *picX, char *picY, char *buf, char *buf1, char *buf2, char *buf3, char *buf4);


//TODO: writing correct code.

int main(int argc, char *argv[])
{

    if (argc != 9)
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

     if(!(checkIfInt(argv[7]) && checkIfInt(argv[8])))
    {
        print("You have to enter integer when talking about the size of the picture!\n");
        exit(EXIT_FAILURE);
    }

    fdWrite = open(argv[1], O_CREAT | O_WRONLY, 0644);

    if (fdWrite == -1)
    {
        print("Cannot create or open file\n");
        exit(EXIT_FAILURE);
    }

    char *buf, *buf1, *buf2, *buf3, *buf4;
    getColor (&buf, argv[3]);
    getColor (&buf1, argv[5]);
    getColor (&buf2, argv[4]);
    getColor (&buf3, argv[6]);
    getColor (&buf4, argv[2]);

    int rowsPerChild = atoi(argv[8]) / 10;

    int pid;
    for (int i = 1; i <10; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork error");
        }
        if (pid == 0)
        {
            drawImage (&fdWrite, argv[7], argv[8], buf, buf1, buf2, buf3, buf4);
            exit(EXIT_SUCCESS);
        }
        else
        {
            wait(NULL);
        }
        
    }

    free(buf);
    free(buf1);
    free(buf2);
    free(buf3);
    free(buf4);
    close(fdWrite);
    exit(EXIT_SUCCESS);
}

bool checkIfInt (char *string)
{
    for(int i = 0; i < strlen(string); i++)
    {
        if(!(string[i] >= '0' && string[i] <= '9'))
        {
            return false;
        }
    }
    return true;
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
    extension[index] = '\0';
    return strcmp(extension, "ppm") == 0; 
}

void getColor (char **buf, char *color) 
{
    *buf = malloc(3 * sizeof(char));

    if (*buf == NULL)return;
    
    if (strcmp(color, "red") == 0)
    {
        (*buf)[0] = 255;
        (*buf)[1] = 0;
        (*buf)[2] = 0;
    }
    else if (strcmp(color, "green") == 0)
    {
        (*buf)[0] = 0;
        (*buf)[1] = 255;
        (*buf)[2] = 0;
    }
    else if (strcmp(color, "blue") == 0)
    {
        (*buf)[0] = 0;
        (*buf)[1] = 0;
        (*buf)[2] = 255;
    }
    else if (strcmp(color, "yellow") == 0)
    {
        (*buf)[0] = 255;
        (*buf)[1] = 255;
        (*buf)[2] = 0;
    }
    else if (strcmp(color, "orange") == 0)
    {
        (*buf)[0] = 255;
        (*buf)[1] = 140;
        (*buf)[2] = 0;
    }
    else if (strcmp(color, "cyan") == 0)
    {
        (*buf)[0] = 0;
        (*buf)[1] = 255;
        (*buf)[2] = 255;
    }
    else if (strcmp(color, "magenta") == 0)
    {
        (*buf)[0] = 255;
        (*buf)[1] = 0;
        (*buf)[2] = 255;
    }
    else if (strcmp(color, "ocean") == 0)
    {
        (*buf)[0] = 0;
        (*buf)[1] = 119;
        (*buf)[2] = 190;
    }
    else if (strcmp(color, "violet") == 0)
    {
        (*buf)[0] = 148;
        (*buf)[1] = 0;
        (*buf)[2] = 211; 
    }
    else
    {
        (*buf)[0] = 0;
        (*buf)[1] = 0;
        (*buf)[2] = 0; 
    }
    
}

void createFileContent (int *fdWrite, int picX, int picY, char *buf, char *buf1, char *buf2, char *buf3, char *buf4) 
{   
    const char *scale = "255";
    const int buffSize = 3;
    int totalPixel = picX * picY;
    int halfPicX = picX/2, halfPicY = picY/2, oneFourthPicY = picY * (1.0/4.0), threeFourthPicY = picY * (3.0/4.0);

    int indexMiddle = 0, indexMiddleCount = 0, indexMiddleActivation = 0;

    int indexX = 0, indexY = 0;


    for (int i = 0; i < totalPixel; i++)
    {
        if(indexX <= halfPicX - indexMiddle)
        {
            if (indexY <= halfPicY)
            {
                write(*fdWrite, buf, buffSize);
            }
            else 
            {
                write(*fdWrite, buf1, buffSize);
            }
        }
        else if (indexX > halfPicX + indexMiddle)
        {
            if (indexY <= halfPicY)
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


        if(indexY >= oneFourthPicY && indexY <= halfPicY)
        {
            indexMiddleActivation = 1;
        }
        else if (indexY > halfPicY && indexY <= threeFourthPicY)
        {
            indexMiddleActivation = 2;
        }
        else 
        {
            indexMiddleActivation = 0;
        }
    }
}

void drawImage (int *fdWrite, char *picX, char *picY, char *buf, char *buf1, char *buf2, char *buf3, char *buf4)
{
    writeHeader (fdWrite, picX, picY); 
    createFileContent (fdWrite, atoi(picX), atoi(picY), buf, buf1, buf2, buf3, buf4);
}