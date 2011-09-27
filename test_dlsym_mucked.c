#include <stdio.h>
#include <sys/socket.h>

int main(void) {
    printf("test_dlsym_mucked socket: %p\n", socket);
    return 0;
}

