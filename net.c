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

#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //for exit(0);
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <arpa/inet.h>

#include "const.h"
#include "net.h"


// http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
// This function is a modified version of the hostname_to_ip()
// Credit goes to me 1412661
/**
 * Resolve IP address from hostname
 * @param Hostname
 * @return IP address in 32 bit unsigned integer format
 */
uint32_t getaddrbyname(char *hostname)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; 			// Use IPv4 address family
    hints.ai_socktype = SOCK_STREAM;	// Use TCP protocol

    int rv;
    struct addrinfo *server;
    if ((rv = getaddrinfo(hostname, "http", &hints, &server)) != 0)
    {
        printf("[ERROR] getaddrinfo(): %s\n", gai_strerror(rv));
        return 0;
    }

    struct sockaddr_in *h = (struct sockaddr_in*)server->ai_addr;
    uint32_t ip = h->sin_addr.s_addr;

    freeaddrinfo(server);

    return ip;
}

/**
 * Open a socket and connect to target host:port
 * @param Socket descriptor (will be modified if function return 0)
 * @param Source port (will be modified if function return 0)
 * @param Target host
 * @param Target port
 * @return 0 if success, 1 if fail.
 */
int portConnect(int* i_sockfd, int* i_port, char* host, int port)
{
    if (i_sockfd == NULL || host == NULL || port == 0)
    {
        printf("[WARN] portConnect(): NULL input\n");
        return 1;
    }

    // Create a socket (endpoint)
    // AF_INET stand for IPv4 address family
    // SOCK_STREAM for TCP protocol
    // Refer: https://linux.die.net/man/2/socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("[ERROR] socket(): Could not open socket.");
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;			// Use IPv4 address family
    serv_addr.sin_port = htons(port);		// Target port
    serv_addr.sin_addr.s_addr = getaddrbyname(host);	// Target IP
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

    // Get port that the os kernel allocate
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(sockfd, (struct sockaddr*)&sin, &len) == -1)
    {
        printf("[ERROR] getsockname() failed.\n");
        return 1;
    }

    *i_sockfd = sockfd;
    if (i_port != NULL)
        *i_port = ntohs(sin.sin_port);

    return 0;
}

/**
 * Open a socket to listen
 * @param Socket descriptor (will be modified if function return 0)
 * @param Port to listen (will be modified if function return 0)
 * @return 0 if success, 1 if fail.
 */
int portListen(int* i_sockfd, int* i_port)
{
    if (i_sockfd == NULL || i_port == NULL)
    {
        printf("[WARN] portListen(): NULL input\n");
        return 1;
    }

    // Create a socket (endpoint)
    // AF_INET stand for IPv4 address family
    // SOCK_STREAM for TCP protocol
    // Refer: https://linux.die.net/man/2/socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("[ERROR] socket(): Could not initialize socket");

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;				// Use IPv4 address family
    serv_addr.sin_addr.s_addr = INADDR_ANY;		// Accept any incoming messages

    // When port is used by another program or not release by OS,
    // let OS choose random port by binding to port 0
    uint16_t port = *i_port;
    serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("[ERROR] bind(): could not bind socket at port %d\n", port--);

		serv_addr.sin_port = 0;
		bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	}

    if (listen(sockfd, BUFFSIZE_DATA) != 0)
    {
        printf("[ERROR] listen(): Could not listen at port %d\n", port);
        return 1;
    }

    *i_sockfd = sockfd;
    *i_port = getPort(sockfd);

    return 0;
}


/* Ask & return answer
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
}*/


/**
 * Get local IP address (exclude 127.0.0.1)
 * @return local IP address (exclude 127.0.0.1) or NULl
 */
// Ref1: http://man7.org/linux/man-pages/man3/getifaddrs.3.html
// Ref2: https://linux.die.net/man/3/inet_aton
char* getLocalIP()
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s, n;
    char host[NI_MAXHOST];

    char* ip = NULL;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

		// Filter 1
		if (family != AF_INET)
			continue;

        /* Display interface name and family (including symbolic
           form of the latter for the common families) */

        /*printf("%-8s %s (%d)\n",
               ifa->ifa_name,
               (family == AF_PACKET) ? "AF_PACKET" :
               (family == AF_INET) ? "AF_INET" :
               (family == AF_INET6) ? "AF_INET6" : "???",
               family);*/

        /* For an AF_INET* interface address, display the address */

        if (family == AF_INET || family == AF_INET6)
        {
            s = getnameinfo(ifa->ifa_addr,
                            (family == AF_INET) ? sizeof(struct sockaddr_in) :
                            sizeof(struct sockaddr_in6),
                            host, NI_MAXHOST,
                            NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            //printf("\t\taddress: <%s>\n", host);

            if (strcmp(host, "127.0.0.1") != 0)
			{
				ip = strcpy((char*)malloc(BUFFSIZE_VAR), host);
				break;
			}
        }
    }

    freeifaddrs(ifaddr);

    return ip;
}


int getPort(int sockfd)
{
	// Get port that the os kernel allocate
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(sockfd, (struct sockaddr*)&sin, &len) == -1)
    {
        printf("[ERROR] getsockname() failed.\n");
        return 0;
    } else return ntohs(sin.sin_port);
}

/**
 * Check status of a socket
 * @param socket descriptor
 * @return -1 if invalid socket
 * @return 0  if listening-socket
 * @return 1  if non-listening socket
 */
// Ref: http://stackoverflow.com/questions/10260600/check-if-socket-is-listening-in-c
enum SOCK_STATUS sockStatus(int sockfd)
{
	int val;
	socklen_t len = sizeof(val);
	if (getsockopt(sockfd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len) == -1)
		return SOCK_FREE;
	else if (val)
		return SOCK_LISTEN;
	else
		return SOCK_NON_LISTEN;
}
