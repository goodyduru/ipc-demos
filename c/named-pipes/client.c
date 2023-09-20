#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ROUNDS 100


void run() {
    int fd, read_bytes;
    char pipe_path[] = "/tmp/ping";
    char end[] = "end";
    char pong[] = "pong";
    char buf[10];
    fd = open(pipe_path, O_RDWR);
    read_bytes = read(fd, buf, sizeof(buf));
    buf[read_bytes] = '\0';
    while ( strcmp(buf, end) != 0 ) {
        printf("Client: Received %s\n", buf);
        write(fd, pong, sizeof(pong));
        printf("Client: Sent pong\n");
        usleep(180000);
        read_bytes = read(fd, buf, sizeof(buf));
        buf[read_bytes] = '\0';
    }
    close(fd);
}
    

int main() {
    run();
}
