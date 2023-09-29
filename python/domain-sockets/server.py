import os
import socket


def run():
    server_path = './udsocket'
    # Delete if the path does exist
    try:
        os.unlink(server_path)
    except OSError:
        pass

    server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    server.bind(server_path)

    server.listen(1)
    msg = "pong".encode()
    while True:
        connection, address = server.accept()
        print(f"Connection from {address}")
        while True:
            data = connection.recv(16).decode()
            if data != "ping":
                break
            print(f"Server: Received {data}")
            connection.sendall(msg)
            print(f"Server: Sent pong")
        if data == "end":
            print("Server: Connection shutting down")
            connection.close()
        else:
            print(f"Server Error: received invalid message {data}, shutting down connection")
            connection.close()


if __name__ == "__main__":
    run()
