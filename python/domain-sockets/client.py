import socket

ROUNDS = 100

def run():
    server_address = './udsocket'
    client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    client.connect(server_address)

    print(f"Connecting to {server_address}")
    i = 0
    msg = "ping".encode()
    while i < ROUNDS:
        client.sendall(msg)
        print("Client: Sent ping")

        data = client.recv(16)
        print(f"Client: Received {data.decode()}")
        i += 1
    client.sendall("end".encode())
    client.close()

if __name__ == "__main__":
    run()
