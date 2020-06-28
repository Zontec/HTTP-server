#include "includes.h"
#include "conntools.h"
#include <signal.h>
#define NO_CONNECTION       0

#define ALIVE               1
#define DEAD                0

 int32_t connection_pointer;
 bool connections_alive_state[MAX_NUMBER_OF_CONNECTIONS];

void  __clean(pthread_t *connections, size_t *size)
{
    free(connections);
    *size = 0;
}

void  __fragmentate(pthread_t *connections, const size_t size)
{
    for(int i = 0; i < size; i++)
    {
        if(!conntools_is_connection_alive(connections[i]))
        {
            int pointer = i;
            if(i == 0)
                connections[i] = DEAD;
            else
                while(pointer > 0 && connections[pointer - 1] == DEAD)
                {
                    connections[pointer - 1] = connections[pointer];
                    connections[pointer--] = DEAD;
                }   
        }
    }
}

bool conntools_is_connection_alive(pthread_t connection)
{
    //if connection closed - kill returns not 0
    return (pthread_kill(connection, 0) == 0 ? true : false);
}

void  conntools_close_connection_hard(pthread_t connection)
{
    if(conntools_is_connection_alive(connection))
        pthread_cancel(connection);
}

void  conntools_clear_connections(pthread_t *connections,
                             const size_t size)
{
    connection_pointer = 0;
    for(int i = 0; i < size; i++)
    {
        pthread_t *tmp_connection = &connections[i];
        if(*tmp_connection != NO_CONNECTION && 
            connections_alive_state[i] == ALIVE)
            pthread_kill(*tmp_connection, SIGKILL);
        
        *tmp_connection = NO_CONNECTION;
        connections[i] = DEAD;
    }
}

int32_t  __conntools_get_next_empty_connection_place(pthread_t *connections,
                                                 const size_t size)
{
    int tries = 0;
    int tmp_connection_pointer = connection_pointer;
    while(tries++ < size)
    {
        if(connections[tmp_connection_pointer++] == NO_CONNECTION)
            return tmp_connection_pointer - 1;
    }
}

pthread_t *__get_next_empty_connection_place(pthread_t *connections, const size_t size) 
{

    for(int i = 0; i < size; i++)
    {
        if(connections[i] == DEAD)
        {
            connection_pointer = i;
            return &connections[i];
        }
    }
    __fragmentate(connections, size);
    return NULL;
}

bool  conntools_add_connection(const int connection_descriptor, 
                        pthread_t *connections, const size_t size, 
                        callback func, callback_args args)
{
    if(connection_pointer = __get_next_empty_connection_place(connections, size) == NULL)
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