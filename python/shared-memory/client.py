import os
import time

import sysv_ipc

ROUNDS = 100


def run():
    path = '/tmp/example'
    fd = os.open(path, flags=os.O_CREAT)
    os.close(fd)
    key = sysv_ipc.ftok(path, 42)
    mem = sysv_ipc.SharedMemory(key, size=20, flags=sysv_ipc.IPC_CREAT, mode=0o644)
    i = 0
    message = b"ping"
    while i != ROUNDS:
        mem.write(message)
        print("Client: Sent ping")
        data = mem.read(4)
        while data == message:
            time.sleep(1e-6)
            data = mem.read(4)
        data = data.decode()
        print(f"Client: Received {data}")
        i += 1
    mem.write(b"end")
    mem.detach()


if __name__ == "__main__":
    run()
