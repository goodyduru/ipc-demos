import os
import time

ROUNDS = 100


def run():
    pipe_path = '/tmp/ping'
    fd = os.open(pipe_path, os.O_RDWR)
    data = os.read(fd, 4).decode()
    while data != 'end':
        print(f"Client: Received {data}")
        os.write(fd, b"pong")
        print(f"Client: Sent pong")
        time.sleep(0.1)
        data = os.read(fd, 4).decode()
    os.close(fd)


if __name__ == "__main__":
    time.sleep(2)
    run()
