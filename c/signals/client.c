#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ROUNDS 100
volatile sig_atomic_t i;
char pong[] = "Client: pong\n";

void print_pong(int signum) {  
    write(1, pong, strlen(pong));
    i++;
}

void run() {
    struct sigaction sa = {
        .sa_handler = print_pong,
        .sa_mask = 0,
        .sa_flags = SA_RESTART,
    };

    if ( sigaction(SIGUSR1, &sa, NULL) == -1 ) {
        exit(1);
    } 

    sigaddset(&(sa.sa_mask), SIGUSR2);
    sigprocmask(SIG_BLOCK, &(sa.sa_mask), NULL);

    while ( i < ROUNDS ) {
        kill(0, SIGUSR2);
    }
    kill(0, SIGQUIT);
}

int main() {
    i = 0;
    run();
}
