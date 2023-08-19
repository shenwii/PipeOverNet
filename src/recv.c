#include "recv.h"

#include <stdio.h>
#include <unistd.h>

#if defined _WIN32 || defined __CYGWIN__
#define MSG_NOSIGNAL 0
#endif

#define BUFFER_SIZE 4096

int pon_recv(struct sockaddr *addr) {
    char buf[BUFFER_SIZE];
    struct sockaddr_storage accept_addr;
    unsigned int accept_addr_len;
    size_t read;
    int rtn = 0;
    //create socket
    int fd = socket(addr->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (fd <= 0) {
        perror("socket init failed");
        return 1;
    }
    //bind
    if (bind(fd, addr, addr->sa_family == AF_INET6 ? sizeof(struct sockaddr_in6): sizeof(struct sockaddr_in)) != 0) {
        perror("bind failed");
        close(fd);
        return 1;
    }
    //listen
    if (listen(fd, 1) != 0) {
        perror("listen failed");
        close(fd);
        return 1;
    }
    //accept socket
    int accept_fd = accept(fd, (struct sockaddr *) &accept_addr, &accept_addr_len);
    if (accept_fd <= 0) {
        perror("accept failed");
        close(fd);
        return 1;
    }
    //loop recv data from socket
    while ((read = recv(accept_fd, buf, BUFFER_SIZE, MSG_NOSIGNAL)) > 0) {
        //write data to stdout
        if (fwrite(buf, 1, read, stdout) <= 0) {
            perror("write failed");
            rtn = 1;
            break;
        }
    }
    if (read < 0) {
        perror("recv failed");
        rtn = 1;
    }
    close(accept_fd);
    close(fd);
    return rtn;
}