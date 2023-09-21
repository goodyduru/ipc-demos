import os


ROUNDS = 100


def run():
    pipe_path = '/tmp/ping'
    fd = os.open(pipe_path, os.O_WRONLY)
    i = 0
    while i != ROUNDS:
        os.write(fd, b'ping')
        print("Client: Sent ping")
        i += 1
    os.write(fd, b'end')
    os.close(fd)


if __name__ == "__main__":
    run()
