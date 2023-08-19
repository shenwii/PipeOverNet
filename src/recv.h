#ifndef _RECV_H
#define _RECV_H

#if defined _WIN32 || defined __CYGWIN__
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/**
 * @brief read data from socker, send to stdout
 * 
 * @param addr listen sockaddr
 * @return int 0: normal
 */
int pon_recv(struct sockaddr *addr);

#endif
