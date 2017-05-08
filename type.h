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


#endif // _TYPE_H_

