#include "send.h"

#include <stdio.h>
#include <unistd.h>

#if defined _WIN32 || defined __CYGWIN__
#define MSG_NOSIGNAL 0
#endif

#define BUFFER_SIZE 4096

int pon_send(struct sockaddr *addr) {
    char buf[BUFFER_SIZE];
    size_t read;
    int rtn = 0;
    //create socket
    int fd = socket(addr->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (fd <= 0) {
        perror("socket init failed");
        return 1;
    }
    //connect to server
    if (connect(fd, addr, addr->sa_family == AF_INET6 ? sizeof(struct sockaddr_in6): sizeof(struct sockaddr_in)) != 0) {
        perror("connect failed");
        close(fd);
        return 1;
    }
    //loop read data from stdin
    while ((read = fread(buf, 1, BUFFER_SIZE, stdin)) > 0) {
        //send data to socket
        if (send(fd, buf, read, MSG_NOSIGNAL) <= 0) {
            perror("send failed");
            rtn = 1;
            break;
        }
    }
    if (read < 0) {
        perror("read failed");
        rtn = 1;
    }
    close(fd);
    return rtn;
}