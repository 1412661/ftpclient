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


#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //for exit(0);
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h> //hostent
#include<arpa/inet.h>


#include "const.h"
#include "type.h"
#include "function.h"

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

int ftp_connect(struct FTPClient* ftp)
{
	//ftp->host = "abc.com";
    if (portConnect(&(ftp->cmd), ftp->host, FTP_CMD_PORT))
	{
		printf("[ERROR] Could not connect to FTP service at %s\n", ftp->host);
		return 1;
	}

	char *hello = ftp_cmd(ftp, NULL, 0);
    printf("%s", hello);
    free(hello);

	return 0;
}

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
    if (rc != 230)
        return 1;
	else return 0;
}

// http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
// This function is a modified version of the hostname_to_ip()
// Credit goes to me 1412661
int getaddrbyname(char *hostname)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    int rv;
    struct addrinfo *server;
    if ((rv = getaddrinfo(hostname , "http" , &hints , &server)) != 0)
    {
        printf("[ERROR] getaddrinfo(): %s\n", gai_strerror(rv));
        return 0;
    }

    struct sockaddr_in *h = (struct sockaddr_in*)server->ai_addr;
	int ip = h->sin_addr.s_addr;

    freeaddrinfo(server);

    return ip;
}


int portConnect(struct Socket* s, char* host, int port)
{
	if (s == NULL || host == NULL || port == 0)
	{
		printf("[WARN] portConnect(): NULL input\n");
		return 1;
	}

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("[ERROR] socket(): Could not open socket.");
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = getaddrbyname(host);
    if (serv_addr.sin_addr.s_addr == 0)
	{
        printf("[ERROR] getaddrbyname(): Could not resolve hostname in to IP address.\n");
        return 1;
	}

    printf("[INFO] Resolved: %s -> %s\n", host, inet_ntoa(serv_addr.sin_addr));

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("[ERROR] connect(): Could not connect to %s:%d\n", host, port);
        return 1;
    }

    struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(sockfd, (struct sockaddr*)&sin, &len) == -1)
	{
		printf("[ERROR] getsockname() failed.\n");
		return 1;
	}

    s->port = ntohs(sin.sin_port);
	s->sockfd = sockfd;

    return 0;
}

int portListen(struct Socket* s)
{
	if (s == NULL)
	{
		printf("[WARN] portListen(): NULL input\n");
		return 1;
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("[ERROR] socket(): Could not initialize socket");

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // When port is used by another program or not release by OS,
    // switch to another port (port--)
    while (1)
    {
        serv_addr.sin_port = htons(s->port);
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            printf("[ERROR] bind(): could not bind socket at port %d\n", s->port--);
        else
            break;
    }

    if (listen(sockfd, BUFFSIZE_DATA) != 0)
	{
		printf("[ERROR] listen(): Could not listen at port %d\n", s->port);
		return 1;
	}

	s->sockfd = sockfd;

	return 0;
}


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
};
