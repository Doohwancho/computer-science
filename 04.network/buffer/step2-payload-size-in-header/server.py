import socket

HEADERSIZE = 10
IP = socket.gethostbyname(socket.gethostname())
PORT = 1233

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server :
    server.bind((IP, PORT))
    server.listen(5)

    while True :
        client, addr = server.accept()
        print(f"Connection from {addr} has been established")

        msg = "Welcome to the server"
        msg = f"{len(msg):<{HEADERSIZE}}" + msg
        print(msg)
        client.send(bytes(msg, "utf-8"))
