#ifndef _CONST_H_
#define _CONST_H_

#include "type.h"

#define BUFFSIZE_VAR	1024
#define BUFFSIZE_DATA	1024*1024

#define FTP_CMD_PORT	21


#define FTP_ACTIVE		1
#define FTP_PASSIVE		2

enum SOCK_STATUS
{
    SOCK_LISTEN,
    SOCK_NON_LISTEN,
    SOCK_FREE
};

#endif // _CONST_H_
