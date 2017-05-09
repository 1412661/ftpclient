#ifndef _TYPE_H_
#define _TYPE_H_


struct Socket
{
    int sockfd;
    int port;
};

struct Computer
{
    struct Socket cmd;
    struct Socket data;
};

struct Data
{
	char* buff;
	int length;
};

struct FTP
{
    char* user;
    char* pass;
    char* host;

    struct Computer client;
    struct Computer server;
    int mode;
};

#endif // _TYPE_H_

