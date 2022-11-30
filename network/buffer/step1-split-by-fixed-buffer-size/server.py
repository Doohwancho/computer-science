import socket

IP = socket.gethostbyname(socket.gethostname())
PORT = 9999

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server :
    server.bind((IP, PORT))
    server.listen(5)

    while True :
        conn, addr = server.accept()

        msg = "Welcome to the server"

        conn.send(bytes("Welcome to the server!", "utf-8"))
