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
    char type = 's'; // Used to denote server sent message
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

    if ( shmctl(id, IPC_RMID, NULL) == -1 ) {
        perror("shmctl");
        exit(1);
    }
    unlink(path);
}

int main() {
    run();
}
