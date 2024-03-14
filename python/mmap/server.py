import mmap
import os
import time

FILE_SIZE = 20

def run():
    path = '/tmp/example'
    fd = os.open(path, flags=os.O_CREAT|os.O_RDWR) # create file
    os.lseek(fd, FILE_SIZE, os.SEEK_SET)
    os.write(fd, b" ")
    os.lseek(fd, 0, os.SEEK_SET)
    mem = mmap.mmap(fd, FILE_SIZE)
    data = mem.read(4)
    mem.seek(0)
    message = b"pong"
    while data != b"ping":
        time.sleep(5e-6)
        data = mem.read(4)
        mem.seek(0)
    data = data.decode()
    while data[0] != 'e':
        print(f"Server: Received {data}")
        mem.write(message)
        print(f"Server: Sent pong")
        mem.seek(0)
        data = mem.read(4)
        while data == message:
            time.sleep(1e-6)
            mem.seek(0)
            data = mem.read(4)
        data = data.decode()
        mem.seek(0)
    mem.close()
    os.close(fd)
    os.unlink(path)


if __name__ == "__main__":
    run()