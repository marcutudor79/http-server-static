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

#include "./lib/file_helper.h"
#include "./lib/http_helper.h"

/******************************************************************************/
/*                              DEFINES                                       */
/******************************************************************************/

/* Max buffer size 100 MB */
#define BUFFER_SIZE 104857600

/* Folder for html and other server related files */
#define SRC_DIR "./src"

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

void build_http_response(const char* file_name, const char* file_ext, char* response, size_t* response_len)
{
    printf("Considered FILE NAME: %s\n", file_name);

    // build HTTP header
    const char* mime_type = get_mime_type(file_ext);
    char* header = (char*)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n
             Content-Type: %s\r\n
             \r\n",
             mime_type);

    // Try to open the file
    int file_fd = get_file_descriptor(SRC_DIR, file_name);
    if (file_fd == -1)
    {
        snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found\n");

        *response_len = strlen(response);
        return;
    }

    // Get the file size for Content-Length
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;
    printf("File descriptor: %d size %li\n", file_fd, file_size);

    // Copy header to response
    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len += strlen(header);

    // Copy file content to response
    ssize_t bytes_read = 0;
    while ((bytes_read = read(file_fd, response + *response_len, BUFFER_SIZE)) > 0)
    {
        *response_len += bytes_read;
    }

    free(header);
    close(file_fd);
}

void* handle_client(void* arg)
{
    int client_fd = *((int*)arg);
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));

    // receive a request data from the client and store into buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received < 0)
    {

        regex_t regex;
        regcomp(&regex, "GET /(.*) HTTP/1.1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex, buffer, 2, matches, 0) == 0)
        {
            buffer[matches[1].rm_eo] = '\0';
            const char* url_encoded_file_name = buffer + matches[1].rm_so;
            char* file_name = url_decode(url_encoded_file_name);

            // If the route is requested in GET, then reply with index
            if (strcmp(file_name, "") == 0)
            {
                strcpy(file_name, "index.html");
            }

            // Get the file extension
            char file_ext[32] = {0};
            strcpy(file_ext, get_file_extension(file_name));

            // Build the HTTP response
            char* response = (char*)malloc(BUFFER_SIZE *2* sizeof(char));
            size_t response_len = 0;
            build_http_response(file_name, file_ext, response, &response_len);

            // Send the response to the client
            send(client_fd, response, response_len, 0);

            free(response);
            free(file_name);
        }

        regfree(&regex);
    }

    close(client_fd);
    free(arg);
    free(buffer);
    return NULL;
}
