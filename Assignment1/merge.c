/*
A C program to merge two given binary-based ppm files (P6).
The names of the two given image files and the name of the combined image file are given as command line arguments.
The combined image is obtained by putting the second image to the top-right corner of the first image. 

Cezar Begu 104808191
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//This will print messages to the consol.
void print(char *message);
//If the character is a digit, space or a letter then it will return true.
bool isDigitOrChar(char c);
//If the character is a digit it will return true.
bool isDigit(char c);
//This will read the header of the picture and store the values in the address of the paramethers.
void readPictureInfo(int *fd, char picFormat[10], char picX[10], char picY[10], char picScale[10]);
//This will merge the two pictures.
void createMerge(int *fd1, int *fd2, int *fdWrite, char format[], char pic1X[10], char pic1Y[10], char pic1Scale[10], char pic2X[10], char pic2Y[10]);

//Checks if the files provided are valid and also if the pictures correspond to the requirements. Else the program will exit with error.
int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        print("Provide the name of the two P6 images you want to merge. And also the name of the file to save.\n");
        return 1;
    }

    int fd1 = open(argv[1], O_RDONLY, 0644);
    int fd2 = open(argv[2], O_RDONLY, 0644);
    int fdWrite = open(argv[3], O_CREAT | O_WRONLY, 0644);

    if (fd1 == -1 || fd2 == -1 || fdWrite == -1)
    {
        print("Cannot create or open file\n");
        return 1;
    }

    char pic1Format[10], pic2Format[10], pic1X[10], pic1Y[10], pic1Scale[10], pic2X[10], pic2Y[10], pic2Scale[10];

    readPictureInfo(&fd1, pic1Format, pic1X, pic1Y, pic1Scale);
    readPictureInfo(&fd2, pic2Format, pic2X, pic2Y, pic2Scale);

    if (atoi(pic1X) < atoi(pic2X)
     || atoi(pic1Y) < atoi(pic2Y))
    {
        print("Picture 2 cannot be bigger then picture 1.\n");
        return 1;
    }

    if (strcmp(pic1Scale, pic2Scale) != 0)
    {
        print("The scales of the pictures are different.\n");
        return 1;
    }

    if(strcmp(pic2Format, "P6") != 0 && strcmp(pic2Format, "P6") != 0)
    {
        print("The pictures are not p6!\n");
        return 1;
    }

    createMerge(&fd1, &fd2, &fdWrite, pic1Format, pic1X, pic1Y, pic1Scale, pic2X, pic2Y);

    close(fd1);
    close(fd2);
    close(fdWrite);
    return 0;
}

void print(char *message)
{
    write(STDOUT_FILENO, message, strlen(message));
}

bool isDigitOrChar(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

void readPictureInfo(int *fd, char picFormat[10], char picX[10], char picY[10], char picScale[10])
{
    int stage = 0;
    char buf[1];
    int indexFormat = 0;
    bool reading = true, pixelReadingStagePart = true;

    while ((stage < 3) && (read(*fd, buf, 1) > 0))
    {
        if (buf[0] == '\n')
        {
            if (reading)
            {
                if (stage == 0)
                    picFormat[indexFormat] = '\0';
                else if (stage == 1)
                    picY[indexFormat] = '\0';
                else if (stage == 2)
                    picScale[indexFormat] = '\0';
                indexFormat = 0;
                stage++;
            }
            reading = true;
        }

        if (buf[0] == '#')
            reading = false;

        if (reading && (isDigitOrChar(buf[0])))
        {
            if (stage == 0)
                picFormat[indexFormat++] = buf[0];
            else if (stage == 1)
            {
                if (buf[0] == ' ')
                {
                    picX[indexFormat] = '\0';
                    pixelReadingStagePart = false;
                    indexFormat = 0;
                }

                if (isDigit(buf[0]))
                {
                    if (pixelReadingStagePart)
                        picX[indexFormat++] = buf[0];
                    else
                        picY[indexFormat++] = buf[0];
                }
            }
            else if (stage == 2 && isDigit(buf[0]))
                picScale[indexFormat++] = buf[0];
        }
    }
}

void createMerge(int *fd1, int *fd2, int *fdWrite, char format[], char pic1X[10], char pic1Y[10], char pic1Scale[10], char pic2X[10], char pic2Y[10])
{
    char specialChar[2] = {'\n', ' '};
    bool picChange = true;
    //converting a string to a int
    int pic1XInt = atoi(pic1X), pic1YInt = atoi(pic1Y), pic2XInt = atoi(pic2X), pic2YInt = atoi(pic2Y), pic1Write = pic1XInt - pic2XInt, scaleInt = atoi(pic1Scale);
    //this formula determines the buffer size
    int buffSize = ceil((float)ceil(log2(scaleInt))/8.0) * 3;
    char buf[buffSize];
    int index = 1;

    //writing the header of the file
    write(*fdWrite, format, strlen(format));
    write(*fdWrite, &specialChar[0], sizeof(char));
    write(*fdWrite, pic1X, strlen(pic1X));
    write(*fdWrite, &specialChar[1], sizeof(char));
    write(*fdWrite, pic1Y, strlen(pic1Y));
    write(*fdWrite, &specialChar[0], sizeof(char));
    write(*fdWrite, pic1Scale, strlen(pic1Scale));
    write(*fdWrite, &specialChar[0], sizeof(char));

    //writing the first image
    while (read(*fd1, buf, sizeof(buf)) > 0)
    {
        write(*fdWrite, buf, sizeof(buf));
    }

    //move the cursor at the top of the image
    lseek(*fdWrite, -pic1XInt * (pic1YInt) * sizeof(buf), SEEK_CUR);

    //writing the second image 
    while (read(*fd2, buf, sizeof(buf)) > 0)
    {
        if (index == 1)
        {
            lseek(*fdWrite, pic1Write * sizeof(buf), SEEK_CUR);
        }
        if (index == pic2XInt)
        {
            index = 1;
        }
        else
        {
            index++;
        }
        write(*fdWrite, buf, sizeof(buf));
    }
}