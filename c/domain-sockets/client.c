#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "udsocket"
#define ROUNDS 100

int main(void) {
    int sock, len, i;
    char ping[] = "ping";
    char end[] = "end";
    struct sockaddr_un remote_addr = {
        .sun_family = PF_LOCAL
    };
    char data[16];

    if ( (sock = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1 ) {
        perror("socket");
        exit(1);
    }
    strcpy(remote_addr.sun_path, SERVER_PATH);
    printf("Connecting to %s\n",remote_addr.sun_path);
    len = strlen(remote_addr.sun_path) + sizeof(remote_addr.sun_family) + 1; // We add 1 to account for null byte.
    if ( connect(sock, (struct sockaddr *)&remote_addr, len) == -1 ) {
        perror("connect");
        exit(1);
    }
    i = 0;

    // Send `ping` a 100 times
    while(i < ROUNDS) {
        if ( send(sock, ping, strlen(ping)+1, 0) < 0 ) {
            perror("send");
            break;
        }
        printf("Client: Sent ping\n");
        len = recv(sock, data, sizeof(data), 0);
        if ( len == 0 ) {
            printf("Server closed the connection\n");
            break;
        }
        if ( len < 0 ) {
            perror("recv");
            break;
        }
        printf("Client: Received %s\n", data);
        i++;
    } 

    // Send `end` to signify end of communication
    if ( send(sock, end, strlen(end)+1, 0) < 0 ) {
        perror("recv");
    }
    close(sock);
}