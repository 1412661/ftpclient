#ifndef _NET_H_
#define _NET_H_

#include <stdint.h>
#include "const.h"
#include "type.h"

// http://www.binarytides.com/hostname-to-ip-address-c-sockets-linux/
// This function is a modified version of the hostname_to_ip()
// Credit goes to me 1412661
/**
 * Resolve IP address from hostname
 * @param Hostname
 * @return IP address in 32 bit unsigned integer format
 */
uint32_t getaddrbyname(char *hostname);


/**
 * Open a socket and connect to target host:port
 * @param Socket descriptor (will be modified if function return 0)
 * @param Source port (will be modified if function return 0)
 * @param Target host
 * @param Target port
 * @return 0 if success, 1 if fail.
 */
int portConnect(int* i_sockfd, int* i_port, char* host, int port);


/**
 * Open a socket to listen
 * @param Socket descriptor (will be modified if function return 0)
 * @param Port to listen (will be modified if function return 0)
 * @return 0 if success, 1 if fail.
 */
int portListen(int* i_sockfd, int* i_port);


/**
 * Get local IP address (exclude 127.0.0.1)
 * @return local IP address (exclude 127.0.0.1) or NULl
 */
 // Ref1: http://man7.org/linux/man-pages/man3/getifaddrs.3.html
 // Ref2: https://linux.die.net/man/3/inet_aton
char* getLocalIP();

/**
 * Get port from socket descriptor
 * @param socket descriptor
 * @return port associated with socket descriptor, 0 if fail
 */
int getPort(int sockfd);

/**
 * Check status of a socket
 * @param socket descriptor
 * @return -1 if invalid socket
 * @return 0  if listening-socket
 * @return 1  if non-listening socket
 */
// Ref: http://stackoverflow.com/questions/10260600/check-if-socket-is-listening-in-c
enum SOCK_STATUS sockStatus(int sockfd);

#endif // _NET_H_
