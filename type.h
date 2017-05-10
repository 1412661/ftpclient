#ifndef _TYPE_H_
#define _TYPE_H_


struct Socket
{
    int sockfd;
    int port;
};

struct Data
{
	char* buff;
	int length;
};

struct FTPClient
{
    char* user;
    char* pass;
    char* host;

    struct Socket data;
    struct Socket cmd;
    int mode;
};

#endif // _TYPE_H_

