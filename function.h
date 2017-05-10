#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "type.h"

/**
 * Clone a memory space
 * @param Pointer point to the memory space that need clone
 * @param Size of memory space that need clone
 * @param Padding space. Usually used for string termination character.
 * @return Pointer point to new memory space cloned from the input.
 */
char* clone(char* buffer, unsigned int size, unsigned int padding);


// Decrapted
char* ask(char* server_ip, int server_port, char* msg, int* byte);


int ftp_connect(struct FTPClient* ftp);
int ftp_auth(struct FTPClient* ftp);
char* ftp_cmd(struct FTPClient* ftp, char* data, int len);

int portConnect(struct Socket* s, char* host, int port);
int portListen(struct Socket* s);

/*
int authFTP(char* user, char* pass);

int closeFTP(struct Computer c1, struct Computer c2);



struct Data ftp_cmd(struct Computer client, struct Computer server, int mode, struct Data data);
struct Data ftp_data(struct Computer c1, struct Computer c2, int mode, struct Data data);
int getPortListen(struct Socket* s);*/

#endif // _FUNCTION_H_
