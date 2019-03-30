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

    // Unlinking the files so that mkfifo can work.
    unlink(serverFIFO);
    unlink(clientFIFO);

    // Creating the FIFO for client and server.
    if (mkfifo(serverFIFO, 0777) != 0 || mkfifo(clientFIFO, 0777) != 0)
    {
        perror("Error with mkfifo");
        exit(EXIT_FAILURE);
    }

    // Making sure that they have the access needed.
    chmod(serverFIFO, 0777);
    chmod(clientFIFO, 0777);

    while (1)
    {
        // connecting to a client
        printf("Waiting for connection...\n");
        int fd_server = open(serverFIFO, O_RDONLY);
        int fd_client = open(clientFIFO, O_WRONLY);
        printf("\n\tmy messages (server)\t\t\t\t\treceived messages (client)\n");
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
                write(fd_client, message, strlen(message) + 1);
            }
        }
        else
        {
            while(1)
            {
                // Reading the message from the client and displaying it
                if (read(fd_server, message, BUFFSIZE) <= 0)
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
    }

    return 0;
}