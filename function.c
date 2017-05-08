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


char* ask(char* server_ip, int server_port, char* msg, int* byte)
{
    int n;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("[ERROR] socket(): Could not open socket.");
        return NULL;
    }

    struct hostent *server = gethostbyname(server_ip);
    if (server == NULL)
    {
        printf("[ERROR] gethostbyname(): Invalid IP address.");
        return NULL;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    memmove((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(server_port);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("[ERROR] connect(): Could not connect to server %s:%d\n", server_ip, server_port);
        return NULL;
    }

    struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(sockfd, (struct sockaddr*)&sin, &len) == -1)
		perror("getsockname");
	else
		printf("[Client] Using port number %d\n", ntohs(sin.sin_port));


    n = write(sockfd, msg, strlen(msg));
    if (n < 0)
    {
        printf("[ERROR] write(): Could not write data to socket");
        return NULL;
    }

    char* data = (char*)malloc(BUFFSIZE_DATA);
    memset(data, 0, BUFFSIZE_DATA);
    n = read(sockfd, data, BUFFSIZE_DATA);

	shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    return data;
}
