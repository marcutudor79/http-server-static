/******************************************************************************/
/* STATIC HTTP WEBSERVER                                                      */
/* file created and modified: 2024                                            */
/******************************************************************************/

#include "fcntl.h"
#include "string.h"

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

/*
    Function used to get the file descriptor based on the dir path + file_name
    @param [IN] char* - directory: the directory where the file is located
    @param [IN] char* - file_name: the name of the file
    @param [OUT] int - the file descriptor
*/
int get_file_descriptor(const char *directory, const char *file_name)
{
    char src_file[100];

    strcpy(src_file, directory);
    strcat(src_file, file_name);

    int fd = open(src_file, O_RDONLY);

    return fd;
}

/*
    Function used to get the file extension for MIME encoding
    @param [IN] char* - file_name: the name of the file
    @param [OUT] const char* - the file extension

*/
const char* get_file_extension(const char *file_name)
{
    // search for the last dot in the file name
    const char *dot = strrchr(file_name, '.');

    // if there is no dot in the file name, return an empty string
    if(!dot || dot == file_name) return "";

    return dot + 1;
}

