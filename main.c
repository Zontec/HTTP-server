#include "src/include/includes.h"
#include "config.h"
#include "http.h"

extern int32_t connection_pointer;
static pthread_t 
        connections[MAX_NUMBER_OF_CONNECTIONS];
extern bool connections_alive_state[MAX_NUMBER_OF_CONNECTIONS];


static void CDECL cleanup(void) __attribute__ ((destructor));

static void CDECL init(void) __attribute__ ((constructor));

void CDECL *connection_thread_handle(void *arg)
{
    while(true)
    {

    }
}

int pos(const char *str)
{
    int len = strlen(str);
    for(int i = 0; i < len - 3; i++)
    {
        if(str[i] == 'G' && str[i + 1] == 'E' && str[i + 2] == 'T')
            return i + 4;
    }
    return -1;
}

int CDECL main(int argc, char **argv)
{
    

    int listening_socket_descriptior;
    int new_connected_descriptior;

    struct sockaddr_in host_adress, client_adress;

    int optimal_level = 1;

    if((listening_socket_descriptior = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error in creating socket!");
        return -1;
    }

    if(setsockopt(listening_socket_descriptior, 
        SOL_SOCKET, SO_REUSEADDR, 
        &optimal_level, sizeof(optimal_level)) == -1)
        {
            perror("Error in setsockopt!");
            return -1;
        }

    host_adress.sin_family = AF_INET;
    host_adress.sin_port = htons(SERVER_PORT);
    host_adress.sin_addr.s_addr = 0;

    memset(&(host_adress.sin_zero), '\0', 8);

    if(bind(listening_socket_descriptior, 
            (struct  sockaddr *)&host_adress, 
            sizeof(struct sockaddr)) == -1)

    {   
        perror("Error in binding socket!");
        return -1;
    }
    
    if(listen(listening_socket_descriptior, 5) == -1)
    {
        perror("Error in listening!");
        return -1;
    }
    printf("Server is ready!\n");


    const char *html_header = "\r\nHTTP/1.1 200 OK\r\nServer: myown\r\n\r\n";

    while(true)
    {
        socklen_t size = sizeof(struct sockaddr_in);
        new_connected_descriptior = accept(listening_socket_descriptior, 
                            (struct sockaddr *)&client_adress, &size);
        char recive_buf[MAX_RECIVE_PACKET_LEN];
        int recived_len = recv(new_connected_descriptior, recive_buf, sizeof(recive_buf), 0);
        
        int posi = pos(recive_buf);

        char data[MAX_SEND_PACKET_LEN] = "\0";
        char buf[MAX_SEND_PACKET_LEN] = "\0";

        if(recive_buf[posi] == '/' && recive_buf[posi + 1] == ' ')
        {

            int fd = open("site/html/index.html", O_RDONLY);

            read(fd, buf, sizeof(buf));

            sprintf(data, "%s%s\r\n", html_header, buf);

            send(new_connected_descriptior, data, strlen(data), 0);

        }
        else
        {

            struct http_frame frame;
            http_init_frame(&frame);
            http_parse(&frame, recive_buf, strlen(recive_buf));
           

            char path[255] = "\0";
            sprintf(path, "site/%s", frame.path_request);
            
            FILE *fd = fopen(path, "r+b");
            if(fd == NULL)
                continue;
            fseek(fd, 0L, SEEK_END);
            int file_size = ftell(fd);
            fseek(fd, 0L, SEEK_SET);

            fread(buf, sizeof(char), file_size, fd);
            
            sprintf(data, "%s%s\r\n", html_header, buf);
            memcpy(data, html_header, strlen(html_header));
            memcpy(data + strlen(html_header), buf, file_size);

            send(new_connected_descriptior, data, file_size + strlen(html_header), 0);
            
            fclose(fd);
            http_free_frame(&frame);
        }
        
        close(new_connected_descriptior);
    }
    return 0;
}


void cleanup(void)
{
    LOG("Destructor called!\n");
}

void init(void)
{
    LOG("Constructor called!\n");
}