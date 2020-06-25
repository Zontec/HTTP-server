#ifndef __CONFIG_H__
#define __CONFIG_H__

//#define DEBUG

#define SERVER_PORT     3000

#define MAX_NUMBER_OF_CONNECTIONS 100

#define MAX_RECIVE_PACKET_LEN       (1 << 12)
#define MAX_SEND_PACKET_LEN         (1 << 20)

#define MAX_BUF_SIZE                (1 << 12)

#ifndef __GNUC__
    #error Only gcc is supported up to now! Please, use gcc to compile!
#endif

#ifndef __GNUC__
    #define CDECL  __attribute__((cdecl))
#else
    #define CDECL
#endif 


#ifndef DEBUG
    #define LOG(str)
    #define xLOG(str,arg)
#else   
    #define LOG(str)        printf(str)
    #define xLOG(str,arg)   printf(str, arg)
#endif


typedef void CDECL (**callback)(void*);
typedef void* callback_args;

#endif