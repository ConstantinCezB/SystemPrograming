/* Cezar Begu
 * 104808191
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG 1
#define BUFFSIZE 255
#define HTONS 8888

int main(int argc, char *argv[])
{
    char buff[BUFFSIZE];
    struct sockaddr_in server, client;
    int addrSize, ansSize, main_sock, client_sock;

    // 1) Creating the socket
    main_sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2) creating the sock addr
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(HTONS);

    // 3) Binding the socket and the sock addr
    if (bind(main_sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("error with bind");
        return -1;
    }

    // 4) Listening to the sock
    listen(main_sock, BACKLOG);

    while (1)
    {
        printf("Waiting for connection...\n");
        addrSize = sizeof(struct sockaddr_in);

        // 5) Accepting the client
        client_sock = accept(main_sock, (struct sockaddr *)&client, (socklen_t *)&addrSize);

        if (client_sock < 0)
        {
            perror("Connection failed");
            return -1;
        }

        // 6) reading to client
        while ((ansSize = read(client_sock, buff, BUFFSIZE)) > 0)
        {
            buff[ansSize] = '\0';

            printf("received command: %s\n", buff);

            // 6) writting to client
            if (fork() == 0)
            {
                // redirecting the output
                dup2(client_sock, STDOUT_FILENO);
                execlp(buff, buff, (char *)0);
                // note: if the process is killed the empty line will be send terminating the child.
            }
        }
    }

    // Terminating 
    close(main_sock);
    close(client_sock);
    return 0;
}
