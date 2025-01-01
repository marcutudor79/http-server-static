/******************************************************************************/
/* STATIC HTTP WEBSERVER                                                      */
/* file created and modified: 2024                                            */
/******************************************************************************/

#include "stdlib.h"
#include "sys/socket.h"
#include "sys/stat.h"
#include "unistd.h"
#include "regex.h"
#include "string.h"
#include "stdio.h"

#include "./lib/file_helper.h"
#include "./lib/http_helper.h"

#define BUFFER_SIZE 104857600
#define SRC_DIR "./src/  "

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