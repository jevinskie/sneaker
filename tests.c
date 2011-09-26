#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(void) {
    int sock;
    struct sockaddr_in serv_addr, cli_addr;
    char buf[256];
    ssize_t n;
    socklen_t len;

    puts("starting server");

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(31337);
    bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    len = sizeof(cli_addr);

    while (1) {
        n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr, &len);
        fputs(buf, stdout);
    }

    return 0;
}

