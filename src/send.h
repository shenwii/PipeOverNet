#ifndef _SEND_H
#define _SEND_H

#if defined _WIN32 || defined __CYGWIN__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/**
 * @brief read data from stdin and send to some server
 * 
 * @param addr server sockaddr
 * @return int 0: normal
 */
int pon_send(struct sockaddr *addr);

#endif
