#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define ROUNDS 100


void run() {
    int fd, read_bytes, i = 0;
    char pipe_path[] = "/tmp/ping";
    char end[] = "end";
    char ping[] = "ping";
    char buf[10];
    mkfifo(pipe_path, S_IFIFO|0640);
    fd = open(pipe_path, O_RDWR);
    while ( i != ROUNDS ) {
        printf("Server: Sent ping\n");
        write(fd, ping, sizeof(ping));
        usleep(100000);
        read_bytes = read(fd, buf, sizeof(buf));
        buf[read_bytes] = '\0';
        printf("Server: Received %s\n", buf);
        i++;
    }
    write(fd, end, sizeof(end));
    close(fd);
    unlink(pipe_path);
}
    

int main() {
    sleep(1);
    run();
}
