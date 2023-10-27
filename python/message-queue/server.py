import os

import sysv_ipc

def run():
    path = '/tmp/example'
    fd = os.open(path, flags=os.O_CREAT) # create file
    os.close(fd)
    key = sysv_ipc.ftok(path, 42)
    mq = sysv_ipc.MessageQueue(key, flags=sysv_ipc.IPC_CREAT, mode=0o644)
    msg_type = 10
    data, _ = mq.receive(type=msg_type)
    data = data.decode()
    while data != 'end':
        print(f"Server: Received {data}")
        mq.send(b"pong", type=(msg_type+1))
        print(f"Server: Sent pong")
        data, _ = mq.receive(type=msg_type)
        data = data.decode()
    os.unlink(path)
    mq.remove()


if __name__ == "__main__":
    run()