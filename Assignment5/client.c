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

#define BUFFSIZE 255
#define HTONS 8888

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("You did not enter the address to connect");
    }

    char ans[BUFFSIZE], serverAns[BUFFSIZE];
    struct sockaddr_in server;
    int main_sock, serverAnsSize;

    // Creating the socket and setting it up.
    main_sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(HTONS);

    // Connecting to the server.
    if (connect(main_sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        bzero(ans, BUFFSIZE);
        bzero(serverAns, BUFFSIZE);

        printf("enter shell command: ");
        scanf("%[^\n]%*c", ans);

        // Sending the command to the server.
        if (write(main_sock, ans, sizeof(ans)) < 0)
        {
            perror("error with write");
            exit(EXIT_FAILURE);
        }

        // Receiving the ans from the server.
        if ((serverAnsSize = recv(main_sock, serverAns, 255, 0)) < 0)
        {
            perror("error with recv");
            exit(EXIT_FAILURE);
        }

        // If the server is terminating then the child will also get killed.
        if (serverAnsSize == 0)
            kill(getpid(), SIGKILL);

        // Displaying the answer to client.
        serverAns[serverAnsSize] = '\0';
        printf("%s\n", serverAns);
    }

    // Terminating 
    close(main_sock);
    return 0;
}
