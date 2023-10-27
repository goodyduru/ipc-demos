#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

struct sv_msgbuf {
    long mtype;
    char text[10];
};

void run() {
    struct sv_msgbuf buf;
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

    if ( msgrcv(id, &buf, sizeof(buf.text), msg_type, 0) == -1 ) {
        perror("msgrcv");
        exit(1);
    }
    while ( strcmp(buf.text, "end") != 0 ) {
        printf("Server: Received %s\n", buf.text);
        buf.mtype = msg_type + 1;
        strcpy(buf.text, "pong");
        buf.text[4] = '\0';
        if ( msgsnd(id, &buf, strlen(buf.text)+1, 0) == -1 ) {
            perror("msgsnd");
            exit(1);
        }
        printf("Server: Sent pong\n");

        if ( msgrcv(id, &buf, sizeof(buf.text), msg_type, 0) == -1 ) {
            perror("msgrcv");
            exit(1);
        }
    }

    if ( msgctl(id, IPC_RMID, NULL) == -1 ) {
        perror("msgctl");
        exit(1);
    }
    unlink(path);
}

int main() {
    run();
}
