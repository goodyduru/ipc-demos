import socket

ROUNDS = 100

def run():
    server_address = './udsocket'
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(server_address)

    print(f"Connecting to {server_address}")
    i = 0
    msg = "ping".encode()
    while i < ROUNDS:
        sock.sendall(msg)
        print("Client: Sent ping")

        data = sock.recv(16)
        print(f"Client: Received {data.decode()}")
        i += 1
    sock.sendall("end".encode())
    sock.close()

if __name__ == "__main__":
    run()
