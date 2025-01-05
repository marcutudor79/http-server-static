/******************************************************************************/
/* STATIC HTTP WEBSERVER                                                      */
/* file created and modified: 2024                                            */
/******************************************************************************/

#include "stdlib.h"
#include "sys/socket.h"
#include "sys/stat.h"
#include "unistd.h"
#include "regex.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"

#include "./lib/file_helper.h"
#include "./lib/http_helper.h"

/******************************************************************************/
/*                              DEFINES                                       */
/******************************************************************************/

/* Max REQUEST buffer size 10 KB */
#define REQUEST_SIZE (10240U)

/* Max REPLY buffer response size 1 MB */
#define REPLY_SIZE (1048576U)

/* Folder for html and other server related files */
#define SRC_DIR "/home/bl4ck/_code/http-server-static/software/src/"

/* Max length of the request */
#define MAX_LEN_REQUEST (100U)

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


void* handle_client(void* arg)
{
    int client_fd       = *((int*)arg);
    char* buffer        = (char*)malloc(REQUEST_SIZE);
    uint32_t bytes_received = 0U;
    char* buffer_tokens = NULL;
    char* response      = (char*)malloc(REPLY_SIZE);
    uint32_t response_len   = 0U;
    FILE *file          = NULL;

    // HTTP content
    char* method    = NULL;
    char* file_path = NULL;

    // receive the bytes
    bytes_received = recv(client_fd, buffer, REQUEST_SIZE, 0);

    // if a request was received
    if (bytes_received > 0U)
    {
        puts(buffer);

        // extract the method
        buffer_tokens = strtok(buffer, " ");
        method = buffer_tokens;

        // check if the method is valid
        if (method == NULL)
        {
            goto exit;
        }

        // if the method is GET
        if (strcmp(method, "GET") == 0)
        {
            puts(method);

            buffer_tokens = strtok(NULL, " ");

            // extract the file path
            file_path = buffer_tokens;
            puts(file_path);

            // if the file path is "/" -> reply with index.html
            if (strcmp(file_path, "/") == 0)
            {
                file_path = (char*)malloc(REQUEST_SIZE);
                strcat(file_path, SRC_DIR);
                strcat(file_path, "index.html");

                puts(file_path);
            }

            // send the requested file
            file = fopen(file_path, "r");
            if (file == NULL)
            {
                snprintf(response, 100U,
                         "HTTP/1.1 404 Not Found\r\n"
                         "Content-Type: text/plain\r\n"
                         "\r\n"
                         "404 Not Found\n");

                response_len = strlen(response);
            }
            else
            {
                // Store header in response
                snprintf(response, 100U,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n");

                // Read from file and store in response
                response_len = strlen(response);

                char* file_content = (char*)malloc(REPLY_SIZE);
                size_t bytes_read = fread(file_content, 1, REPLY_SIZE, file);
                memcpy(response + response_len, file_content, bytes_read);
                response_len += bytes_read;

                // free the memory and close the file
                free(file_content);
                fclose(file);
            }

            // send the response
            send(client_fd, response, response_len, 0);
        }
    }


exit:
    close(client_fd);
    free(arg);
    free(buffer);
    free(response);
    free(file_path);
    return NULL;
}
