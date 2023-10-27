#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define ROUNDS 100

struct cl_msgbuf {
    long mtype;
    char text[10];
};

void run() {
    struct cl_msgbuf buf;
    int id;
    key_t key;
    char path[] = "/tmp/example";
    int fd = open(path, O_RDONLY|O_CREAT, 0440);
    long msg_type = 10;
    close(fd);

    if ( (key = ftok(path, 42)) == -1 ) {
        perror("ftok");
        exit(1);
    }

    if ( (id = msgget(key, 0644 | IPC_CREAT)) == -1 ) {
        perror("msgget");
        exit(1);
    }

    
    for ( int i = 0; i < ROUNDS; i++ ) {
        buf.mtype = msg_type;
        strcpy(buf.text, "ping");
        buf.text[4] = '\0'; // Add null character for string comparison purpose
        if ( msgsnd(id, &buf, strlen(buf.text)+1, 0) == -1 ) { // Account for null character
            perror("msgsnd");
            exit(1);
        }
        printf("Client: Sent ping\n");

        if ( msgrcv(id, &buf, sizeof(buf.text), msg_type+1, 0) == -1 ) {
            perror("msgrcv");
            exit(1);
        }
        printf("Client: Received %s\n", buf.text);
    }

    buf.mtype = msg_type;
    strcpy(buf.text, "end");
    buf.text[3] = '\0';
    if ( msgsnd(id, &buf, strlen(buf.text)+1, 0) == -1 ) {
        perror("msgsnd");
        exit(1);
    }
}

int main() {
    run();
}
