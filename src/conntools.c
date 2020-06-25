#include "includes.h"
#include "conntools.h"

#define NO_CONNECTION       0

#define ALIVE               1
#define DEAD                0

extern int32_t connection_pointer;
extern bool connections_alive_state[MAX_NUMBER_OF_CONNECTIONS];

void  __clean(pthread_t *connections, const size_t size)
{

}

void  __fragmentate(pthread_t *connections, const size_t size)
{

}

bool  is_connection_alive(pthread_t connection)
{
    //if connection closed - kill returns not 0
    return pthread_kill(connection, 0) == 0 ? true : false;
}

void  close_connection_hard(pthread_t connection)
{
    if(is_connection_alive(connection))
        pthread_cancel(connection);
}

void  clear_connections(pthread_t *connections, const size_t size)
{
    connection_pointer = 0;
    for(int i = 0; i < size; i++)
    {
        pthread_t *tmp_connection = &connections[i];
        if(*tmp_connection != NO_CONNECTION && 
            connections_alive_state[i] == ALIVE)
            (void)close_connection_hard(*tmp_connection);
        
        *tmp_connection = NO_CONNECTION;
        connections[i] = DEAD;
    }
}

int32_t  get_next_empty_connection_place(pthread_t *connections, const size_t size)
{
    int tries = 0;
    int tmp_connection_pointer = connection_pointer;
    while(tries++ < size)
    {
        if(connections[tmp_connection_pointer++] == NO_CONNECTION)
            return tmp_connection_pointer - 1;
    }
}

bool  add_connection(const int connection_descriptor, 
                        pthread_t *connections, const size_t size, 
                        callback func, callback_args args)
{
    if(connection_pointer = get_next_empty_connection_place(connections, size) < 0)
    {
        perror("Connection limit reached!");
        return false;
    } 
    
    int res = pthread_create(connections[connection_pointer], 
                NULL, func, args);
    if(res > 0)
    {
        connections_alive_state[connection_pointer] = ALIVE;
        return true;
    }
    else
    {
        perror("Error creating new thread");
        return false;
    }
}