import socket

HOST = socket.gethostname()
IP = socket.gethostbyname(HOST)
PORT = 8888

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server :

    server.bind((IP, PORT))

    server.listen(5) #how many connections to accept?

    while True :
        client, client_address = server.accept()

        with client :
            reply = client.recv(1024).decode()

            if reply != "" :
                print(f"Message from the server : {reply}")

                msg = input("Type message to send to the client : ")
                client.sendall(msg.encode())

        if reply == "" :
            print("Closing connection")
            break
