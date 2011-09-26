#include <sys/socket.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "uthash.h"

typedef int (*socket_ft)(int domain, int type, int protocol);

typedef ssize_t (*sendto_ft)(int socket, const void *message,
        size_t length, int flags,
        const struct sockaddr *dest_addr, socklen_t dest_len);

typedef ssize_t (*recvfrom_ft)(int socket, void *buffer,
        size_t length, int flags, struct sockaddr *address,
        socklen_t *address_len);

typedef struct fsock_tag {
    int ffd;
    int rfd;
    int cstart;
    int cend;
    int nstart;
    int nend;
    int cnt;
    int domain;
    int type;
    int protocol;
    UT_hash_handle hh;
} fsock_t;

int next_ffd = 0;

fsock_t *fsocks = NULL;

static int s1, s2;
static int cnt;

int socket(int domain, int type, int protocol) {
    static socket_ft real_socket = NULL;
    fsock_t *fsock;

    if (real_socket == NULL) {
        real_socket = dlsym((void *) -1l, "socket");
        //real_socket = dlsym(RTLD_NEXT, "socket");
    }

    fsock = malloc(sizeof(fsock_t));
    assert(fsock != NULL);

    fsock->ffd = next_ffd++;
    printf("calling real_socket\n");
    fsock->rfd = real_socket(domain, type, protocol);
    fsock->cstart = 0;
    fsock->cend = 10;
    fsock->nstart = 8;
    fsock->nend = 20;
    fsock->cnt = 0;
    fsock->domain = domain;
    fsock->type = type;
    fsock->protocol = protocol;
    HASH_ADD_INT(fsocks, ffd, fsock);

    return fsock->ffd;
}

ssize_t sendto(int ffd, const void *message, size_t length,
       int flags, const struct sockaddr *dest_addr,
       socklen_t dest_len) {
    fsock_t *fsock;
   
    static sendto_ft real_sendto = NULL;

    if (real_sendto == NULL) {
        real_sendto = dlsym((void *) -1l, "sendto");
        //real_sendto = dlsym(RTLD_NEXT, "sendto");
    }

    HASH_FIND_INT(fsocks, &ffd, fsock);
    assert(fsock != NULL);
    

    if (fsock->cnt > fsock->nstart) {
        fsock->rfd = socket(fsock->domain, fsock->type, fsock->protocol);
    }

    printf("calling real_sento\n");
    return real_sendto(fsock->rfd, message, length, flags, dest_addr, dest_len);
}

ssize_t recvfrom(int ffd, void *buffer, size_t length,
       int flags, struct sockaddr *address,
       socklen_t *address_len) {
    fsock_t *fsock;

    static recvfrom_ft real_recvfrom = NULL;

    if (real_recvfrom == NULL) {
        real_recvfrom = dlsym((void *) -1l, "recvfrom");
        //real_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
    }

    HASH_FIND_INT(fsocks, &ffd, fsock);
    assert(fsock != NULL);
    
    printf("calling real_recvfrom\n");
    return real_recvfrom(fsock->rfd, buffer, length, flags, address, address_len);
}

