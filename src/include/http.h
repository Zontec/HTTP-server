#ifndef __HTTP_H__
#define __HTTP_H__

#include "includes.h"

typedef unsigned char u_char;

enum http_header
{
    POST,
    GET,
    HEAD,
    PUT,

    NOTDEFINED_HEADER
};

enum http_version
{
    HTTP1_0,
    HTTP1_1
};



struct http_frame
{
    enum http_header     header;
    uint64_t        arrival_time;
    u_char          *path_request;
    u_char          *content;
    size_t          content_size;
    u_char          *args;
    enum http_version    vhttp;
};


void __attribute__((cdecl)) http_free_frame(struct http_frame *http) 
                __attribute__((__nonnull));

void __attribute__((cdecl)) http_init_frame(struct http_frame *http) 
                __attribute__((__nonnull));

void __attribute__((cdecl)) http_parse(struct http_frame *http,
                 const char *data, size_t size) __attribute__((__nonnull));



#endif