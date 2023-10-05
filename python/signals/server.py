import os
import signal

should_end = False

def end(signum, frame):
    global should_end
    os.write(1, b"End\n")
    should_end = True

def print_ping(signum, frame):
    os.write(1, b"Server: ping\n")
    os.kill(0, signal.SIGUSR1)


def run():
    signal.signal(signal.SIGUSR2, handler=print_ping)
    signal.signal(signal.SIGQUIT, end)
    signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGUSR1})

    while not should_end:
        pass

if __name__ == "__main__":
    run()

