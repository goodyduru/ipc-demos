#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define ROUNDS 100
#define FILE_SIZE 20

void run() {
    char type = 'c'; // Used to denote client sent message

    char path[] = "/tmp/example";
    int fd = open(path, O_RDWR|O_CREAT, 0600);

    lseek(fd, FILE_SIZE, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);

    char *addr = mmap(NULL, FILE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    for ( int i = 0; i < ROUNDS; i++ ) {
        *addr = type;
        memcpy(addr+1, "ping\0", 5);
        printf("Client: Sent ping\n");
        while ( *addr == type ) { // If client sent message still exists
            usleep(1);
        }
        printf("Client: Received %s\n", addr+1);
    }
    *addr = type;
    memcpy(addr+1, "end\0", 4);
    munmap(addr, FILE_SIZE);
}

int main() {
    run();
}
