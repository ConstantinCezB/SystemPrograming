#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include<sys/types.h>
#include<signal.h>

#define BUFFSIZE 255

int main(int argc, char **argv)
{
    char serverFIFO[] = "/tmp/serverFIFO";
    char clientFIFO[] = "/tmp/clientFIFO";
    char message[BUFFSIZE];

    // connecting to a server
    printf("Connecting...\n");
    int fd_server = open(serverFIFO, O_WRONLY);
    int fd_client = open(clientFIFO, O_RDONLY);

    if (fd_server < 0 || fd_client < 0)
    {
        perror("error with open");
        exit(EXIT_FAILURE);
    }

    printf("\n\tmy messages (client)\t\t\t\t\treceived messages (server)\n");
    printf("-------------------------------------------------------------------------------------------------\n");

    // Creating a child so that the reading and writting in 
    // the communication can be done at the same time.
    int child;
    if((child = fork()) == 0)
    {
        while(1)
        {
            // Reading the user input and sending it to the client
            int sizeInput = read(STDIN_FILENO, message, BUFFSIZE);
            message[sizeInput] = '\0'; // This prevents new messages to overlap on new one if shorter
            write(fd_server, message, strlen(message) + 1);
        }
    }
    else
    {
        while(1)
        {
            // Reading the message from the client and displaying it
            if (read(fd_client, message, BUFFSIZE) <= 0)
            {
                kill(child, SIGKILL);
                break;
            }
            printf("\t\t\t\t\t\t\t\t%s", message);
        }           
    }

    // Closing the files
    close(fd_server);
    close(fd_client);

    return 0;
}