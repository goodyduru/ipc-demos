import os
import time

ROUNDS = 100


def run():
    pipe_path = '/tmp/ping'
    os.mkfifo(pipe_path, 416)
    fd = os.open(pipe_path, os.O_RDWR)
    i = 0
    while i != ROUNDS:
        print("Server: Sent ping")
        os.write(fd, b'ping')
        time.sleep(0.1)
        data = os.read(fd, 4).decode()
        print(f"Server: Received {data}")
        i += 1
    os.write(fd, b'end')
        
    os.close(fd)
    os.unlink(pipe_path)


if __name__ == "__main__":
    time.sleep(1)
    run()
