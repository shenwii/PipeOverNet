#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#if defined _WIN32 || defined __CYGWIN__
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include "recv.h"
#include "send.h"

//default port
#define DEFAULT_PORT 9347

/**
 * @brief print usage info
 * 
 * @param prog program name
 */
void __usage(char *prog) {
    fprintf(stderr, "Usage: %s send|recv host[:port]\n", prog);
}

/**
 * @brief resolve host
 * 
 * @param host ip or host
 * @param addr sockaddr
 * @return int 0: normal
 */
int __resolve_host(__const__ char *host, struct sockaddr *addr) {
    struct addrinfo *result = NULL;
    if (getaddrinfo(host, NULL, NULL, &result) != 0) {
        return 1;
    }
    if (result == NULL) {
        return 1;
    }
    if(result->ai_family == AF_INET) { //ipv4
        memcpy(addr, result->ai_addr, sizeof(struct sockaddr_in));
    } else if(result->ai_family == AF_INET6) { //ipv6
        memcpy(addr, result->ai_addr, sizeof(struct sockaddr_in6));
    } else {
        freeaddrinfo(result);
        return 1;
    }
    freeaddrinfo(result);
    return 0;
}

/**
 * @brief parse host[:port] string
 * 
 * @param hostport host[:port] string
 * @param addr sockaddr
 * @return int 0: normal
 */
int __parse_hostport(char *hostport, struct sockaddr *addr) {
    char *hoststr;
    uint16_t port = DEFAULT_PORT;
    char *portfind = strstr(hostport, ":");
    if (portfind == NULL) {
        hoststr = hostport;
    } else {
        char *portstr = portfind + 1;
        port = atoi(portstr);
        if (port == 0) {
            fprintf(stderr, "port can't be 0\n");
            return 1;
        }
        *portfind = '\0';
        hoststr = hostport;
    }
    if (__resolve_host(hoststr, addr) != 0) {
        return 1;
    }
    if (addr->sa_family == AF_INET) {
        ((struct sockaddr_in *) addr)->sin_port = htons(port);
    } else {
        ((struct sockaddr_in6 *) addr)->sin6_port = htons(port);
    }
    return 0;
}

int main(int argc, char **argv) {
    struct sockaddr_storage addr;
    if (argc != 3) {
        __usage(argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "send") == 0) {
        if (__parse_hostport(argv[2], (struct sockaddr *) &addr) != 0) {
            fprintf(stderr, "host[:port] not right\n");
            return 1;
        }
        return pon_send((struct sockaddr *) &addr);
    } else if (strcmp(argv[1], "recv") == 0) {
        if (__parse_hostport(argv[2], (struct sockaddr *) &addr) != 0) {
            fprintf(stderr, "host[:port] not right\n");
            return 1;
        }
        return pon_recv((struct sockaddr *) &addr);
    }
    __usage(argv[0]);
    return 1;
}