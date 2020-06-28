#ifndef __CONNTOOLS_H__
#define __CONNTOOLS_H__

#include "includes.h"
#include "config.h"

bool CDECL conntools_add_connection(const int connection_descriptor, 
                        pthread_t *connections, const size_t size, 
                        callback func, callback_args args) __attribute__((__nonnull));

bool CDECL conntools_is_connection_alive(pthread_t connection);

void CDECL conntools_close_connection_hard(pthread_t connection);

void CDECL conntools_clear_connections(pthread_t *connections,
                             const size_t size) __attribute__((__nonnull));



#endif