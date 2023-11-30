#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>

#define ROUNDS 100

void run() {
    int id;
    key_t key;
    char path[] = "/tmp/example";
    int fd = open(path, O_RDONLY|O_CREAT, 0440);
    char type = 'c'; // Used to denote client sent message
    close(fd);

    if ( (key = ftok(path, 42)) == -1 ) {
        perror("ftok");
        exit(1);
    }

    if ( (id = shmget(key, 20, 0600 | IPC_CREAT)) == -1 ) {
        perror("msgget");
        exit(1);
    }

    char *addr = shmat(id, NULL, 0);
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
}

int main() {
    run();
}
