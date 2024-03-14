import mmap
import os
import time

ROUNDS = 100
FILE_SIZE = 20


def run():
    path = '/tmp/example'
    fd = os.open(path, flags=os.O_CREAT|os.O_RDWR)
    os.lseek(fd, FILE_SIZE, os.SEEK_SET)
    os.write(fd, b" ")
    os.lseek(fd, 0, os.SEEK_SET)
    mem = mmap.mmap(fd, FILE_SIZE)
    i = 0
    message = b"ping"
    while i != ROUNDS:
        mem.write(message)
        print("Client: Sent ping")
        mem.seek(0)
        data = mem.read(4)
        while data == message:
            time.sleep(1e-6)
            mem.seek(0)
            data = mem.read(4)
        data = data.decode()
        print(f"Client: Received {data}")
        mem.seek(0)
        i += 1
    mem.write(b"end")
    mem.close()
    os.close(fd)


if __name__ == "__main__":
    run()
