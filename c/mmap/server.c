#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define ROUNDS 100
#define FILE_SIZE 20

void run() {
    char type = 's'; // Used to denote server sent message

    char path[] = "/tmp/example";
    int fd = open(path, O_RDWR|O_CREAT, 0600);

    lseek(fd, FILE_SIZE, SEEK_SET);
    write(fd, " ", 1);
    lseek(fd, 0, SEEK_SET);
    

    char *addr = mmap(NULL, FILE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    while ( *addr != 'c' ) { // If client hasn't sent message sleep for some microseconds
        usleep(5);
    }
    while ( memcmp(addr+1, "end\0", 4) != 0 ) {
        printf("Server: Received %s\n", addr+1);
        *addr = type;
        memcpy(addr+1, "pong\0", 5);
        printf("Server: Sent pong\n");
        while ( *addr == type ) { // If server sent message still exists
            usleep(1);
        }
    }

    munmap(addr, FILE_SIZE);
    close(fd);
    unlink(path);
}

int main() {
    run();
}
