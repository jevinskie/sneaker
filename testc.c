#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(void) {
    int sock;
    int i;
    struct sockaddr_in serv_addr;
    char buf[256];

    puts("starting client");

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(31337);

    for (i = 0; i < 16; i++) {
        strcpy(buf, "hello, world\n");
        sendto(sock, buf, strlen(buf)+1, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    }

    return 0;
}

