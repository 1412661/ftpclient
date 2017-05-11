#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //for exit(0);
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <arpa/inet.h>


#include "const.h"
#include "type.h"
#include "function.h"
#include "net.h"

/**
 * Clone a memory space
 * @param Pointer point to the memory space that need clone
 * @param Size of memory space that need clone
 * @param Padding space. Usually used for string termination character.
 * @return Pointer point to new memory space cloned from the input.
 */
char* clone(char* buffer, unsigned int size, unsigned int padding)
{
    /**
    * Example use:
    * char str1[] = "abc";
    * char* str2 = clone(str1, 3, 1);
    * str2[3] = '\0';
    * Sometime, str1 is in raw format without string termination character '\0'
    */
    return memcpy((char*)malloc(sizeof(char)*(size+padding)), buffer, size);
}

/**
 * Connect to FTP server and return hello message
 * @param FTP infomation
 * @return 0 if success, 1 if fail.
 */
int ftp_connect(struct FTPClient* ftp)
{
    if (portConnect(&(ftp->cmd.sockfd), &(ftp->cmd.port), ftp->host, FTP_CMD_PORT))
	{
		printf("[ERROR] Could not connect to FTP service at %s\n", ftp->host);
		return 1;
	}

	char *hello = ftp_cmd(ftp, NULL, 0);
    printf("%s", hello);
    free(hello);

	return 0;
}

/**
 * FTP Authentication
 * @param FTP infomation
 * @return 0 if success, 1 if fail.
 */
int ftp_auth(struct FTPClient* ftp)
{
    ftp->user = (char*)malloc(BUFFSIZE_VAR);
    ftp->pass = (char*)malloc(BUFFSIZE_VAR);
    char* response;
    char cmd[BUFFSIZE_VAR];

    printf("Username: ");
    fgets(ftp->user, BUFFSIZE_VAR, stdin);
	sprintf(cmd, "USER %s\r\n", ftp->user);
	response = ftp_cmd(ftp, cmd, strlen(cmd));
    printf("%s", response);		// Don't care about return code. Always 331 :3

    strcpy(ftp->pass, getpass("Password: "));
	sprintf(cmd, "PASS %s\r\n", ftp->pass);
	response = ftp_cmd(ftp, cmd, strlen(cmd));
    printf("%s", response);

    // return code
    int rc;
    sscanf(response, "%d", &rc);
    if (rc != 230)			// FTP command 230 indicate
        return 1;
	else return 0;
}

/**
 * Switch FTP mode between active and passive
 * @param FTP infomation
 * @param Mode to be switched
 * @return 0 if success, 1 if fail.
 */
int ftp_mode(struct FTPClient* ftp, int mode)
{
    // Default FTP mode is active
    if (ftp->mode == 0)
        ftp->mode = FTP_ACTIVE;

	// Exception of this function

    if (ftp->data.sockfd && ftp->mode == mode)
		return 0;

    switch (mode)
    {
		case FTP_ACTIVE:
            if (ftp->mode == FTP_PASSIVE)
			{

			}
			break;
		case FTP_PASSIVE:
			if (ftp->mode == FTP_PASSIVE)
			{

			}
			break;
		default:
			printf("[ERROR] ftp_mode(): mode %d not valid\n", mode);
			return 1;
    }
}

/**
 * FTP send & receive command
 * @param FTP infomation
 * @param command to be sent, can be NULL
 * @param length of command
 * @return data returned by server
 */
char* ftp_cmd(struct FTPClient* ftp, char* data, int len)
{
    if (data != NULL)
	{
		len = write(ftp->cmd.sockfd, data, len);
		if (len < 0)
		{
            printf("[ERROR] write(): Could not write data to socket\n");
            return NULL;
		}
	}

	char* response = (char*)malloc(BUFFSIZE_VAR);
    memset(response, 0, BUFFSIZE_VAR);
    read(ftp->cmd.sockfd, response, BUFFSIZE_VAR);

	return response;
}
