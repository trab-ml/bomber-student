#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdlib.h>
#include <errno.h>

// Define an enum for error codes
typedef enum
{
    INVALID_ARGUMENT_ERROR, // Error code for invalid arguments
    MALLOC_ERROR,               
    REALLOC_ERROR,
    THREAD_CREATION_ERROR,
    THREADPOOL_CREATION_ERROR,
    MUTEX_ERROR,
    INIT_GET_RESPONSE_ERROR,                 
    ADD_MAP_ERROR,
    CJSON_CREATE_OBJECT_ERROR,                 
    SOCKET_CREATION_ERROR,         
    SETSOCKOPT_ERROR,               
    BIND_ERROR,                    
    LISTEN_ERROR,                 
    ACCEPT_ERROR,                
    SOCKET_RECV_ERROR,       
    SOCKET_SEND_ERROR,
    SOCKET_CLOSE_ERROR,              
    CLIENT_LIST_ADD_ERROR,
    GET_QUERY_ERROR,               
} ErrorCode;

void handleError(ErrorCode error);
const char *getErrorMessage(ErrorCode error);

#endif // ERROR_HANDLER_H