import os
import signal

i = 0
ROUNDS = 100

def print_pong(signum, frame):
    global i
    os.write(1, b"Client: pong\n")
    i += 1


def run():
    signal.signal(signal.SIGUSR1, print_pong)
    while i < ROUNDS:
        os.kill(0, signal.SIGUSR2)
    os.kill(0, signal.SIGQUIT)

if __name__ == "__main__":
    run()

