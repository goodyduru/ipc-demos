import os
import time

import sysv_ipc

def run():
    path = '/tmp/example'
    fd = os.open(path, flags=os.O_CREAT) # create file
    os.close(fd)
    key = sysv_ipc.ftok(path, 42)
    mem = sysv_ipc.SharedMemory(key, size=20, flags=sysv_ipc.IPC_CREAT, mode=0o644)
    data = mem.read(4)
    message = b"pong"
    while data != b"ping":
        time.sleep(5e-6)
        data = mem.read(4)
    data = data.decode()
    while data[0] != 'e':
        print(f"Server: Received {data}")
        mem.write(message)
        print(f"Server: Sent pong")
        data = mem.read(4)
        while data == message:
            time.sleep(1e-6)
            data = mem.read(4)
        data = data.decode()
    os.unlink(path)
    mem.remove()


if __name__ == "__main__":
    run()