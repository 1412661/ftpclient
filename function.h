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


int establishFTP(struct FTP* ftp);

int authFTP(char* user, char* pass);

int closeFTP(struct Computer c1, struct Computer c2);



struct Data sendCmd(struct Computer client, struct Computer server, int mode, struct Data data);
struct Data sendData(struct Computer c1, struct Computer c2, int mode, struct Data data);
int getPortListen(struct Socket* s);

#endif // _FUNCTION_H_
