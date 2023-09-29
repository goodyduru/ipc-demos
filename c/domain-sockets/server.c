#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_PATH "udsocket"

int main(void) {
    int sock, client, len;
    char pong[] = "pong";
    struct sockaddr_un client_addr, local_addr = {
        .sun_family = PF_LOCAL
    };
    char data[16];

    if ( (sock = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1 ) {
        perror("socket");
        exit(1);
    }

    strcpy(local_addr.sun_path, SERVER_PATH);
    unlink(local_addr.sun_path);
    len = strlen(local_addr.sun_path) + sizeof(local_addr.sun_family) + 1; // Add 1 to account for null byte in sun_path
    if ( bind(sock, (struct sockaddr *)&local_addr, len) == -1 ) {
        perror("bind");
        exit(1);
    }

    if ( listen(sock, 1) == - 1 ) {
        perror("listen");
        exit(1);
    } 

    // Loop for ever, it's a server duh.
    while(1) {
        socklen_t slen = sizeof(client_addr);
        int n;
        if ( (client = accept(sock, (struct sockaddr *) &client_addr, &slen)) == -1 ) {
            perror("accept");
            exit(1);
        }

        printf("Connected\n");
        do {
            n = recv(client, data, sizeof(data), 0);
            if ( n < 0 ) {
                perror("recv");
                break;
            }

            if ( strncmp("ping", data, 4) == 0 ) {
                printf("Server: Received %s\n", data);
                if ( send(client, pong, strlen(pong)+1, 0) < 0 ) {
                    perror("send");
                    break;
                }
                printf("Server: Sent pong\n");

            } else if ( strncmp("end", data, 3) == 0 ) {
                printf("Server: Connection shutting down\n");
                break;
            }
            else {
                printf("Server Error: received invalid message %s, shutting down connection\n", data);
                break;
            }
        } while ( strncmp("ping", data, 4) == 0 );
        close(client);
    }
}