import os

import sysv_ipc

ROUNDS = 100


def run():
    path = '/tmp/example'
    fd = os.open(path, flags=os.O_CREAT)
    os.close(fd)
    key = sysv_ipc.ftok(path, 42)
    mq = sysv_ipc.MessageQueue(key, flags=sysv_ipc.IPC_CREAT, mode=0o644)
    msg_type = 10
    i = 0
    while i != ROUNDS:
        mq.send(b"ping", type=msg_type)
        print("Client: Sent ping")
        data, _ = mq.receive(type=(msg_type+1))
        data = data.decode()
        print(f"Client: Received {data}")
        i += 1
    mq.send(b"end", type=msg_type)


if __name__ == "__main__":
    run()
