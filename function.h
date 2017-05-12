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


/**
 * Connect to FTP server and return hello message
 * @param FTP infomation
 * @return 0 if success, 1 if fail.
 */
int ftp_connect(struct FTPClient* ftp);


/**
 * FTP Authentication
 * @param FTP infomation
 * @return 0 if success, 1 if fail.
 */
int ftp_auth(struct FTPClient* ftp);


/**
 * FTP send & receive command/data
 * @param Socket descriptor
 * @param command to be sent
 * @param length of command
 * @return data returned by server
 */
 // Note: ftp_comm() can handle both listening socket & non-listening socket
 // Note: ftp_comm() doesn't check for invalid socket
char* ftp_comm(int sockfd, char* data, int len);


/**
 * Switch FTP mode between active and passive
 * @param FTP infomation
 * @param Mode to be switched
 * @return 0 if success, 1 if fail.
 */
int ftp_mode(struct FTPClient* ftp, int mode);


char* upcase(char* str);

int ftp_loop(struct FTPClient *ftp);
//struct Data ftp_data(struct Computer c1, struct Computer c2, int mode, struct Data data);

#endif // _FUNCTION_H_
