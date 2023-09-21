import os


def run():
    pipe_path = '/tmp/ping'
    os.mkfifo(pipe_path)
    fd = os.open(pipe_path, os.O_RDONLY)
    data = os.read(fd, 4).decode()
    while data != 'end':
        print(f"Server: Received {data}")
        data = os.read(fd, 4).decode()
    os.close(fd)
    os.unlink(pipe_path)


if __name__ == "__main__":
    run()
