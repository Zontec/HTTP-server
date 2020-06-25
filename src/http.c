#include "http.h"

#define MAX_BUF_SIZE        (1<<10)

const char *__req_array[4] = {
    "POST",
    "GET",
    "HEAD",
    "PUT"
};

void http_init_frame(struct http_frame *http)
{
    memset(http, 0, sizeof(struct http_frame));
}

void http_free_frame(struct http_frame *http)
{
    if(http->args)
        free(http->args);
    if(http->content)
        free(http->content);
    if(http->path_request)
        free(http->path_request);
    http_init_frame(http);
}


enum http_header define_http_header(char *str)
{
    if(!strcmp(str, "POST"))
        return POST;
    if(!strcmp(str, "GET"))
        return GET;
    if(!strcmp(str, "PUT"))
        return PUT;
    if(!strcmp(str, "HEAD"))
        return HEAD;
    return NOTDEFINED_HEADER;
}


/*

GET /hello.htm HTTP/1.1
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
Host: www.tutorialspoint.com
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive

*/


void http_parse(struct http_frame *http, const char *data, size_t size)
{
    char tmp_data[MAX_BUF_SIZE] = "\0";
    char *cur_data_pointer = tmp_data;
    const char delim[]=" ";

    http->arrival_time = clock();
    memcpy(tmp_data, data, size);
    
    strtok(cur_data_pointer, delim);
    http->header = define_http_header(cur_data_pointer);
    cur_data_pointer += strlen(tmp_data) + 1;
    strtok(cur_data_pointer, delim);
    
    int tmp_len = strlen(cur_data_pointer) + 1;
    http->path_request = malloc(tmp_len);
    memset(http->path_request, 0, tmp_len);
    memcpy(http->path_request, cur_data_pointer, strlen(cur_data_pointer)); 

    cur_data_pointer += tmp_len;
    
    if(strstr(cur_data_pointer, "HTTP/1.1") != NULL){
        http->vhttp = HTTP1_1;
    }
        if(strstr(cur_data_pointer, "HTTP/1.0") != NULL){
        http->vhttp = HTTP1_0;
    }
    
}