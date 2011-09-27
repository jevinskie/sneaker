#include <sys/socket.h>

void print_socket(void) {
    printf("fake libpython socket: %p\n", socket);
    return;
}

