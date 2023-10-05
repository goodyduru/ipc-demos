#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t is_ended;
char ping[] = "Server: ping\n";

void print_ping(int signum) {  
    if ( signum == SIGQUIT ) {     
        write(1, "End\n", 4);
        is_ended = 1; // End the while loop
    }
    else {
        write(1, ping, strlen(ping));
        kill(0, SIGUSR1);
    }
}

void run() {
    struct sigaction sa = {
        .sa_handler = print_ping,
        .sa_mask = 0,
        .sa_flags = SA_RESTART,
    };

    if ( sigaction(SIGUSR2, &sa, NULL) == -1  || sigaction(SIGQUIT, &sa, NULL) == -1 ) {
        exit(1);
    }

    while ( !is_ended );
}

int main() {
    is_ended = 0;
    run();
}
