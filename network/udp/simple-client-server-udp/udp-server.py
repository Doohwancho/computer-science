import socket

HOST = socket.gethostname()
IP = socket.gethostbyname(HOST)
PORT = 9999

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as server :
    server.bind((IP, PORT))
    clients_connected = []

    while True :
        message_received, address = server.recvfrom(1024)

        clients_connected.append(address)
        print(f"Message Received from {address} : {message_received.decode()}")

        if message_received.decode() == "" :
            print(f"Closing connection with {address}")
            clients_connected.remove(address)

        msg = input("Enter message : ")
        for client in clients_connected :
            server.sendto(msg.encode(), client)
