/******************************************************************************/
/* STATIC HTTP WEBSERVER                                                      */
/* file created and modified: 2024                                            */
/******************************************************************************/

#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"
#include "utils.h"

#define PORT 80

/******************************************************************************/
/*                              DEFINES                                       */
/******************************************************************************/

/******************************************************************************/
/*                              TYPEDEFS                                      */
/******************************************************************************/

/******************************************************************************/
/*                              LOCAL VARS                                    */
/******************************************************************************/

/******************************************************************************/
/*                              LOCAL FUNCS                                   */
/******************************************************************************/

/******************************************************************************/
/*                              GLOBAL VARS                                   */
/******************************************************************************/

/******************************************************************************/
/*                              GLOBAL FUNCS                                  */
/******************************************************************************/

int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;

    // create a server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // config socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    printf("http://localhost:%d\n", PORT);

    while(1)
    {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int* client_fd = malloc(sizeof(int));

        if (*(client_fd) = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void*)client_fd);
        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}